#include <iostream>
#include <string>
#include <vector>
#include <set>


#include "ParsingInputFile/parser.h"
#include "AutomatonDataStructure/Node.h"
#include "AutomatonDataStructure/Automaton.h"
#include "AutomatonDataStructure/automatonGenerator.h"
#include "Postfix-expression/Postfix_expression.h"

#include "AutomatonDataStructure/CombinedAutomaton.h"

#include "NFADFAConverter/NFADFAConverter.h"

#include "LexicalAnalyzer/LexicalAnalyzer.h"

#include "CFGParser/CFG.h"
#include "CFGParser/CFGParser.h"

#include "tablePreparation/FirstCalculation.h"
#include "tablePreparation/FollowCalculation.h"

#include "table/Table.h"

using namespace std;

int Node::nodeCounter = 0;
//
//int hide(){
//
//// get current path of the project
//    std::string current_path = __FILE__;
//    current_path = current_path.substr(0, current_path.find_last_of('/')) ;
//
//    // Read the file into a string.
//    std::string filepath = current_path + "/input.txt";
//    vector<string> lines = readInputFile(filepath);
//
//    // Get the regular expressions from the file.
//    vector<string> regularExpressionsVector = getRegularExpressions(lines);
//    vector<string> regularDefinitionsVector = getRegularDefinitions(lines);
//    vector<string> keywordsVector = getKeywords(lines);
//    vector<char> punctuationsVector = getPunctuations(lines);
//    set<char> reservedSymbolsSet = getReservedSymbols(lines);
//
//
//    cout << "Regular Expressions: \n";
//    for (auto & i : regularExpressionsVector) {
//        cout << i << endl;
//    }
//
//    cout << "Regular Definitions: \n";
//    for (auto & i : regularDefinitionsVector) {
//        cout << i << endl;
//    }
//
//    cout << "Keywords: \n";
//    for (auto & i : keywordsVector) {
//        cout << i << endl;
//    }
//
//    cout << "Punctuations: \n";
//    for (auto & i : punctuationsVector) {
//        cout << i << endl;
//    }
//
//    cout << "Reserved Symbols: \n";
//    for (auto & i : reservedSymbolsSet) {
//        cout << i << endl;
//    }
//    pair<unordered_map<string, string>, vector<string>> regularDefinitionsPair = getRegularDefinitionsMap(regularDefinitionsVector);
//    unordered_map<string, string> regularDefinitionsMap = regularDefinitionsPair.first;
//
//    cout << "Regular Definitions Map: \n";
//    for (auto & i : regularDefinitionsMap) {
//        cout << i.first << " : ";
//        for (auto & j : i.second) {
//            cout << j << " ";
//        }
//        cout << endl;
//    }
//
//
//    unordered_map<string, Automaton> automatonMap = generateAutomatonFromRegularDefinitions(regularDefinitionsPair);
//
//    cout << "Automaton Map: \n";
//
//    printAutomatonMap(automatonMap);
//
//    // get regular expression  map
//    unordered_map<string, string> regularExpressionsMap = getRegularExpressionsMap(regularExpressionsVector);
//    unordered_map<string, Automaton> regularExpressionsAutomatonMap = generateAutomatonFromRegularExpressions(regularExpressionsMap, automatonMap);
//    unordered_map<string, Automaton> keyWorldsAutomaton = generateAutomatonFromKeyWords(keywordsVector);
//    unordered_map<char, Automaton> punctuationsAutomaton = generateAutomatonFromPunctuations(punctuationsVector);
//
//    unordered_map<string, int> regularExpressionsPriorityMap = getPriorityMap(regularExpressionsVector, keywordsVector, punctuationsVector);
//
//    cout << "Regular Expressions Automaton Map: \n";
//    printAutomatonMap(regularExpressionsAutomatonMap);
//
//    cout << "Regular Expressions Priority Map: \n";
//    for (auto & i : regularExpressionsPriorityMap) {
//        cout << i.first << " : " << i.second << endl;
//    }
//
//    // test the combined automaton
//    CombinedAutomaton combinedAutomaton;
//
//
//    combinedAutomaton.generateCombinedAutomaton(regularExpressionsAutomatonMap, keyWorldsAutomaton, punctuationsAutomaton);
//    combinedAutomaton.setPriorityMap(regularExpressionsPriorityMap);
//
//    combinedAutomaton.getFinalNodesMap();
//    combinedAutomaton.getStartNode();
//
//
//    cout << "starting to convert" << endl;
//
//    CombinedAutomaton DFA = NFADFAConverter::convertNFAToDFA(combinedAutomaton, regularExpressionsPriorityMap);
//
//    cout << "converted" << endl;
//
//    cout << endl;
//
//    ifstream inFile("./sample_programs/p3.txt");
//    if (!inFile.is_open()) {
//        std::cerr << "Unable to open the file." << std::endl;
//        return 1;
//    }
//
//    std::stringstream buffer;
//    buffer << inFile.rdbuf();
//    string program = buffer.str();
//    inFile.close();
//    LexicalAnalyzer *lex = new LexicalAnalyzer(&DFA, program);
//    vector<pair<string, string>> tokens = lex->getAllTokens();
//    lex->print_tokens();
//    lex->write_tokens("./sample_programs/p3_out.txt");
//    cout << endl;
//    lex->print_symbol_table();
//    return 0;
//}
//
//
//int main()
//{
//
//
//    std::string current_path = __FILE__;
//    current_path = current_path.substr(0, current_path.find_last_of('/')) ;
//
//
//    std::string filepath = current_path + "/CFGRules.txt";
//
//    CFG cfg = parseCFG(filepath);
//    cfg.print_productions();
//    cfg.left_recursion_elimination();
//    cout << "Grammar after eliminating left recursion" << endl;
//    cfg.print_productions();
//    cfg.left_factoring();
//    cout << "Grammar after eliminating left factoring" << endl;
//    cfg.print_productions();
//    return 0;
//}
int main(){

    ////E  TE’

    //T  FT’
    //T’  *FT’ | 
    //F  (E) | id*****
    /// **///
    NonTerminal nt_E = *new NonTerminal("E");
    NonTerminal nt_E_ = *new NonTerminal("E'");
    NonTerminal nt_T = *new NonTerminal("T");
    NonTerminal nt_T_ = *new NonTerminal("T'");
    NonTerminal nt_F = *new NonTerminal("F");

    vector<CFGEntry *> production;
    production.push_back(&nt_T);
    production.push_back(&nt_E_);
    nt_E.addProduction(production);

    //E’  +TE’ | 
    production.clear();
    production.push_back(new Terminal("+"));
    production.push_back(&nt_T);
    production.push_back(&nt_E_);
    nt_E_.addProduction(production);

    production.clear();
    production.push_back(new Terminal("\\L"));
    nt_E_.addProduction(production);

    //T  FT’
    production.clear();
    production.push_back(&nt_F);
    production.push_back(&nt_T_);
    nt_T.addProduction(production);

    //T’  *FT’ | 
    production.clear();
    production.push_back(new Terminal("*"));
    production.push_back(&nt_F);
    production.push_back(&nt_T_);
    nt_T_.addProduction(production);

    production.clear();
    production.push_back(new Terminal("\\L"));
    nt_T_.addProduction(production);

    //F  (E) | id
    production.clear();
    production.push_back(new Terminal("("));
    production.push_back(&nt_E);
    production.push_back(new Terminal(")"));
    nt_F.addProduction(production);

    production.clear();
    production.push_back(new Terminal("id"));
    nt_F.addProduction(production);

    vector<string> nonTerminalsNames;
    nonTerminalsNames.push_back(nt_E.getName());
    nonTerminalsNames.push_back(nt_E_.getName());
    nonTerminalsNames.push_back(nt_T.getName());
    nonTerminalsNames.push_back(nt_T_.getName());
    nonTerminalsNames.push_back(nt_F.getName());

    unordered_map<string, NonTerminal *> namesNonTerminalsMap;
    namesNonTerminalsMap[nt_E.getName()] = &nt_E;
    namesNonTerminalsMap[nt_E_.getName()] = &nt_E_;
    namesNonTerminalsMap[nt_T.getName()] = &nt_T;
    namesNonTerminalsMap[nt_T_.getName()] = &nt_T_;
    namesNonTerminalsMap[nt_F.getName()] = &nt_F;




    CFG cfg = *new CFG(nonTerminalsNames, namesNonTerminalsMap);
    cfg.left_recursion_elimination();
    cfg.left_factoring();
    cfg.print_productions();


    calculateFirstToCFG(make_pair(nonTerminalsNames, namesNonTerminalsMap));


    calculateFollowToNonTerminals(nonTerminalsNames, namesNonTerminalsMap);

    // print first and follow
    cout << "First and Follow sets: \n";
    for (auto & i : nonTerminalsNames) {
        // print first
        cout << "First(" << i << ") = {";
        for (auto & j : namesNonTerminalsMap[i]->getAllFirstSet()) {
            cout << j->getName() << ", ";
        }
        cout << "}\t\t";

        // print follow
        cout << "Follow(" << i << ") = {";
        for (auto & j : namesNonTerminalsMap[i]->getFollowSet()) {
            cout << j->getName() << ", ";
        }
        cout << "}\n";

    }

    // create parsing table
    Table table = Table(namesNonTerminalsMap, nonTerminalsNames);

    // print parsing table
    table.printTable();

}