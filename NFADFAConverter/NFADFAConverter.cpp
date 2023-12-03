#include "NFADFAConverter.h"

// This should never start from zero, since it is the default value for a map that has int as a value
#define DFAALIASSTART 1

struct HashVectorInt
{
    size_t operator()(const vector<int> &vec) const
    {
        size_t ret = vec.size();
        for (int i : vec)
        {
            ret ^= i + 0x9e3779b9 + (ret << 6) + (ret >> 2);
        }
        return ret;
    }
};

template <typename T>
unordered_set<T> unorderedSetsIntersection(const unordered_set<T> &first, const unordered_set<T> &second)
{
    unordered_set<T> result;
    for (T element : first)
        if (second.find(element) != second.end())
            result.insert(element);
    return result;
}

string findHighestPeriorityRE(vector<string> &regularExpressions, unordered_map<string, int> &regularExpressionsPriorityMap)
{
    string highestPeriorityRE = regularExpressions[0];
    int highestPeriority = regularExpressionsPriorityMap[highestPeriorityRE];
    for (string regularExpression : regularExpressions)
    {
        int priority = regularExpressionsPriorityMap[regularExpression];
        if (priority < highestPeriority)
        {
            highestPeriority = priority;
            highestPeriorityRE = regularExpression;
        }
    }
    return highestPeriorityRE;
}

unordered_set<Node *> findEClosure(unordered_set<Node *> nodes)
{
    stack<Node *> stack;
    unordered_set<Node *> eClosure;
    for (Node *node : nodes)
    {
        stack.push(node);
        eClosure.insert(node);
    }
    while (!stack.empty())
    {
        Node *node = stack.top();
        stack.pop();
        for (Node *nextNode : node->getNextNodes((char)238))
            if (eClosure.find(nextNode) == eClosure.end())
            {
                stack.push(nextNode);
                eClosure.insert(nextNode);
            }
    }
    return eClosure;
}

unordered_set<Node *> move(unordered_set<Node *> &sources, char input)
{
    unordered_set<Node *> destinations;
    for (Node *source : sources)
    {
        vector<Node *> nextNodes = source->getNextNodes(input);
        destinations.insert(nextNodes.begin(), nextNodes.end());
    }
    return destinations;
}

unordered_set<char> findAllPossibleInputs(Node *startNode)
{
    unordered_set<char> allPossibleInputs;
    unordered_set<Node *> markedNodes;
    queue<Node *> nodesQueue;
    nodesQueue.push(startNode);
    while (!nodesQueue.empty())
    {
        Node *unmarkedNode = nodesQueue.front();
        nodesQueue.pop();
        unordered_map<char, vector<Node *>> transitions = unmarkedNode->getNextNodes();
        for (auto const &transition : transitions)
        {
            allPossibleInputs.insert(transition.first);
            for (Node *destinationNode : transition.second)
                if (markedNodes.find(destinationNode) == markedNodes.end())
                {
                    nodesQueue.push(destinationNode);
                    markedNodes.insert(destinationNode);
                }
        }
    }
    return allPossibleInputs;
}

unordered_map<int, string> determineFinalStatesMap(
    unordered_map<unordered_set<Node *>, int, HashUnorderedSetNodePointers> dfaStatesNodesMap,
    unordered_map<Node *, string> finalNodesMap,
    unordered_map<string, int> &regularExpressionsPriorityMap)
{
    unordered_map<int, string> finalStatesMap;
    unordered_set<Node *> finalNodes;
    for (auto const &finalNodesMapElement : finalNodesMap)
        finalNodes.insert(finalNodesMapElement.first);
    for (auto const &dfaStatesNodesMapElement : dfaStatesNodesMap)
    {
        unordered_set<Node *> finalNodesIntersection = unorderedSetsIntersection<Node *>(dfaStatesNodesMapElement.first, finalNodes);
        if (finalNodesIntersection.empty())
            continue;
        vector<string> regularExpressions;
        for (Node *finalNode : finalNodesIntersection)
            regularExpressions.push_back(finalNodesMap[finalNode]);
        finalStatesMap[dfaStatesNodesMapElement.second] = findHighestPeriorityRE(regularExpressions, regularExpressionsPriorityMap);
    }
    return finalStatesMap;
}

