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
    set<Terminal*> followSet;
    bool hasEpsilonProduction;
    vector<bool> hasEpsilonProductionInFirst;
public:
    NonTerminal(string name);
    void addProduction(vector<CFGEntry *> production);
    bool isTerminal() override;
    vector<vector<CFGEntry*>> getProductions();
    vector<bool> getHasEpsilonProductionInFirst(){
        return hasEpsilonProductionInFirst;
    }
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
    }

    void addHasEpsilonProductionInFirst(bool hasEpsilonProductionInFirst)
    {
        this->hasEpsilonProductionInFirst.push_back(hasEpsilonProductionInFirst);
    }
    void changeHasEpsilonProductionInFirst(int index, bool hasEpsilonProductionInFirst)
    {
        this->hasEpsilonProductionInFirst[index] = hasEpsilonProductionInFirst;
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
    void printFollowSet()
    {
        cout << "Follow Set of " << this->getName() << " is: ";
        cout << "{";
        for (const auto& followEntry: this->followSet) {
            cout << followEntry->getName() << ", ";
        }
        cout << "}, ";
        cout << endl;
    }
    bool getHasEpsilonProductionInFirst(int i)
    {
        return hasEpsilonProductionInFirst[i];
    }
    void setProductions(vector<vector<CFGEntry *>> productions);
    void addTerminalToFollowSet(Terminal* terminal);
    void addSetToFollowSet(set<Terminal*> followSet);
    void setFollowSet(set<Terminal*> followSet);
    set<Terminal*> getFollowSet();

};

class CFG
{
private:
    vector<string> nonTerminalsNames;
    unordered_map<string, NonTerminal *> namesNonTerminalsMap;
    CFGEntry* create_entry(string name, bool terminal);
    vector<CFGEntry*> findLongestCommonPrefix(vector<vector<CFGEntry*>> productions);

public:
    CFG(vector<string> &nonTerminalsNames, unordered_map<string, NonTerminal *> namesNonTerminalsMap);

    string get_unique_non_terminal_name(string name);
    
    
    vector<vector<CFGEntry *>> build_string_from_production(vector<vector<CFGEntry *>> prod, vector<int> indices);

    // get non terminals names
    vector<string> get_non_terminals_names();

    // get non terminals map
    unordered_map<string, NonTerminal *> get_non_terminals_map();

    void non_immediate_left_recursion_elimination(int i, int j);

    void immediate_left_recursion_elimination(int i);

    void left_recursion_elimination();

    void left_factor_non_terminal(NonTerminal *A, string name);

    void left_factoring();

    void print_productions();

    unordered_map<string, NonTerminal *> get_names_non_terminals_map();
};
#endif // CFG_H