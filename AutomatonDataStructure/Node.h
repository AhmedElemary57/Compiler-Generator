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

    public:
        Node();
        Node(unordered_map<char,vector<Node*> > nextNodes,bool isFinal);
        void addNextNode(Node* nextNode,char input);
        unordered_map<char,vector<Node*> > getNextNodes();
        vector<Node* > getNextNodes(char input);

        void setIsFinal(bool isFinal);
        void printNode();


};


#endif //COMPILERGENERATOR_NODE_H
