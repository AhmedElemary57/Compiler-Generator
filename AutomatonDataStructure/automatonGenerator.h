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


unordered_map<string, Automaton> generateAutomatonFromRegularDefinitions(const unordered_map<string, vector<string>>& regularDefinitionsMap);
void printAutomatonMap(const unordered_map<string, Automaton>& automatonMap);

#endif //COMPILERGENERATOR_AUTOMATONGENERATOR_H
