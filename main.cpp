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

int main()
{
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

    pair<unordered_map<string, string>, vector<string>> regularDefinitionsPair = getRegularDefinitionsMap(regularDefinitionsVector);
    unordered_map<string, string> regularDefinitionsMap = regularDefinitionsPair.first;

    unordered_map<string, Automaton> automatonMap = generateAutomatonFromRegularDefinitions(regularDefinitionsPair);

    unordered_map<string, string> regularExpressionsMap = getRegularExpressionsMap(regularExpressionsVector);
    unordered_map<string, Automaton> regularExpressionsAutomatonMap = generateAutomatonFromRegularExpressions(regularExpressionsMap, automatonMap);
    unordered_map<string, Automaton> keyWorldsAutomaton = generateAutomatonFromKeyWords(keywordsVector);
    unordered_map<char, Automaton> punctuationsAutomaton = generateAutomatonFromPunctuations(punctuationsVector);

    unordered_map<string, int> regularExpressionsPriorityMap = getPriorityMap(regularExpressionsVector, keywordsVector, punctuationsVector);

    CombinedAutomaton combinedAutomaton;
    combinedAutomaton.generateCombinedAutomaton(regularExpressionsAutomatonMap, keyWorldsAutomaton, punctuationsAutomaton);
    combinedAutomaton.setPriorityMap(regularExpressionsPriorityMap);
    combinedAutomaton.getFinalNodesMap();
    combinedAutomaton.getStartNode();

    CombinedAutomaton DFA = NFADFAConverter::convertNFAToDFA(combinedAutomaton, regularExpressionsPriorityMap);

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
    cout << endl;

    current_path = __FILE__;
    current_path = current_path.substr(0, current_path.find_last_of('/')) ;
    filepath = current_path + "/CFGRules.txt";

    CFG cfg = parseCFG(filepath);
    cfg.print_productions();
    cout << endl;

    cfg.left_recursion_elimination();
    cfg.left_factoring();
    cfg.print_productions();
    cout << endl;

    Table table = Table(cfg);
    cfg.printFirstAndFollowSets();
    cout << endl;
    table.printTable();
    cout << endl;
}
