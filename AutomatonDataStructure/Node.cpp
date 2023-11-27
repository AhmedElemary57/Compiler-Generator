#include "Node.h"

using namespace std;

Node::Node() {
    this->nextNodes = {};
    this->isFinal = false;
    this->nodeNumber = nodeCounter++;
}

// Deep copy constructor
Node::Node(const Node& other)  {
    // Deep copy the nextNodes map
    for (const auto& entry : other.nextNodes) { // entry is a pair of (char, vector<Node*>)
        char input = entry.first;
        const auto& nextNodesForInput = entry.second; // nextNodesForInput is a vector<Node*>

        for (const auto& nextNode : nextNodesForInput) {
            // Recursively deep copy each nextNode
            Node* copiedNextNode = new Node(*nextNode);
            addNextNode(copiedNextNode, input);
        }
    }

    // Set the isFinal flag
    isFinal = other.isFinal;
}

Node::Node(unordered_map<char, vector<Node *>> nextNodes, bool isFinal) {
    this->nextNodes = nextNodes;
    this->isFinal = isFinal;
    this->nodeNumber = nodeCounter++;
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
    cout << "Node: " << this->nodeNumber << endl;
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
