#include "NFADFAConverter.h"

// this constant represent the start of aliase numbers that will be asigned to the DFA states in subset construction,
// such taht the start state will have alias = DFAALIASSTART
// this constant should never be assigned a value less than one, since the value zero is reserved as an alis for the DFA phi state
#define DFAALIASSTART 1

// hash function for vector of int, to be used inside unordered_set, unordered_mao, etc.
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

// function to find elements that are common between two unordered_set of any template type
template <typename T>
unordered_set<T> unorderedSetsIntersection(const unordered_set<T> &first, const unordered_set<T> &second)
{
    unordered_set<T> result;
    for (T element : first)
        if (second.find(element) != second.end())
            result.insert(element);
    return result;
}

// function to get the first RE that has the highest periority inside the given vector of REs
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

// function to get the epsilon closure of a given set of nodes the result will include the given nodes themselves
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

// for each node in the given set, get the destination nodes under the transition of the given input and append them to the returned set
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

// function to find all possible inputs that all nodes of an automaton can accept (this function is needed to make this class more generic)
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
    // unordered_map<int, string>: the key is the DFA final state alias, and the value is the assigned RE for that final state
    unordered_map<int, string> finalStatesMap;
    unordered_set<Node *> finalNodes;
    for (auto const &finalNodesMapElement : finalNodesMap)
        finalNodes.insert(finalNodesMapElement.first);
    // for each set of nodes in the left most column of the subset construction table.
    for (auto const &dfaStatesNodesMapElement : dfaStatesNodesMap)
    {
        // if this set contains some final nodes of the original NFA
        unordered_set<Node *> finalNodesIntersection = unorderedSetsIntersection<Node *>(dfaStatesNodesMapElement.first, finalNodes);
        if (finalNodesIntersection.empty())
            continue;
        vector<string> regularExpressions;
        // determine which of these nodes' RE has the highest periority,
        // then put that set in the map of DFA final states and assign it the highest periority RE
        for (Node *finalNode : finalNodesIntersection)
            regularExpressions.push_back(finalNodesMap[finalNode]);
        finalStatesMap[dfaStatesNodesMapElement.second] = findHighestPeriorityRE(regularExpressions, regularExpressionsPriorityMap);
    }
    return finalStatesMap;
}

// this function build the subset construction table for the DFA
// it returns a pair such that is first element is the transitions map of the DFA states,
// and the second element is a map between the DFA final states and their RE
pair<unordered_map<int, unordered_map<char, int>>, unordered_map<int, string>> findDFAInfo(CombinedAutomaton nfa,
                                                                                           unordered_map<string, int> &regularExpressionsPriorityMap,
                                                                                           unordered_set<char> allPossibleInputs)
{
    int alias = DFAALIASSTART;
    // this map is used to hold the aliases of the DFA states, such that the key is an unordered set holding the corresponding set of NFA nodes
    unordered_map<unordered_set<Node *>, int, HashUnorderedSetNodePointers> dfaStatesSetAlias;
    unordered_map<int, unordered_set<Node *>> statesAliasMap;
    unordered_set<int> allStates;
    queue<int> unmarkedStates;
    // unordered_map<int, unordered_map<char, int>>: the key is DFA state alias, the value is another map between all possbiel input and their destinations
    unordered_map<int, unordered_map<char, int>> statesTransitions;
    unordered_set<Node *> nextStateNodes = findEClosure({nfa.getStartNode()});
    dfaStatesSetAlias[nextStateNodes] = alias;
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
            // if it is the first time of the destination set of nodes to appear, give an alias to it and add it to necessary data structures
            if (allStates.find(dfaStatesSetAlias[nextStateNodes]) == allStates.end())
            {
                dfaStatesSetAlias[nextStateNodes] = alias;
                statesAliasMap[alias] = nextStateNodes;
                allStates.insert(alias);
                unmarkedStates.push(alias);
                alias++;
            }
            // get the alias of the destination set of nodes and put it in the subset construction table of the DFA state
            transitions[input] = dfaStatesSetAlias[nextStateNodes];
        }
        statesTransitions.insert({currentState, transitions});
    }
    return {statesTransitions, determineFinalStatesMap(dfaStatesSetAlias, nfa.getFinalNodesMap(), regularExpressionsPriorityMap)};
}

