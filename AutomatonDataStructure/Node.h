#include <bits/stdc++.h>
#include <memory>
#include <string>
#include <vector>
#include <set>

using namespace std;

#ifndef COMPILERGENERATOR_NODE_H
#define COMPILERGENERATOR_NODE_H


class Node {
    private:
        unordered_map<char,vector<Node*>> nextNodes;
        bool isFinal;
        static int nodeCounter;
        int nodeNumber;

    public:
        Node();
        Node(Node& other, unordered_map<Node*, Node*> &nextNodes);
        Node(unordered_map<char,vector<Node*> > nextNodes,bool isFinal);
        void addNextNode(Node* nextNode,char input);
        unordered_map<char,vector<Node*> > getNextNodes();
        vector<Node* > getNextNodes(char input);

        void setNextNodes(unordered_map<char,vector<Node*>> nextNodes);
        void setIsFinal(bool isFinal);
        void printNode();
        int getNodeNumber() const {
        return nodeNumber;
        }
        bool getIsFinal() const {
            return isFinal;
        }

};

struct HashUnorderedSetNodePointers
{
    size_t operator()(const unordered_set<Node *> &unorderedSet) const
    {
        size_t ret = 0;
        for (Node *nodePointer : unorderedSet)
        {
            ret ^= hash<Node *>()(nodePointer);
        }
        return ret;
    }
};


#endif // COMPILERGENERATOR_NODE_H