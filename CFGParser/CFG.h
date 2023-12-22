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
    virtual bool isTerminal();
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
    vector<vector<CFGEntry *>> getProductions();
    void setProductions(vector<vector<CFGEntry *>> productions);
};

class CFG
{
private:
    vector<string> nonTerminalsNames;
    unordered_map<string, NonTerminal *> namesNonTerminalsMap;

    void non_immediate_left_recursion_elimination(int i, int j);
    void immediate_left_recursion_elimination(int i);
    void left_factor_non_terminal(NonTerminal *A, string name);
    vector<string> build_string_from_production(vector<vector<CFGEntry *>> prod, vector<int> indices);

public:
    CFG(vector<string> &nonTerminalsNames, unordered_map<string, NonTerminal *> namesNonTerminalsMap);
    void left_recursion_elimination();
    void left_factoring();
    void print_productions();
};

#endif //COMPILERGENERATOR_CFG_H