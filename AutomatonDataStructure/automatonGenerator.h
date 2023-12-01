#ifndef COMPILERGENERATOR_AUTOMATONGENERATOR_H
#define COMPILERGENERATOR_AUTOMATONGENERATOR_H

#include "../ParsingInputFile/parser.h"
#include "Automaton.h"

unordered_map<string, Automaton> generateAutomatonFromRegularDefinitions(unordered_map<string, vector<string>> &regularDefinitionsMap);
void printAutomatonMap(const unordered_map<string, Automaton> &automatonMap);
void handleRegularDefinitionsInTermsOfOtherRegularDefinitions(unordered_map<string, vector<string>> &regularDefinitionsMap, unordered_map<string, Automaton> &automatonMap);
Automaton plusOperation(Automaton automaton);
Automaton closure(const Automaton &automaton);

#endif // COMPILERGENERATOR_AUTOMATONGENERATOR_H