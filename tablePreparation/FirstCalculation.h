//
// Created by elsaber on 21/12/23.
//
#include <string>
#include <vector>
#include "../CFGParser/CFG.h"
using namespace std;
#ifndef COMPILERGENERATOR_FIRSTCALCULATION_H
#define COMPILERGENERATOR_FIRSTCALCULATION_H

void calculateFirstToCFG(CFG &cfg) ;
void calculateFirstToNonTerminal(NonTerminal* nonTerminal,  unordered_map<string, bool> visited);


#endif //COMPILERGENERATOR_FIRSTCALCULATION_H
