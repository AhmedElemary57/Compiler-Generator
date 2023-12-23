//
// Created by elsaber on 21/12/23.
//

#include "FirstCalculation.h"
void calculateFirstToCFG(CFG &cfg) {

    unordered_map<string, bool> visited;
    vector<string> nonTerminals = cfg.get_non_terminals_names();
    unordered_map<string, NonTerminal *> namesNonTerminalsMap = cfg.get_names_non_terminals_map();
    for (auto &nonTerminalName: nonTerminals) {
        visited[nonTerminalName] = false;
    }
    for (auto &nonTerminalName: nonTerminals) {
        if (namesNonTerminalsMap[nonTerminalName]->allFirstComputed()) {
            continue;
        }
        cout<<nonTerminalName<<endl;
        calculateFirstToNonTerminal(namesNonTerminalsMap[nonTerminalName], visited, namesNonTerminalsMap);
    }
}

void calculateFirstToNonTerminal(NonTerminal *nonTerminal, unordered_map<string, bool> visited, unordered_map<string, NonTerminal *> namesNonTerminalsMap) {
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
                NonTerminal *curNonTerminal = namesNonTerminalsMap[entry->getName()];
                set<Terminal*> firstSetOfEntry = curNonTerminal->getAllFirstSet();
                if(!curNonTerminal->allFirstComputed()){
                    calculateFirstToNonTerminal(curNonTerminal, visited, namesNonTerminalsMap);
                    firstSetOfEntry = curNonTerminal->getAllFirstSet();
                }

                firstSet.insert(firstSetOfEntry.begin(), firstSetOfEntry.end());
                if(!curNonTerminal->hasEpsilon()){
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
