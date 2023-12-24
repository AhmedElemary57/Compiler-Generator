//
// Created by USER on 12/24/2023.
//

#include "Table.h"

Table::Table(unordered_map<string, NonTerminal *> nonTerminals,
             vector<string> nonTerminalsNames) {
    fillTable(nonTerminals, nonTerminalsNames);
}

// search in a set of CFGEntry for a certain terminal
bool searchInSet(set<Terminal*> set, string terminalName) {
    for (auto it : set) {
        if (!(it->getName() == terminalName)) continue;
        return true;
    }
    return false;
}

// function to check wither a terminal is in first set of one non terminal productions
vector<CFGEntry*> Table::fillRowOfNonTerminal(NonTerminal* nonTerminal) {
    //TODO: optimize this function an use the set itself for searching
    vector<vector<CFGEntry*>> productions = nonTerminal->getProductions();
    vector<CFGEntry*> result;
    set<Terminal*> followSet = nonTerminal->getFollowSet();

    for (int i = 0; i < productions.size(); ++i) {
        // get first set of certain production
        set<Terminal*> firstSet = nonTerminal->getFirstSet(i);
        // check if the terminal is in the first set of the production
        //For each terminal 'a' in First(α), add A → α to M[A, a]
        for( auto terminal : firstSet){
            table[nonTerminal->getName()][terminal->getName()] = productions[i];
        }
        //  If ε is in First(α) and $ is in Follow(A), add A → α to M[A, $]
        if(searchInSet(firstSet, "\\L") && searchInSet(followSet, "$")){
            table[nonTerminal->getName()]["$"] = productions[i];
        }
        // If ε is in First(α): For each terminal 'b' in Follow(A), add A → α to M[A, b].
        if(searchInSet(firstSet, "\\L")){
            for (auto it : followSet) {
                table[nonTerminal->getName()][it->getName()] = productions[i];
            }
        }
    }
    return result;
}
/***
 For each production A → α:
    For each terminal 'a' in First(α), add A → α to M[A, a].
    If ε is in First(α):
    For each terminal 'b' in Follow(A), add A → α to M[A, b].
    If ε is in First(α) and $ is in Follow(A), add A → α to M[A, $].
 * **/
void Table::fillTable(unordered_map<string, NonTerminal *> nonTerminals,
                      vector<string> nonTerminalsNames) {
    calculateFirstToCFG(make_pair(nonTerminalsNames, nonTerminals));

    for (int i = 0; i < nonTerminalsNames.size(); ++i) {
        // get first set of the non terminal
        NonTerminal* nonTerminal = nonTerminals[nonTerminalsNames[i]];
        fillRowOfNonTerminal(nonTerminal);
    }
}

void Table::printTable() {
    for (auto it : table) {
        cout << it.first << " : ";
        for (auto it2 : it.second) {
            cout << it2.first << " : ";
            for (auto it3 : it2.second) {
                cout << it3->getName() << " ";
            }
            cout << " | ";
        }
        cout << endl;
    }
}
