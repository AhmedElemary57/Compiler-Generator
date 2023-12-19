#include <bits/stdc++.h>

using namespace std;

class CFGEntry
{
private:
    string name;
    // vector<CFGEntry *> firstSet;             should this be a vector or other data structure?
    // vector<CFGEntry *> followSet;            should this be a vector or other data structure?

public:
    CFGEntry(string name)
    {
        this->name = name;
    }

    string getName()
    {
        return this->name;
    }

    virtual bool isTerminal();
};

class Terminal : public CFGEntry
{
public:
    Terminal(string name) : CFGEntry(name) {}

    bool isTerminal() override
    {
        return true;
    }
};

class NonTerminal : public CFGEntry
{
private:
    vector<vector<CFGEntry *>> productions;

public:
    bool hasEpsilonProduction;

    NonTerminal(string name) : CFGEntry(name) {}

    bool isTerminal() override
    {
        return false;
    }
};

class CFG
{
private:
    vector<string> nonTerminalsNames;
    unordered_map<string, NonTerminal *> namesNonTerminalsMap;

public:
    CFG(vector<string> &nonTerminalsNames, unordered_map<string, NonTerminal *> namesNonTerminalsMap)
    {
        this->nonTerminalsNames = nonTerminalsNames;
        this->namesNonTerminalsMap = namesNonTerminalsMap;
    }
};