// function to find the first groups that are used as the first step to minimize the DFA
// it returns a pair such that the first element is the first groups, and the second element is a map between the states and its group number
pair<vector<vector<int>>, unordered_map<int, int>> findDFAInitialGroupsInfo(vector<int> &allDFAStates,
                                                                            unordered_map<int, string> &dfaFinalStatesMap)
{
    unordered_map<int, int> statesGroupMap;
    // a map between each RE and all of its accepting states (each map element represents a group)
    unordered_map<string, vector<int>> reFinalStatesMap;
    vector<vector<int>> groups;
    vector<int> nonFinalStates;
    // determine all non final states and put them in the same group
    for (int &state : allDFAStates)
        if (dfaFinalStatesMap.find(state) == dfaFinalStatesMap.end())
            nonFinalStates.push_back(state);
    groups.push_back(nonFinalStates);
    for (auto const &element : dfaFinalStatesMap)
        if (reFinalStatesMap.find(element.second) == reFinalStatesMap.end())
            reFinalStatesMap[element.second] = vector<int>{element.first};
        else
            reFinalStatesMap[element.second].push_back(element.first);
    // for each RE put its acceptors in a separate group
    for (auto const &element : reFinalStatesMap)
        groups.push_back(element.second);
    // construct the map between each states and the group number it is assigned to
    for (int groupNumber = 1; groupNumber <= groups.size(); groupNumber++)
        for (int &state : groups[groupNumber - 1])
            statesGroupMap[state] = groupNumber;
    return {groups, statesGroupMap};
}

// function to minimize the DFA states and return a map between each DFA state and its group number
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

// function that consturcts and returns a combined automaton based on the given minimized DFA info
CombinedAutomaton constructDFA(unordered_map<int, int> &statesGroupMap,
                               unordered_map<int, unordered_map<char, int>> &dfaTransitionsMap,
                               unordered_map<int, string> &dfaFinalStatesMap,
                               unordered_map<string, int> &regularExpressionsPriorityMap)
{
    CombinedAutomaton dfaAutomaton;
    int startGroup = statesGroupMap[DFAALIASSTART];
    unordered_map<int, string> finalGroupsMap;
    // determine the RE of the final groups such that each group take the RE corresponding to the highest periority
    for (auto const &finalState : dfaFinalStatesMap)
    {
        int group = statesGroupMap[finalState.first];
        if (finalGroupsMap.find(group) == finalGroupsMap.end())
            finalGroupsMap[group] = finalState.second;
        else if (regularExpressionsPriorityMap[finalGroupsMap[group]] > regularExpressionsPriorityMap[finalState.second])
            finalGroupsMap[group] = finalState.second;
    }
    // map between each state alias and its corresponding Node object
    unordered_map<int, Node *> groupsNodeMap;
    unordered_set<int> finishedGroups;
    // map between each final Node objects and its RE
    unordered_map<Node *, string> finalNodesMap;
    for (auto const &element : dfaTransitionsMap)
    {
        int group = statesGroupMap[element.first];
        if (finishedGroups.find(group) == finishedGroups.end())
        {
            unordered_map<char, int> stateTransitions = element.second;
            Node *node;
            // if this group has no Node object, then create a new one for it
            if (groupsNodeMap.find(group) == groupsNodeMap.end())
            {
                node = new Node();
                groupsNodeMap[group] = node;
            }
            node = groupsNodeMap[group];
            // if this group is a final group then make its Node a final Node as well
            if (finalGroupsMap.find(group) != finalGroupsMap.end())
            {
                node->setIsFinal(true);
                finalNodesMap[node] = finalGroupsMap[group];
            }
            // for each possible transition of this group, at it to its corresponding Node object
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