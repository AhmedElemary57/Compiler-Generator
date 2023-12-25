//
// Created by USER on 12/24/2023.
//

#include "Table.h"

Table::Table(CFG &cfg) {
    fillTable(cfg);
}

// search in a set of CFGEntry for a certain terminal
bool searchInSet(set<Terminal*> set, string terminalName) {
    for (auto it : set) {
        if (!(it->getName() == terminalName)) continue;
        return true;
    }
    return false;
}

// function to check wither a terminal is in first set of one non terminal productions
void Table::fillRowOfNonTerminal(NonTerminal* nonTerminal) {
    //TODO: optimize this function an use the set itself for searching
    vector<vector<CFGEntry*>> productions = nonTerminal->getProductions();

    for (int i = 0; i < productions.size(); ++i) {
        // get first set of certain production
        set<Terminal*> firstSet = nonTerminal->getFirstSet(i);
        // check if the terminal is in the first set of the production
        //For each terminal 'a' in First(α), add A → α to M[A, a]
        for( auto terminal : firstSet){
            // if  table[nonTerminal->getName()][terminal->getName()] exist then there is ambiguity
            if(table[nonTerminal->getName()].find(terminal->getName()) != table[nonTerminal->getName()].end()){
                cerr << "Error: Ambiguity in the grammar" << endl;
            }else{
                table[nonTerminal->getName()][terminal->getName()] = make_pair(Production,productions[i]);
            }
        }
    }
    set<Terminal*> followSet = nonTerminal->getFollowSet();

    if(nonTerminal->hasEpsilon()){
        for (auto it : followSet) {
            if(table[nonTerminal->getName()].find(it->getName()) != table[nonTerminal->getName()].end()){
                cerr << "Error: Ambiguity in the grammar" << endl;
            }else{
                table[nonTerminal->getName()][it->getName()] = make_pair(Epsilon,*new vector<CFGEntry*>());
            }
        }
    }
    bool flag = true;
    for (int i = 0; i < productions.size(); ++i){
        if(nonTerminal->getHasEpsilonProductionInFirst(i)){
            flag= false;
            for (auto it : followSet) {
                if(table[nonTerminal->getName()].find(it->getName()) != table[nonTerminal->getName()].end()){
                    cerr << "Error: Ambiguity in the grammar" << endl;
                }
                else table[nonTerminal->getName()][it->getName()] = make_pair(Production,productions[i]);
            }
        }
    }
    if(flag||!nonTerminal->hasEpsilon()){
        for (auto it : followSet) {
            if(table[nonTerminal->getName()].find(it->getName()) != table[nonTerminal->getName()].end()){
                continue;
            }else{
                table[nonTerminal->getName()][it->getName()] = make_pair(Sync,*new vector<CFGEntry*>());
            }
        }
    }
}

/***
 For each production A → α:
    For each terminal 'a' in First(α), add A → α to M[A, a].
    If ε is in First(α):
    For each terminal 'b' in Follow(A), add A → α to M[A, b].
    If ε is in First(α) and $ is in Follow(A), add A → α to M[A, $].
 * **/
