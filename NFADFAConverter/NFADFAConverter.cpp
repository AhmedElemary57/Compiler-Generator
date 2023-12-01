#include "NFADFAConverter.h"

typedef unordered_map<unordered_set<Node *>, unordered_map<char, unordered_set<Node *>>, HashUnorderedSetNodePointers> DFATransitions;

unordered_set<Node *> eClosure(unordered_set<Node *> nodes)
{
    stack<Node *> stack;
    unordered_set<Node *> eClosuere;
    for (Node *node : nodes)
    {
        stack.push(node);
        eClosuere.insert(node);
    }
    while (!stack.empty())
    {
        Node *node = stack.top();
        stack.pop();
        for (Node *nextNode : node->getNextNodes((char)238))
            if (eClosuere.find(nextNode) == eClosuere.end())
            {
                stack.push(nextNode);
                eClosuere.insert(nextNode);
            }
    }
    return eClosuere;
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

DFATransitions findDFATransitions(Node *startNode, unordered_set<char> allPossibleInputs)
{
    unordered_set<unordered_set<Node *>, HashUnorderedSetNodePointers> allStates;
    queue<unordered_set<Node *>> unmarkedStates;
    DFATransitions statesTransitions;
    unordered_set<Node *> nextState = eClosure({startNode});
    allStates.insert(nextState);
    unmarkedStates.push(nextState);
    while (!unmarkedStates.empty())
    {
        unordered_set<Node *> currentState = unmarkedStates.front();
        unmarkedStates.pop();
        unordered_map<char, unordered_set<Node *>> transitions;
        for (const char input : allPossibleInputs)
        {
            nextState = eClosure(move(currentState, input));
            if (allStates.find(nextState) == allStates.end())
            {
                allStates.insert(nextState);
                unmarkedStates.push(nextState);
            }
            transitions[input] = nextState;
        }
        statesTransitions.insert({currentState, transitions});
    }
    return statesTransitions;
}

template <typename T>
unordered_set<T> unorderedSetsIntersection(unordered_set<T> &first, unordered_set<T> &second)
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

unordered_map<unordered_set<Node *>, string, HashUnorderedSetNodePointers> determineFinalStatesMap(
    vector<unordered_set<Node *>> &dfaStates,
    unordered_map<Node *, string> finalNodesMap,
    unordered_map<string, int> &regularExpressionsPriorityMap)
{
    unordered_map<unordered_set<Node *>, string, HashUnorderedSetNodePointers> finalStatesMap;
    unordered_set<Node *> finalNodes;
    for (auto const &finalNodesMapElement : finalNodesMap)
        finalNodes.insert(finalNodesMapElement.first);
    for (unordered_set<Node *> dfaState : dfaStates)
    {
        unordered_set<Node *> finalNodesIntersection = unorderedSetsIntersection<Node *>(dfaState, finalNodes);
        if (finalNodesIntersection.empty())
            continue;
        vector<string> regularExpressions;
        for (Node *finalNode : finalNodesIntersection)
            regularExpressions.push_back(finalNodesMap[finalNode]);
        finalStatesMap[dfaState] = findHighestPeriorityRE(regularExpressions, regularExpressionsPriorityMap);
    }
    return finalStatesMap;
}

CombinedAutomaton NFADFAConverter::convertNFAToDFA(CombinedAutomaton nfa, unordered_map<string, int> &regularExpressionsPriorityMap)
{
    Node *nfaStartNode = nfa.getStartNode();
    unordered_set<char> allPossibleInputs = findAllPossibleInputs(nfaStartNode);
    DFATransitions dfaTransitions = findDFATransitions(nfaStartNode, allPossibleInputs);
    vector<unordered_set<Node *>> dfaStates;
    for (auto const &dfaTransition : dfaTransitions)
        dfaStates.push_back(dfaTransition.first);
    unordered_map<unordered_set<Node *>, string, HashUnorderedSetNodePointers> finalStatesMap = determineFinalStatesMap(
        dfaStates, nfa.getFinalNodesMap(), regularExpressionsPriorityMap);
}