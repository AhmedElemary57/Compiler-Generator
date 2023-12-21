//
// Created by elsaber on 21/12/23.
//

#include "FirstCalculation.h"
void calculateFirstToCFG(pair<vector<string>, unordered_map<string, NonTerminal *>> nonTerminals) {

    unordered_map<string, bool> visited;
    for (auto &nonTerminalName: nonTerminals.first) {
        if (nonTerminals.second[nonTerminalName]->allFirstComputed()) {
            continue;
        }

        calculateFirstToNonTerminal(nonTerminals.second[nonTerminalName], visited);
    }
}

void calculateFirstToNonTerminal(NonTerminal *nonTerminal, unordered_map<string, bool> visited) {
    if(visited[nonTerminal->getName()] && !nonTerminal->allFirstComputed()){
        cerr << "Left Recursion Detected" << endl;
        exit(1);
    }
    if(nonTerminal->allFirstComputed()){
        return;
    }
    visited[nonTerminal->getName()] = true;
    for (auto &production : nonTerminal->getProductions()) {
        set<Terminal*> firstSet;
        for (int i = 0; i < production.size(); i++) {
            auto &entry = production[i];
            if(entry->isTerminal()){
                firstSet.insert((Terminal*)entry);
                break;
            }
            else{
                set<Terminal*> firstSetOfEntry = ((NonTerminal*)entry)->getAllFirstSet();
                if(firstSetOfEntry.empty()){
                    calculateFirstToNonTerminal((NonTerminal*)entry, visited);
                    firstSetOfEntry = ((NonTerminal*)entry)->getAllFirstSet();
                }

                firstSet.insert(firstSetOfEntry.begin(), firstSetOfEntry.end());
                if(!((NonTerminal*)entry)->hasEpsilon()){
                    break;
                }
                if(i == production.size() - 1){
                    nonTerminal->setHasEpsilonProductionInFirst();
                }
            }
        }
        nonTerminal->addSetToFirst(firstSet);
    }
}
