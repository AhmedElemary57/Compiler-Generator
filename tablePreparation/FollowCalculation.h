//
// Created by USER on 12/23/2023.
//

#ifndef COMPILERGENERATOR_FOLLOWCALCULATION_H
#define COMPILERGENERATOR_FOLLOWCALCULATION_H

#include <string>
#include <vector>
#include "../CFGParser/CFG.h"
using namespace std;

void calculateFollowToNonTerminal(const vector<string>& nonTerminalsNames,
                                  const string& nonTerminalName,
                                  unordered_map<string, NonTerminal*> nonTerminals,
                                  unordered_map<string, bool>& computed);
void calculateFollowToNonTerminals(const vector<string>& nonTerminalsNames,
                                   unordered_map<string, NonTerminal*> nonTerminals);
#endif //COMPILERGENERATOR_FOLLOWCALCULATION_H
