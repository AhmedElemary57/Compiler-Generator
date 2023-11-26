//
// Created by USER on 11/26/2023.
//

#include "Automaton.h"

using namespace std;

Automaton::Automaton() {
    this->startNode = nullptr;
    this->finalNode = nullptr;
}

Automaton::Automaton(Node *startNode, Node *finalNode) {
    this->startNode = startNode;
    this->finalNode = finalNode;
}

void Automaton::setStartNode(Node *startNode) {
    this->startNode = startNode;
}

void Automaton::setFinalNode(Node *finalNode) {
    this->finalNode = finalNode;
}

Node *Automaton::getStartNode() {
    return this->startNode;
}

Node *Automaton::getFinalNode() {
    return this->finalNode;
}

void Automaton::printAutomaton(){
    cout << "Automaton: " << this << endl;
    cout << "Start node: " << this->startNode << endl;
    cout << "Final node: " << this->finalNode << endl;
    cout << "Start node: " << endl;
    this->startNode->printNode(); // print the start node of the automaton (which will print all inputs and next nodes)

}

