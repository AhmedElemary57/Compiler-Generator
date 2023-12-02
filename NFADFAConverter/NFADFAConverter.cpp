#include "NFADFAConverter.h"

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
        if (priority > highestPeriority)
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
                    nodesQueue.push(destinationNode);
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
    int alias = 1;
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

vector<unordered_set<int>> minimizeDFAStates(vector<unordered_set<int>> &statesGroups,
                                             unordered_map<int, unordered_map<char, int>> &dfaTransitionsMap,
                                             unordered_set<char> allPossibleInputs)
{
    // TODO
    return vector<unordered_set<int>>();
}

CombinedAutomaton NFADFAConverter::convertNFAToDFA(CombinedAutomaton nfa, unordered_map<string, int> &regularExpressionsPriorityMap)
{
    unordered_set<char> allPossibleInputs = findAllPossibleInputs(nfa.getStartNode());
    pair<unordered_map<int, unordered_map<char, int>>, unordered_map<int, string>> dfaInfo = findDFAInfo(nfa, regularExpressionsPriorityMap, allPossibleInputs);
    unordered_map<int, unordered_map<char, int>> dfaTransitionsMap = dfaInfo.first;
    unordered_map<int, string> dfaFinalStatesMap = dfaInfo.second;
    unordered_set<int> dfaFinalStates;
    unordered_set<int> dfaNonFinalStates;
    for (auto const &dfaFinalStatesMapElement : dfaFinalStatesMap)
        dfaFinalStates.insert(dfaFinalStatesMapElement.first);
    for (auto const &dfaTransitionsMapElement : dfaTransitionsMap)
        if (dfaFinalStates.find(dfaTransitionsMapElement.first) == dfaFinalStates.end())
            dfaNonFinalStates.insert(dfaTransitionsMapElement.first);
    vector<unordered_set<int>> statesGroups = vector<unordered_set<int>>{dfaFinalStates, dfaNonFinalStates};
    statesGroups = minimizeDFAStates(statesGroups, dfaTransitionsMap, allPossibleInputs);
}