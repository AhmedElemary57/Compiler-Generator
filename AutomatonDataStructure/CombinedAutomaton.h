#ifndef COMPILERGENERATOR_COMBINEDAUTOMATON_H
#define COMPILERGENERATOR_COMBINEDAUTOMATON_H

#include "Node.h"

class CombinedAutomaton
{
private:
    Node* startNode;
    unordered_map<Node *, string> finalNodesMap;

public:
    CombinedAutomaton();
    CombinedAutomaton(Node* startNode, unordered_map<Node *, string> finalNodesMap);
    void setStartNode(Node* startNode);
    void setFinalNodesMap(unordered_map<Node *, string> finalNodesMap);
    Node* getStartNode();
    unordered_map<Node *, string> getFinalNodesMap();
};

#endif // COMPILERGENERATOR_COMBINEDAUTOMATON_H