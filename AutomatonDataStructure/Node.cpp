//
// Created by USER on 11/26/2023.
//

#include "Node.h"

using namespace std;

Node::Node() {
    this->nextNodes = {};
    this->isFinal = false;
}

Node::Node(unordered_map<char, vector<Node *>> nextNodes, bool isFinal) {
    this->nextNodes = nextNodes;
    this->isFinal = isFinal;
}

void Node::addNextNode(Node *nextNode, char input) {
    // check if the input is already in the map
    if (this->nextNodes.find(input) == this->nextNodes.end()) {
        this->nextNodes[input] = {};
    }
    this->nextNodes[input].push_back(nextNode);
}

unordered_map<char, vector<Node *>> Node::getNextNodes() {
    return this->nextNodes;
}

vector<Node *> Node::getNextNodes(char input) {
    return this->nextNodes[input];
}

void Node::setIsFinal(bool isFinal) {
    this->isFinal = isFinal;
}

void Node::printNode() {
    cout << "Node: " << this << endl;
    cout << "Is final: " << this->isFinal << endl;
    cout << "Next nodes: " << endl;
    for (const auto &nextNode : this->nextNodes) {
        cout << "Input: " << nextNode.first << endl;
        cout << "Nodes: " << endl;
        for (const auto &node : nextNode.second) {
            cout << node << endl;
        }
    }
}

