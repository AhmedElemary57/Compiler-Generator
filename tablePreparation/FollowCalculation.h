//
// Created by USER on 12/23/2023.
//

#ifndef COMPILERGENERATOR_FOLLOWCALCULATION_H
#define COMPILERGENERATOR_FOLLOWCALCULATION_H

#include <string>
#include <vector>
#include "../CFGParser/CFG.h"
using namespace std;

void calculateFollowToNonTerminals(const vector<string>& nonTerminalsNames,
                                   unordered_map<string, NonTerminal*> nonTerminals);
void calculateFollowToNonTerminal(const vector<string>& nonTerminalsNames,
                                  const string& nonTerminalName,
                                  unordered_map<string, NonTerminal*> nonTerminals);
// Function to update follow set based on the last entry in a production
void updateFollowSetForLastEntry(const string& nonTerminalName,
                                 NonTerminal* currentNonTerminal,
                                 const vector<string>& nonTerminalsNames,
                                 unordered_map<string, NonTerminal*>& nonTerminals);
// Update follow set for non-terminal in production
void updateFollowSetForNonTerminalInProduction(NonTerminal* currentNonTerminal,
                                               const vector<string>& nonTerminalsNames,
                                               const string& nonTerminalName,
                                               const vector<CFGEntry*>& production,
                                               int currentIndex,
                                               unordered_map<string, NonTerminal*>& nonTerminals);
#endif //COMPILERGENERATOR_FOLLOWCALCULATION_H
