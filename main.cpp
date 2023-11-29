#include <iostream>
#include <string>
#include <vector>
#include <set>

#include "ParsingInputFile/parser.h"
#include "AutomatonDataStructure/Node.h"
#include "AutomatonDataStructure/Automaton.h"
#include "AutomatonDataStructure/automatonGenerator.h"

using namespace std;

int Node::nodeCounter = 0;
int main() {
    // Read the file into a string.
    string filepath = "/home/elsaber/compilers/Compiler-Generator/input.txt";
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

    unordered_map<string, vector<string>> regularDefinitionsMap = getRegularDefinitionsMap(regularDefinitionsVector);

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

    return 0;
}
