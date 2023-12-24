#include "../CFGParser/CFG.h"
#include "../tablePreparation/FirstCalculation.h"
#include "../tablePreparation/FollowCalculation.h"
#ifndef COMPILERGENERATOR_TABLE_H
#define COMPILERGENERATOR_TABLE_H


class Table {

    public:
        unordered_map<string, unordered_map<string,vector<CFGEntry*>>> table; // <nonTerminal, <terminal, production>>
        Table(unordered_map<string, NonTerminal*> nonTerminals, vector<string> nonTerminalsNames);\
        void printTable();
        void printTableToFile(string fileName);
        // get production of nonTerminal and terminal
        vector<CFGEntry*> getProduction(string nonTerminalName, string terminalName);
        // get production of nonTerminal and terminal


    private:
        unordered_map<string, NonTerminal> nonTerminals;
        unordered_map<string, Terminal> terminals;
        void fillTable(unordered_map<string, NonTerminal*> nonTerminals, vector<string> nonTerminalsNames);
        vector<CFGEntry*> fillRowOfNonTerminal(NonTerminal* nonTerminal);

};


#endif //COMPILERGENERATOR_TABLE_H
