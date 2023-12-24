#include "FollowCalculation.h"
#include <bits/stdc++.h>
#include "FirstCalculation.h"

// Function to initialize the follow set for the start symbol
void initializeFollowSetForStartSymbol(const vector<string>& nonTerminalsNames,
                                       unordered_map<string, NonTerminal*>& nonTerminals) {
    // Rule 1: FOLLOW(S) = {$}
    nonTerminals[nonTerminalsNames[0]]->addTerminalToFollowSet(new Terminal("$"));
}

void updateFollowSetForLastEntry(const string &nonTerminalName, NonTerminal *currentNonTerminal,
                                 const vector<string> &nonTerminalsNames,
                                 unordered_map<string, NonTerminal *> &nonTerminals, unordered_map<string, bool> &computed) {
    set<Terminal*> nonTerminalsInProduction = currentNonTerminal->getFollowSet();

    // If follow set is empty, calculate it
    if (nonTerminalsInProduction.empty()) {
        calculateFollowToNonTerminal(nonTerminalsNames, currentNonTerminal->getName(), nonTerminals, computed);
        nonTerminalsInProduction = currentNonTerminal->getFollowSet();
        computed[currentNonTerminal->getName()] = true;
    }
    nonTerminals[nonTerminalName]->addSetToFollowSet(nonTerminalsInProduction);
}



void updateFollowSetForNonTerminalInProduction(NonTerminal *currentNonTerminal, const vector<string> &nonTerminalsNames,
                                               const string &nonTerminalName, const vector<CFGEntry *> &production,
                                               int currentIndex, unordered_map<string, NonTerminal *> &nonTerminals,
                                               unordered_map<string, bool> &computed) {

    if (currentIndex == production.size() - 1) {
        // Rule 3: A → αB
        if (nonTerminalName == production[currentIndex]->getName() && nonTerminalName != currentNonTerminal->getName()) {
            updateFollowSetForLastEntry(nonTerminalName, currentNonTerminal, nonTerminalsNames, nonTerminals, computed);
        }
    } else if (nonTerminalName == production[currentIndex]->getName()) {
        if (production[currentIndex + 1]-> isTerminal()) {
            // Rule 2: A → αBβ, where B is terminal
            nonTerminals[nonTerminalName]->addTerminalToFollowSet((Terminal*)production[currentIndex + 1]);
        } else {
//            set<Terminal*> firstSetOfNextEntry = ((NonTerminal*)production[currentIndex + 1])->getAllFirstSet();
            NonTerminal nextNonTerminal = *nonTerminals[production[currentIndex + 1]->getName()];
            set<Terminal*> firstSetOfNextEntry = nextNonTerminal.getAllFirstSet();
            // Rule 4: A → αBβ and ε belongs to FIRST(β)
            bool hasEpsilonInFirstSet = false;
            vector<bool> next= nextNonTerminal.getHasEpsilonProductionInFirst();

            if(nextNonTerminal.hasEpsilon()){
                hasEpsilonInFirstSet = true;
            }
            for(int i = 0; i < next.size(); i++){
                if(next[i]){
                    hasEpsilonInFirstSet = true;
                    break;
                }
            }

//            if (checkForEpsilonInFirstSet(firstSetOfNextEntry)) {
            if (hasEpsilonInFirstSet){
                updateFollowSetForNonTerminalInProduction(currentNonTerminal, nonTerminalsNames, nonTerminalName,
                                                          production, currentIndex + 1, nonTerminals, computed);

                set<Terminal*> followSetOfNextEntry = nonTerminals[production[currentIndex + 1]->getName()]->getFollowSet();

                // If follow set is empty, calculate it
                if (followSetOfNextEntry.empty()) {
                    calculateFollowToNonTerminal(nonTerminalsNames, production[currentIndex + 1]->getName(), nonTerminals, computed);
                    followSetOfNextEntry = nonTerminals[production[currentIndex + 1]->getName()]->getFollowSet();
                    computed[production[currentIndex + 1]->getName()] = true;
                }

                nonTerminals[nonTerminalName]->addSetToFollowSet(followSetOfNextEntry);
            }

            // Rule 2: A → αBβ, where B is non-terminal
            nonTerminals[nonTerminalName]->addSetToFollowSet(firstSetOfNextEntry);

        }
    }
}

// Calculate follow set for a specific non-terminal
void calculateFollowToNonTerminal(const vector<string>& nonTerminalsNames,
                                  const string& nonTerminalName,
                                  unordered_map<string, NonTerminal*> nonTerminals,
                                  unordered_map<string, bool>& computed) {
    for (auto& nonTerminal : nonTerminals) {
        // Get the production of the non-terminal
        vector<vector<CFGEntry*>> productions = nonTerminal.second->getProductions();
        for (auto& production : productions) {
            for (int i = 0; i < production.size(); i++) {
                auto& entry = production[i];
                if (!entry->isTerminal()) {
                    updateFollowSetForNonTerminalInProduction(nonTerminal.second, nonTerminalsNames, nonTerminalName, production, i, nonTerminals, computed);
                }
            }
        }
    }
}

// Calculate follow set for each non-terminal
void calculateFollowToNonTerminals(const vector<string>& nonTerminalsNames,
                                   unordered_map<string, NonTerminal*> nonTerminals) {
    initializeFollowSetForStartSymbol(nonTerminalsNames, nonTerminals);
    unordered_map<string, bool> computed;
    // Initialize the computed map
    for (auto& nonTerminalName : nonTerminalsNames) {
        computed[nonTerminalName] = false;
    }
    for (auto& nonTerminalName : nonTerminalsNames) {
        // If the follow set is already calculated
        if (computed[nonTerminalName]) {
            continue;
        }
        calculateFollowToNonTerminal(nonTerminalsNames, nonTerminalName, nonTerminals, computed);

    }
}
