#ifndef COMPILERGENERATOR_AUTOMATON_H
#define COMPILERGENERATOR_AUTOMATON_H

#include <bits/stdc++.h>
#include <memory>
#include "Node.h"

class Automaton
{
private:
    Node *startNode;
    Node *finalNode;

public:
    Automaton();
    Automaton(const Automaton &other) noexcept;
    Automaton(Node *startNode, Node *finalNode);
    void setStartNode(Node *startNode);
    void setFinalNode(Node *finalNode);
    Node *getStartNode();
    Node *getFinalNode();
    void printAutomaton();
};

#endif // COMPILERGENERATOR_AUTOMATON_H