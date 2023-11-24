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
    unordered_map<int, vector<string>> regularExpressions = getRegularExpressionsAndDefinition(lines);
    vector<string> regularExpressionsVector = regularExpressions[0];
    vector<string> regularDefinitionsVector = regularExpressions[1];

    cout << "Regular Expressions: \n";
    for (int i = 0; i < regularExpressionsVector.size(); i++) {
        cout << regularExpressionsVector[i] << endl;
    }

    cout << "Regular Definitions: \n";
    for (int i = 0; i < regularDefinitionsVector.size(); i++) {
        cout << regularDefinitionsVector[i] << endl;
    }


    return 0;
}
