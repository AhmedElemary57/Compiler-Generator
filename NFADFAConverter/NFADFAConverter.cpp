#include "NFADFAConverter.h"

vector<Node *> NFADFAConverter::EClosure(vector<Node *> nodes)
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
    return vector<Node *>(eClosuere.begin(), eClosuere.end());
}