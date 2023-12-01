//
// Created by USER on 11/26/2023.
//

#include "Automaton.h"

using namespace std;

// Default constructor
Automaton::Automaton() {
    this->startNode = nullptr;
    this->finalNode = nullptr;
}

// Deep copy constructor
Automaton::Automaton(const Automaton& other) noexcept {
    // Perform deep copy for startNode and finalNode
    unordered_map<int, Node*> unique_next;

    // Deep copy finalNode and startNode
    finalNode = new Node(*(other.finalNode), unique_next);
    unique_next[finalNode->getNodeNumber()] = finalNode;

    startNode = new Node(*(other.startNode), unique_next);
}

// Constructor with parameters
Automaton::Automaton(Node* startNode, Node* finalNode) {
    this->startNode = startNode;
    this->finalNode = finalNode;
}

// Function to set the startNode
void Automaton::setStartNode(Node* startNode) {
    this->startNode = startNode;
}

// Function to set the finalNode
void Automaton::setFinalNode(Node* finalNode) {
    this->finalNode = finalNode;
}

// Function to get the startNode
Node* Automaton::getStartNode() {
    return this->startNode;
}

// Function to get the finalNode
Node* Automaton::getFinalNode() {
    return this->finalNode;
}

// Function to print information about the automaton
void Automaton::printAutomaton() {
    cout << "Automaton Information:" << endl;

    if (startNode && finalNode) {
        cout << "Start Node: " << startNode->getNodeNumber() << endl;
        cout << "Final Node: " << finalNode->getNodeNumber() << endl;
    } else {
        cout << "Automaton is not properly initialized." << endl;
    }
    cout << endl;
}