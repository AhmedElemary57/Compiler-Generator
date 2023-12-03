//
// Created by elsaber on 28/11/23.
//
#ifndef COMPILERGENERATOR_POSTFIX_EXPRESSION_H
#define COMPILERGENERATOR_POSTFIX_EXPRESSION_H

#include "../AutomatonDataStructure/Automaton.h"
#include "../AutomatonDataStructure/automatonGenerator.h"
#include "../ParsingInputFile/parser.h"

Automaton postfix(string expression, unordered_map<string, Automaton> automatons);
string prepareCharacters(string str, unordered_map<string, Automaton> &automatons);
bool is_special(char c);

#endif //COMPILERGENERATOR_POSTFIX_EXPRESSION_H
