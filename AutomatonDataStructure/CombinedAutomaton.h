//
// Created by USER on 12/2/2023.
//

#include "Node.h"
#include <bits/stdc++.h>    // for string, vector, set, and unordered_map
#include "Automaton.h"

#ifndef COMPILERGENERATOR_COMBINEDAUTOMATON_H
#define COMPILERGENERATOR_COMBINEDAUTOMATON_H


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
    void generateCombinedAutomaton(unordered_map<string, Automaton> &automatonMap);
};



#endif //COMPILERGENERATOR_COMBINEDAUTOMATON_H
