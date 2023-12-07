#include "Node.h"

using namespace std;

Node::Node() {
    this->nextNodes = {};
    this->isFinal = false;
    this->nodeNumber = nodeCounter++;
}

// Deep copy constructor
Node::Node(Node& other, unordered_map<Node*, Node*> &unique_next)  {
    // Deep copy the nextNodes map
    nodeNumber = nodeCounter++;
    unique_next[&other] = this;
    for (const auto& entry : other.nextNodes) { // entry is a pair of (char, vector<Node*>)
        char input = entry.first;
        const auto& nextNodesForInput = entry.second; // nextNodesForInput is a vector<Node*>

        for (const auto& nextNode : nextNodesForInput) {
            // Recursively deep copy each nextNode
            if(unique_next.find(nextNode) == unique_next.end()) {
                Node *copiedNextNode = new Node(*nextNode, unique_next);
                addNextNode(copiedNextNode, input);
            }
            else {
                Node *copiedNextNode = unique_next.at(nextNode);
                addNextNode(copiedNextNode, input);
            }
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

void Node::setNextNodes(unordered_map<char,vector<Node*>> nextNodes){
    this->nextNodes = nextNodes;
}

void Node::setIsFinal(bool isFinal) {

    this->isFinal = isFinal;
}

void Node::printNode() {
    cout << "Node " << nodeNumber << " - Is Final: " << (isFinal ? "Yes" : "No") << endl;

    for (const auto& entry : nextNodes) {
        char input = entry.first;
        const auto& nextNodesForInput = entry.second;

        cout << "  Input '" << input << "': ";

        for (const auto& nextNode : nextNodesForInput) {
            cout << nextNode->nodeNumber << " ";
        }

        cout << endl;
    }
    cout << endl;
}

