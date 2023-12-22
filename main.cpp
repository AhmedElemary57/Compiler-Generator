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

using namespace std;

int Node::nodeCounter = 0;
void hide1(){
    NonTerminal *a1 = new NonTerminal("E");
    a1->hasEpsilonProduction = false;
    vector<vector<CFGEntry *>> productions1;
    vector<CFGEntry *> p1;

    CFGEntry *c1 = new CFGEntry("E");
    p1.push_back(c1);

    CFGEntry *c2 = new CFGEntry("+");
    p1.push_back(c2);

    CFGEntry *c3 = new CFGEntry("T");
    p1.push_back(c3);

    vector<CFGEntry *> p2;
    CFGEntry *c8 = new CFGEntry("T");
    p2.push_back(c8);
    productions1.push_back(p1);
    productions1.push_back(p2);
    a1->setProductions(productions1);
    
    NonTerminal *a2 = new NonTerminal("T");
    a2->hasEpsilonProduction = false;
    vector<vector<CFGEntry *>> productions2;
    vector<CFGEntry *> p3;

    CFGEntry *c4 = new CFGEntry("T");
    p3.push_back(c4);

    CFGEntry *c5 = new CFGEntry("*");
    p3.push_back(c5);

    CFGEntry *c6 = new CFGEntry("F");
    p3.push_back(c6);

    vector<CFGEntry *> p4;
    CFGEntry *c7 = new CFGEntry("F");
    p4.push_back(c7);
    productions2.push_back(p3);
    productions2.push_back(p4);
    a2->setProductions(productions2);

    NonTerminal *a3 = new NonTerminal("F");
    a3->hasEpsilonProduction = false;

    vector<vector<CFGEntry *>> productions3;
    vector<CFGEntry *> p5;

    CFGEntry *c9 = new CFGEntry("(");
    p5.push_back(c9);

    CFGEntry *c10 = new CFGEntry("E");
    p5.push_back(c10);

    CFGEntry *c11 = new CFGEntry(")");
    p5.push_back(c11);

    vector<CFGEntry *> p6;
    CFGEntry *c12 = new CFGEntry("id");
    p6.push_back(c12);
    productions3.push_back(p5);
    productions3.push_back(p6);

    a3->setProductions(productions3);


    vector<string> nonTerminalsNames;
    unordered_map<string, NonTerminal *> namesNonTerminalsMap;

    nonTerminalsNames.push_back("E");
    nonTerminalsNames.push_back("T");
    nonTerminalsNames.push_back("F");

    namesNonTerminalsMap.insert({"E", a1});
    namesNonTerminalsMap.insert({"T", a2});
    namesNonTerminalsMap.insert({"F", a3});
}
int hide(){
// get current path of the project
    std::string current_path = __FILE__;
    current_path = current_path.substr(0, current_path.find_last_of('/')) ;

    // Read the file into a string.
    std::string filepath = current_path + "/input.txt";
    vector<string> lines = readInputFile(filepath);

    // Get the regular expressions from the file.
    vector<string> regularExpressionsVector = getRegularExpressions(lines);
    vector<string> regularDefinitionsVector = getRegularDefinitions(lines);
    vector<string> keywordsVector = getKeywords(lines);
    vector<char> punctuationsVector = getPunctuations(lines);
    set<char> reservedSymbolsSet = getReservedSymbols(lines);


    cout << "Regular Expressions: \n";
    for (auto & i : regularExpressionsVector) {
        cout << i << endl;
    }

    cout << "Regular Definitions: \n";
    for (auto & i : regularDefinitionsVector) {
        cout << i << endl;
    }

    cout << "Keywords: \n";
    for (auto & i : keywordsVector) {
        cout << i << endl;
    }

    cout << "Punctuations: \n";
    for (auto & i : punctuationsVector) {
        cout << i << endl;
    }

    cout << "Reserved Symbols: \n";
    for (auto & i : reservedSymbolsSet) {
        cout << i << endl;
    }
    pair<unordered_map<string, string>, vector<string>> regularDefinitionsPair = getRegularDefinitionsMap(regularDefinitionsVector);
    unordered_map<string, string> regularDefinitionsMap = regularDefinitionsPair.first;

    cout << "Regular Definitions Map: \n";
    for (auto & i : regularDefinitionsMap) {
        cout << i.first << " : ";
        for (auto & j : i.second) {
            cout << j << " ";
        }
        cout << endl;
    }


    unordered_map<string, Automaton> automatonMap = generateAutomatonFromRegularDefinitions(regularDefinitionsPair);

    cout << "Automaton Map: \n";

    printAutomatonMap(automatonMap);

    // get regular expression  map
    unordered_map<string, string> regularExpressionsMap = getRegularExpressionsMap(regularExpressionsVector);
    unordered_map<string, Automaton> regularExpressionsAutomatonMap = generateAutomatonFromRegularExpressions(regularExpressionsMap, automatonMap);
    unordered_map<string, Automaton> keyWorldsAutomaton = generateAutomatonFromKeyWords(keywordsVector);
    unordered_map<char, Automaton> punctuationsAutomaton = generateAutomatonFromPunctuations(punctuationsVector);

    unordered_map<string, int> regularExpressionsPriorityMap = getPriorityMap(regularExpressionsVector, keywordsVector, punctuationsVector);

    cout << "Regular Expressions Automaton Map: \n";
    printAutomatonMap(regularExpressionsAutomatonMap);

    cout << "Regular Expressions Priority Map: \n";
    for (auto & i : regularExpressionsPriorityMap) {
        cout << i.first << " : " << i.second << endl;
    }

    // test the combined automaton
    CombinedAutomaton combinedAutomaton;


    combinedAutomaton.generateCombinedAutomaton(regularExpressionsAutomatonMap, keyWorldsAutomaton, punctuationsAutomaton);
    combinedAutomaton.setPriorityMap(regularExpressionsPriorityMap);

    combinedAutomaton.getFinalNodesMap();
    combinedAutomaton.getStartNode();


    cout << "starting to convert" << endl;

    CombinedAutomaton DFA = NFADFAConverter::convertNFAToDFA(combinedAutomaton, regularExpressionsPriorityMap);

    cout << "converted" << endl;

    cout << endl;

    ifstream inFile("./sample_programs/p3.txt");
    if (!inFile.is_open()) {
        std::cerr << "Unable to open the file." << std::endl;
        return 1; 
    }
    
    std::stringstream buffer;
    buffer << inFile.rdbuf();
    string program = buffer.str();
    inFile.close();
    LexicalAnalyzer *lex = new LexicalAnalyzer(&DFA, program);
    vector<pair<string, string>> tokens = lex->getAllTokens();
    lex->print_tokens();
    lex->write_tokens("./sample_programs/p3_out.txt");
    cout << endl;
    lex->print_symbol_table();
    return 0;
}
int main()
{

    NonTerminal *a1 = new NonTerminal("A");
    a1->hasEpsilonProduction = false;
    vector<vector<CFGEntry *>> productions1;
    vector<CFGEntry *> p1;

    CFGEntry *c1 = new CFGEntry("a");
    p1.push_back(c1);

    CFGEntry *c2 = new CFGEntry("B");
    p1.push_back(c2);


    
    CFGEntry *c3 = new CFGEntry("C");
    p1.push_back(c3);

    CFGEntry *c10 = new CFGEntry("D");
    p1.push_back(c10);

    vector<CFGEntry *> p2;
    CFGEntry *c4 = new CFGEntry("a");
    p2.push_back(c4);

    
    CFGEntry *c5 = new CFGEntry("B");
    p2.push_back(c5);

    CFGEntry *c6 = new CFGEntry("E");
    p2.push_back(c6);

    CFGEntry *c11 = new CFGEntry("F");
    p2.push_back(c11);

    vector<CFGEntry *> p3;
    CFGEntry *c9 = new CFGEntry("a");
    p3.push_back(c9);

    CFGEntry *c55 = new CFGEntry("B");
    p3.push_back(c55);
    
    CFGEntry *c56 = new CFGEntry("C");
    p3.push_back(c56);

    CFGEntry *c57 = new CFGEntry("T");
    p3.push_back(c57);

    productions1.push_back(p1);
    productions1.push_back(p2);
    productions1.push_back(p3);
  
    a1->setProductions(productions1);
    
    

    


    vector<string> nonTerminalsNames;
    unordered_map<string, NonTerminal *> namesNonTerminalsMap;

    nonTerminalsNames.push_back("A");
    
    

    namesNonTerminalsMap.insert({"A", a1});
  
    
    
    CFG *cfg = new CFG(nonTerminalsNames, namesNonTerminalsMap);
    cfg->print_productions();
    cout << endl;
    cfg->left_factoring();
    cfg->print_productions();

    return 0;
}