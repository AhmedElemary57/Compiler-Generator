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
    vector<set<Terminal*>> firstOfProductions;
    bool hasEpsilonProduction;
    bool hasEpsilonProductionInFirst;
public:

    NonTerminal(string name);
    void addProduction(vector<CFGEntry *> production);
    bool isTerminal() override;
    vector<vector<CFGEntry*>> getProductions();

    bool hasEpsilon()
    {
        return hasEpsilonProduction;
    }

    void addSetToFirst(set<Terminal*> firstSet)
    {
        firstOfProductions.push_back(firstSet);
    }
    void setHasEpsilonProduction(bool hasEpsilonProduction)
    {
        this->hasEpsilonProduction = hasEpsilonProduction;
        hasEpsilonProductionInFirst = hasEpsilonProduction;
    }

    void setHasEpsilonProductionInFirst()
    {
        this->hasEpsilonProductionInFirst = true;
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

    void setProductions(vector<vector<CFGEntry *>> productions);



};

class CFG
{
private:
    vector<string> nonTerminalsNames;
    unordered_map<string, NonTerminal *> namesNonTerminalsMap;

public:
    CFG(vector<string> &nonTerminalsNames, unordered_map<string, NonTerminal *> namesNonTerminalsMap);

    string get_unique_non_terminal_name(string name);

    vector<vector<CFGEntry *>> build_string_from_production(vector<vector<CFGEntry *>> prod, vector<int> indices);

    void non_immediate_left_recursion_elimination(int i, int j);

    void immediate_left_recursion_elimination(int i);

    void left_recursion_elimination();

    void left_factor_non_terminal(NonTerminal *A, string name);

    void left_factoring();

    void print_productions();
};
#endif // CFG_H