pair<unordered_map<int, unordered_map<char, int>>, unordered_map<int, string>> findDFAInfo(CombinedAutomaton nfa,
                                                                                           unordered_map<string, int> &regularExpressionsPriorityMap,
                                                                                           unordered_set<char> allPossibleInputs)
{
    // Phi state will have alias = 0 by default, and start dfa state will have alias = DFAALIASSTART
    int alias = DFAALIASSTART;
    unordered_map<unordered_set<Node *>, int, HashUnorderedSetNodePointers> statesNodesMap;
    unordered_map<int, unordered_set<Node *>> statesAliasMap;
    unordered_set<int> allStates;
    queue<int> unmarkedStates;
    unordered_map<int, unordered_map<char, int>> statesTransitions;
    unordered_set<Node *> nextStateNodes = findEClosure({nfa.getStartNode()});
    statesNodesMap[nextStateNodes] = alias;
    statesAliasMap[alias] = nextStateNodes;
    allStates.insert(alias);
    unmarkedStates.push(alias);
    alias++;
    while (!unmarkedStates.empty())
    {
        int currentState = unmarkedStates.front();
        unmarkedStates.pop();
        unordered_map<char, int> transitions;
        for (const char input : allPossibleInputs)
        {
            nextStateNodes = findEClosure(move(statesAliasMap[currentState], input));
            if (allStates.find(statesNodesMap[nextStateNodes]) == allStates.end())
            {
                statesNodesMap[nextStateNodes] = alias;
                statesAliasMap[alias] = nextStateNodes;
                allStates.insert(alias);
                unmarkedStates.push(alias);
                alias++;
            }
            transitions[input] = statesNodesMap[nextStateNodes];
        }
        statesTransitions.insert({currentState, transitions});
    }
    return {statesTransitions, determineFinalStatesMap(statesNodesMap, nfa.getFinalNodesMap(), regularExpressionsPriorityMap)};
}

pair<vector<vector<int>>, unordered_map<int, int>> findDFAInitialGroupsInfo(vector<int> &allDFAStates,
                                                                            unordered_map<int, string> &dfaFinalStatesMap)
{
    unordered_map<int, int> statesGroupMap;
    unordered_map<string, vector<int>> reFinalStatesMap;
    vector<vector<int>> groups;
    vector<int> nonFinalStates;
    for (int &state : allDFAStates)
        if (dfaFinalStatesMap.find(state) == dfaFinalStatesMap.end())
            nonFinalStates.push_back(state);
    groups.push_back(nonFinalStates);
    for (auto const &element : dfaFinalStatesMap)
        if (reFinalStatesMap.find(element.second) == reFinalStatesMap.end())
            reFinalStatesMap[element.second] = vector<int>{element.first};
        else
            reFinalStatesMap[element.second].push_back(element.first);
    for (auto const &element: reFinalStatesMap)
        groups.push_back(element.second);
    for (int groupNumber = 1; groupNumber <= groups.size(); groupNumber++)
        for (int &state : groups[groupNumber - 1])
            statesGroupMap[state] = groupNumber;
    return {groups, statesGroupMap};
}

unordered_map<int, int> minimizeDFAStates(unordered_map<int, int> &statesGroupMap,
                                          vector<vector<int>> &groups,
                                          unordered_map<int, unordered_map<char, int>> &dfaTransitionsMap,
                                          unordered_set<char> &allPossibleInputs)
{
    unordered_map<int, int> newStatesGroupMap;
    vector<vector<int>> newGroups;
    int groupCounter = 1;
    for (vector<int> &group : groups)
    {
        unordered_map<vector<int>, vector<int>, HashVectorInt> inputsGroupStates;
        for (int &state : group)
        {
            vector<int> stateInputsGroup = vector<int>(allPossibleInputs.size());
            unordered_map<char, int> stateTransitions = dfaTransitionsMap[state];
            int i = 0;
            for (char input : allPossibleInputs)
            {
                stateInputsGroup[i] = statesGroupMap[stateTransitions[input]];
                i++;
            }
            inputsGroupStates[stateInputsGroup].push_back(state);
        }
        for (auto const &element : inputsGroupStates)
        {
            vector<int> newGroup = vector<int>(element.second.size());
            int i = 0;
            for (int state : element.second)
            {
                newStatesGroupMap[state] = groupCounter;
                newGroup[i] = (state);
                i++;
            }
            newGroups.push_back(newGroup);
            groupCounter++;
        }
    }
    if (newGroups.size() == groups.size())
        return newStatesGroupMap;
    return minimizeDFAStates(newStatesGroupMap, newGroups, dfaTransitionsMap, allPossibleInputs);
}

