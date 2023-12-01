//
// Created by USER on 11/26/2023.
//
#include <bits/stdc++.h>    // for string, vector, set, and unordered_map
#include <memory>           // for shared_ptr
#include "../ParsingInputFile/parser.h"
#include <iostream>
#include "Node.h"
#include "Automaton.h"

#ifndef COMPILERGENERATOR_AUTOMATONGENERATOR_H
#define COMPILERGENERATOR_AUTOMATONGENERATOR_H


unordered_map<string, Automaton> generateAutomatonFromRegularDefinitions(unordered_map<string, vector<string>> &regularDefinitionsMap);
void printAutomatonMap(const unordered_map<string, Automaton> &automatonMap);
void handleRegularDefinitionsInTermsOfOtherRegularDefinitions(unordered_map<string, vector<string>> &regularDefinitionsMap, unordered_map<string, Automaton> &automatonMap);
Automaton plusOperation(Automaton automaton);
Automaton closure(const Automaton& automaton);
Automaton unionOperation(Automaton automaton1, Automaton automaton2);
Automaton concatenation(Automaton automaton1, Automaton automaton2);

#endif //COMPILERGENERATOR_AUTOMATONGENERATOR_H

Automaton closure(const Automaton &automaton);

#endif // COMPILERGENERATOR_AUTOMATONGENERATOR_H