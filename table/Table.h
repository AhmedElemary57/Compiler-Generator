#include "../CFGParser/CFG.h"
#include "../tablePreparation/FirstCalculation.h"
#include "../tablePreparation/FollowCalculation.h"
#ifndef COMPILERGENERATOR_TABLE_H
#define COMPILERGENERATOR_TABLE_H

enum return_t {Production ,Epsilon ,Error, Sync, nonTerminalError};

class Table {

    public:
        unordered_map<string, unordered_map<string,pair<return_t,vector<CFGEntry*>>>> table; // <nonTerminal, <terminal, production>>
        Table(CFG &cfg);\
        void printTable();
        void printTableToFile(string fileName);
        // get production of nonTerminal and terminal
        pair<return_t,vector<CFGEntry *>>getProduction(string nonTerminalName, string terminalName);
        // get production of nonTerminal and terminal


    private:

        void fillTable(CFG &cfg);
        void fillRowOfNonTerminal(NonTerminal* nonTerminal);

};


#endif //COMPILERGENERATOR_TABLE_H
