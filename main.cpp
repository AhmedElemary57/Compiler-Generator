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

using namespace std;

int Node::nodeCounter = 0;
int main()
{

    // get current path of the project
    std::string current_path = __FILE__;
    current_path = current_path.substr(0, current_path.find_last_of('\\')) ;

    // Read the file into a string.
    std::string filepath = current_path + "\\input.txt";
    cout<< filepath << endl;
    vector<string> lines = readInputFile(filepath);

    // Get the regular expressions from the file.
    vector<string> regularExpressionsVector = getRegularExpressions(lines);
    vector<string> regularDefinitionsVector = getRegularDefinitions(lines);
    vector<string> keywordsVector = getKeywords(lines);
    vector<string> punctuationsVector = getPunctuations(lines);
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

    unordered_map<string, string> regularDefinitionsMap = getRegularDefinitionsMap(regularDefinitionsVector);

    cout << "Regular Definitions Map: \n";
    for (auto & i : regularDefinitionsMap) {
        cout << i.first << " : ";
        for (auto & j : i.second) {
            cout << j << " ";
        }
        cout << endl;
    }


    unordered_map<string, Automaton> automatonMap = generateAutomatonFromRegularDefinitions(regularDefinitionsMap);

    cout << "Automaton Map: \n";

    printAutomatonMap(automatonMap);

    // get regular expression  map
    unordered_map<string, string> regularExpressionsMap = getRegularExpressionsMap(regularExpressionsVector);
    unordered_map<string, Automaton> regularExpressionsAutomatonMap = generateAutomatonFromRegularExpressions(regularExpressionsMap, automatonMap);
    unordered_map<string, int> regularExpressionsPriorityMap = getRegularExpressionsPriorityMap(regularExpressionsVector);

    cout << "Regular Expressions Automaton Map: \n";
    printAutomatonMap(regularExpressionsAutomatonMap);

    cout << "Regular Expressions Priority Map: \n";
    for (auto & i : regularExpressionsPriorityMap) {
        cout << i.first << " : " << i.second << endl;
    }

    // test the combined automaton
    CombinedAutomaton combinedAutomaton;


    combinedAutomaton.generateCombinedAutomaton(regularExpressionsAutomatonMap);
    combinedAutomaton.setPriorityMap(regularExpressionsPriorityMap);

    combinedAutomaton.getFinalNodesMap();
    combinedAutomaton.getStartNode();


    cout << "starting to convert" << endl;

    CombinedAutomaton DFA = NFADFAConverter::convertNFAToDFA(combinedAutomaton, regularExpressionsPriorityMap);

    cout << "converted" << endl;

    string program = "int sum , count , pass , mnt; while (pass !=10){pass = pass + 1 ;}";
    LexicalAnalyzer *lex = new LexicalAnalyzer(&DFA, program);



    return 0;
}