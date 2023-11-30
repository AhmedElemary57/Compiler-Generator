#include "NFADFAConverter.h"

vector<Node *> NFADFAConverter::EClosure(vector<Node *> nodes)
{
    stack<Node *> stack;
    vector<Node *> eClosuere = vector<Node *>();
    unordered_set<int> takenNodeNumbers = unordered_set<int>();
    for (Node *node : nodes)
    {
        stack.push(node);
        eClosuere.push_back(node);
        takenNodeNumbers.insert(node->getNodeNumber());
    }
    while (!stack.empty())
    {
        Node *node = stack.top();
        stack.pop();
        for (Node *nextNode : node->getNextNodes((char)238))
            if (takenNodeNumbers.find(nextNode->getNodeNumber()) != takenNodeNumbers.end())
            {
                stack.push(nextNode);
                eClosuere.push_back(nextNode);
                takenNodeNumbers.insert(nextNode->getNodeNumber());
            }
    }
    return eClosuere;
}