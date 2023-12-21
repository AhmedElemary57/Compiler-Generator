#include "CFG.h"

CFGEntry::CFGEntry(string name)
{
    this->name = name;
}

string CFGEntry::getName()
{
    return this->name;
}

Terminal::Terminal(string name) : CFGEntry(name) {}

bool Terminal::isTerminal()
{
    return true;
}

NonTerminal::NonTerminal(string name) : CFGEntry(name) {}

bool NonTerminal::isTerminal()
{
    return false;
}

void NonTerminal::addProduction(vector<CFGEntry *> production)
{
    this->productions.push_back(production);
}

CFG::CFG(vector<string> &nonTerminalsNames, unordered_map<string, NonTerminal *> namesNonTerminalsMap)
{
    this->nonTerminalsNames = nonTerminalsNames;
    this->namesNonTerminalsMap = namesNonTerminalsMap;
}