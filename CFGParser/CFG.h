#include <bits/stdc++.h>
// Example header guard in CFG.h
#ifndef CFG_H
#define CFG_H





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
    vector<set<Terminal*>> firstOfProductions;
    bool hasEpsilonProduction;
    bool hasEpsilonProductionInFirst;
public:

    NonTerminal(string name) : CFGEntry(name) {
        hasEpsilonProduction = false;
        hasEpsilonProductionInFirst = false;
    }
    void setHasEpsilonProduction()
    {
        this->hasEpsilonProduction = true;
        hasEpsilonProductionInFirst = true;
    }
    void addProduction(vector<CFGEntry *> production)
    {
        productions.push_back(production);
    }
    void setHasEpsilonProductionInFirst()
    {
        this->hasEpsilonProductionInFirst = true;
    }
    bool isTerminal() override
    {
        return false;
    }
    vector<vector<CFGEntry*>> getProductions()
    {
        return productions;
    }
    bool hasEpsilon()
    {
        return hasEpsilonProduction;
    }
    void addSetToFirst(set<Terminal*> firstSet)
    {
        firstOfProductions.push_back(firstSet);
    }
    set<Terminal*> getAllFirstSet()
    {
        set<Terminal*> first;
        for (const set<Terminal *>& firstProduction: this->firstOfProductions) {
            first.insert(firstProduction.begin(), firstProduction.end());
        }
        return first;
    }
    set<Terminal*> getFirstSet(int productionIndex)
    {
        return firstOfProductions[productionIndex];
    }

    bool allFirstComputed() {
        return firstOfProductions.size() == productions.size();
    }
    void printFirstSet()
    {
        cout << "First Set of " << this->getName() << " is: ";
        for (const auto& firstProduction: this->firstOfProductions) {
            cout << "{";
            for (const auto& firstEntry: firstProduction) {
                cout << firstEntry->getName() << ", ";
            }
            cout << "}, ";
        }
        cout << endl;
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
#endif // CFG_H