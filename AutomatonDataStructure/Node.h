#ifndef COMPILERGENERATOR_NODE_H
#define COMPILERGENERATOR_NODE_H

#include <bits/stdc++.h>
#include <memory>
#include <string>
#include <vector>
#include <set>

using namespace std;

class Node
{
private:
    unordered_map<char, vector<Node *>> nextNodes;
    bool isFinal;
    static int nodeCounter;
    int nodeNumber;

public:
    Node();
    Node(const Node &other, unordered_map<char, Node *> nextNodes);
    Node(unordered_map<char, vector<Node *>> nextNodes, bool isFinal);
    void addNextNode(Node *nextNode, char input);
    unordered_map<char, vector<Node *>> getNextNodes();
    vector<Node *> getNextNodes(char input);

    void setIsFinal(bool isFinal);
    void printNode();
    int getNodeNumber() const
    {
        return nodeNumber;
    }
};

#endif // COMPILERGENERATOR_NODE_H