//
// Created by USER on 12/2/2023.
//

#include "CombinedAutomaton.h"

CombinedAutomaton::CombinedAutomaton() {
    this->startNode = nullptr;
    this->finalNodesMap = unordered_map<Node *, string>();
}

CombinedAutomaton::CombinedAutomaton(Node *startNode, unordered_map<Node *, string> finalNodesMap) {
    this->startNode = startNode;
    this->finalNodesMap = finalNodesMap;
}

void CombinedAutomaton::setStartNode(Node *startNode) {
    this->startNode = startNode;
}

void CombinedAutomaton::setFinalNodesMap(unordered_map<Node *, string> finalNodesMap) {
    this->finalNodesMap = finalNodesMap;
}

Node *CombinedAutomaton::getStartNode() {
    return this->startNode;
}

unordered_map<Node *, string> CombinedAutomaton::getFinalNodesMap() {
    return this->finalNodesMap;
}

void CombinedAutomaton::generateCombinedAutomaton(unordered_map<string, Automaton> &automatonMap) {
    /***
     * This function generates the combined automaton from the automaton map
     * @param automatonMap: the map of the automata generated from the regular expressions
     * @return: the combined automaton
     */
    Node *tempStartNode = new Node();
    unordered_map<Node *, string> tempFinalNodesMap;
    for (auto &automaton : automatonMap) {
        tempStartNode->addNextNode(automaton.second.getStartNode(), char(238)); // epsilon input to go to each automaton
        tempFinalNodesMap[automaton.second.getFinalNode()] = automaton.first;
    }

    this->setStartNode(tempStartNode);
    this->setFinalNodesMap(tempFinalNodesMap);


}