void Table::fillTable(CFG &cfg) {

    calculateFirstToCFG(cfg);
    calculateFollowToNonTerminals(cfg.get_non_terminals_names(),cfg.get_non_terminals_map());
    vector<string> nonTerminalsNames = cfg.get_non_terminals_names();
    unordered_map<string, NonTerminal*> nonTerminals = cfg.get_names_non_terminals_map();
    for (int i = 0; i < nonTerminalsNames.size(); ++i) {
        // get first set of the non terminal
        NonTerminal* nonTerminal = nonTerminals[nonTerminalsNames[i]];
        fillRowOfNonTerminal(nonTerminal);
    }
}
//
//void Table::printTable() {
//    std::ofstream myfile;
//        std::string current_path = __FILE__;
//    current_path = current_path.substr(0, current_path.find_last_of('/')) ;
////
////
//    std::string filepath = current_path + "/CFGRules.csv";
//    myfile.open (filepath);
//    string header = "NonTerminal,";
//    // Check if the file is opened successfully
//    if (!myfile.is_open()) {
//        cerr << "Error: Unable to open the file" << endl;
//        exit(1);
//    }
//
//    set<string> names;
//    for (auto it : table) {
//        for (auto it2 : it.second) {
//            names.insert(it2.first);
//        }
//    }
//    for (auto it = names.begin(); it != names.end(); ++it) {
//        header += *it + ",";
//    }
//    header.pop_back();
//    myfile << header << endl;
//    for (auto it : table) {
//        string row = it.first + ",";
//        for (auto it2 = names.begin(); it2 != names.end(); ++it2) {
//            pair<return_t,vector<CFGEntry *>> cell = getProduction(it.first, *it2);
//            if(cell.first == nonTerminalError){
//                cerr<< "Error: No such non terminal in the grammar" << endl;
//                exit(1);
//            }
//            else if(cell.first == Production){
//                row += it.first + " -> ";
//                for (int i = 0; i < cell.second.size(); ++i) {
//                    row += cell.second[i]->getName() + " ";
//                }
//                row += ",";
//            }
//            else if(cell.first == Epsilon){
//                row += "Epsilon,";
//            }
//            else if(cell.first == Error){
//                row += "Error,";
//            }
//            else if(cell.first == Sync){
//                row += "Sync,";
//            }
//        }
//        row.pop_back();
//        myfile << row << endl;
//        row = "";
//    }
//    myfile.close();
//}


void Table::printTable(vector<string> names) {
    names.emplace_back("$");
    std::ofstream myfile;
    std::string current_path = __FILE__;
    current_path = current_path.substr(0, current_path.find_last_of('/'));

    std::string filepath = current_path + "/CFGRules.csv";
    myfile.open(filepath);

    // Check if the file is opened successfully
    if (!myfile.is_open()) {
        cerr << "Error: Unable to open the file" << endl;
        exit(1);
    }



    // Write header
    string header = "Non Terminal,";
    myfile << "NonTerminal,";
    for (const auto& name : names) {
        myfile << name << ",";
        header += name + ",";
    }
    myfile.seekp(-1, std::ios_base::end);  // Remove the trailing comma
    myfile << std::endl;
    string row = "";
    // Write data
    for (const auto& entry : table) {
        myfile << entry.first << ",";
        row += entry.first + ",";
        for (const auto& name : names) {
            pair<return_t, vector<CFGEntry*>> cell = getProduction_print(entry.first, name);
            if (cell.first == nonTerminalError) {
                cerr << "Error: No such non-terminal in the grammar" << endl;
                exit(1);
            } else if (cell.first == Production) {
                myfile << entry.first << " -> ";
                for (int i = 0; i < cell.second.size(); ++i) {
                    myfile << cell.second[i]->getName() << " ";
                    row += cell.second[i]->getName() + " ";
                }
                row += ",";
                myfile << ",";
            } else if (cell.first == Epsilon) {
                myfile << "Epsilon,";
                row += "Epsilon,";
            } else if (cell.first == Error) {
                myfile << "Error,";
                row += "Error,";
            } else if (cell.first == Sync) {
                myfile << "Sync,";
                row += "Sync,";
            }
        }
        myfile.seekp(-1, std::ios_base::end);  // Remove the trailing comma
        myfile << std::endl;
        row = "";
    }

    myfile.close();
}
pair<return_t,vector<CFGEntry *>> Table::getProduction_print(string nonTerminalName, string terminalName) {
    if(table.find(nonTerminalName) == table.end()){
        return make_pair(nonTerminalError,vector<CFGEntry*>());
    }
    if(table[nonTerminalName].find(terminalName) == table[nonTerminalName].end()){
        return make_pair(Error,vector<CFGEntry*>());
    }
    return table[nonTerminalName][terminalName];
}

pair<return_t,vector<CFGEntry *>> Table::getProduction(string nonTerminalName, string terminalName) {
    if(table.find(nonTerminalName) == table.end()){
        cerr << "Error: No such non terminal in the grammar" << endl;
        return make_pair(nonTerminalError,vector<CFGEntry*>());
    }
    if(table[nonTerminalName].find(terminalName) == table[nonTerminalName].end()){
        cerr << "Error: No such terminal in the grammar" << endl;
        return make_pair(Error,vector<CFGEntry*>());
    }
    return table[nonTerminalName][terminalName];
}
