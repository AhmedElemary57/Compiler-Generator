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
    unordered_map<string, int> priorityMap;


public:
    CombinedAutomaton();
    CombinedAutomaton(Node *startNode, unordered_map<Node *, string> finalNodesMap, unordered_map<string, int> priorityMap);
    void setPriorityMap(unordered_map<string, int> &priorityMap);
    void setStartNode(Node* startNode);
    void setFinalNodesMap(unordered_map<Node *, string> finalNodesMap);
    Node* getStartNode();
    unordered_map<Node *, string> getFinalNodesMap();
    int getPriority(Node* node);
    string getTokenName(Node* node);
    int getPriority(string token);
    void generateCombinedAutomaton(unordered_map<string, Automaton> &automatonMap);
    void generateCombinedAutomaton(unordered_map<string, Automaton> &automatonMap, unordered_map<string, Automaton> &keywordsMap);
    void generateCombinedAutomaton(unordered_map<string, Automaton> &automatonMap, unordered_map<string, Automaton> &keywordsMap, Automaton &punctuationsAutomaton);

};



#endif //COMPILERGENERATOR_COMBINEDAUTOMATON_H
