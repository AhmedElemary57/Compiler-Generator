#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

#include "parser.h"

using namespace std;


int main() {
    // Read the file into a string.
    string filepath = "D:\\.CSED\\CompilerGenerator\\input.txt";
    vector<string> lines = readInputFile(filepath);
    cout << "The first line is in the file is: \n" << lines[0] << endl;

    // Get the regular expressions from the file.
    vector<string> regularExpressionsVector = getRegularExpressions(lines);
    vector<string> regularDefinitionsVector = getRegularDefinitions(lines);
    vector<string> keywordsVector = getKeywords(lines);
    vector<string> punctuationsVector = getPunctuations(lines);


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


    return 0;
}