CombinedAutomaton constructDFA(unordered_map<int, int> &statesGroupMap,
                               unordered_map<int, unordered_map<char, int>> &dfaTransitionsMap,
                               unordered_map<int, string> &dfaFinalStatesMap,
                               unordered_map<string, int> &regularExpressionsPriorityMap)
{
    CombinedAutomaton dfaAutomaton;
    int startGroup = statesGroupMap[DFAALIASSTART];
    unordered_map<int, string> finalGroupsMap;
    for (auto const &finalState : dfaFinalStatesMap)
    {
        int group = statesGroupMap[finalState.first];
        if (finalGroupsMap.find(group) == finalGroupsMap.end())
            finalGroupsMap[group] = finalState.second;
        else if (regularExpressionsPriorityMap[finalGroupsMap[group]] > regularExpressionsPriorityMap[finalState.second])
            finalGroupsMap[group] = finalState.second;
    }
    unordered_map<int, Node *> groupsNodeMap;
    unordered_set<int> finishedGroups;
    unordered_map<Node *, string> finalNodesMap;
    for (auto const &element : dfaTransitionsMap)
    {
        int group = statesGroupMap[element.first];
        if (finishedGroups.find(group) == finishedGroups.end())
        {
            unordered_map<char, int> stateTransitions = element.second;
            Node *node;
            if (groupsNodeMap.find(group) == groupsNodeMap.end())
            {
                node = new Node();
                groupsNodeMap[group] = node;
            }
            node = groupsNodeMap[group];
            if (finalGroupsMap.find(group) != finalGroupsMap.end())
            {
                node->setIsFinal(true);
                finalNodesMap[node] = finalGroupsMap[group];
            }
            for (auto const &stateTransition : stateTransitions)
            {
                int nextGroup = statesGroupMap[stateTransition.second];
                Node *nextNode;
                if (groupsNodeMap.find(nextGroup) == groupsNodeMap.end())
                {
                    nextNode = new Node();
                    groupsNodeMap[nextGroup] = nextNode;
                }
                nextNode = groupsNodeMap[nextGroup];
                node->addNextNode(nextNode, stateTransition.first);
            }
            finishedGroups.insert(group);
        }
    }
    dfaAutomaton.setStartNode(groupsNodeMap[startGroup]);
    dfaAutomaton.setFinalNodesMap(finalNodesMap);
    dfaAutomaton.setPriorityMap(regularExpressionsPriorityMap);
    return dfaAutomaton;
}

CombinedAutomaton NFADFAConverter::convertNFAToDFA(CombinedAutomaton nfa, unordered_map<string, int> &regularExpressionsPriorityMap)
{
    unordered_set<char> allPossibleInputs = findAllPossibleInputs(nfa.getStartNode());
    allPossibleInputs.erase((char)238);
    pair<unordered_map<int, unordered_map<char, int>>, unordered_map<int, string>> dfaInfo = findDFAInfo(nfa, regularExpressionsPriorityMap, allPossibleInputs);
    unordered_map<int, unordered_map<char, int>> dfaTransitionsMap = dfaInfo.first;
    unordered_map<int, string> dfaFinalStatesMap = dfaInfo.second;
    vector<int> allDFAStates;
    for (auto const &dfaTransition : dfaTransitionsMap)
        allDFAStates.push_back(dfaTransition.first);
    pair<vector<vector<int>>, unordered_map<int, int>> dfaInitialGroupsInfo = findDFAInitialGroupsInfo(allDFAStates, dfaFinalStatesMap);
    unordered_map<int, int> statesGroupMap = minimizeDFAStates(dfaInitialGroupsInfo.second, dfaInitialGroupsInfo.first, dfaTransitionsMap, allPossibleInputs);
    return constructDFA(statesGroupMap, dfaTransitionsMap, dfaFinalStatesMap, regularExpressionsPriorityMap);
}