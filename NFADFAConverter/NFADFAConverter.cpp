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

unordered_set<Node *> move(unordered_set<Node *> sources, char input)
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

DFATransitions findDFATransitions(Node *startNode)
{
    unordered_set<unordered_set<Node *>, HashUnorderedSetNodePointers> allStates;
    queue<unordered_set<Node *>> unmarkedStates;
    DFATransitions statesTransitions;
    unordered_set<char> allPossibleInputs = findAllPossibleInputs(startNode);
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