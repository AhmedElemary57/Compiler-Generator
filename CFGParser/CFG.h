#ifndef COMPILERGENERATOR_CFG_H
#define COMPILERGENERATOR_CFG_H

#include <bits/stdc++.h>

using namespace std;

class CFGEntry
{
private:
    string name;
    // vector<CFGEntry *> firstSet;             should this be a vector or other data structure?
    // vector<CFGEntry *> followSet;            should this be a vector or other data structure?

public:
    CFGEntry(string name);
    string getName();
    virtual bool isTerminal() = 0;
};

class Terminal : public CFGEntry
{
public:
    Terminal(string name);
    bool isTerminal() override;
};

class NonTerminal : public CFGEntry
{
private:
    vector<vector<CFGEntry *>> productions;

public:
    bool hasEpsilonProduction;
    NonTerminal(string name);
    bool isTerminal() override;
    void addProduction(vector<CFGEntry *> production);
};

class CFG
{
private:
    vector<string> nonTerminalsNames;
    unordered_map<string, NonTerminal *> namesNonTerminalsMap;

public:
    CFG(vector<string> &nonTerminalsNames, unordered_map<string, NonTerminal *> namesNonTerminalsMap);
};

#endif //COMPILERGENERATOR_CFG_H