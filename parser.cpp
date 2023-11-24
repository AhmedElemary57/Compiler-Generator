#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>

using namespace std;
// This function reads the first character from a file and returns it.
/**
 * 1- Lexical rules input file is a text file.
 * 2- Regular definitions are lines in the form LHS = RHS
 * 3- Regular expressions are lines in the form LHS: RHS
 * 4- Keywords are enclosed by { } in separate lines.
 * 5- Punctuations are enclosed by [ ] in separate lines
 * 6- \L represents Lambda symbol.
 * 7- The following symbols are used in regular definitions and regular expressions with the meaning discussed in class: - | + * ( )
 8- Any reserved symbol needed to be used within the language, is preceded by an
escapebackslash character.
 * ***/
vector<string> readInputFile(const string& filepath) {
    ifstream file(filepath);

    if (!file.is_open()) {
        std::cerr << "Error opening the file.\n";
        exit(1);
    }

    string line;
    vector<string> lines;
    while (getline(file, line)) {
        lines.push_back(line);
    }
    file.close();
    return lines;
}
unordered_map<int, vector<string>> getRegularExpressionsAndDefinition(const vector<string>& lines) {
    vector<string> regularExpressions;
    vector<string> regularDefinitions;

    for (const auto& line : lines) {
        if (line.find(':') != string::npos) {
            regularExpressions.push_back(line);
        } else if (line.find('=') != string::npos) {
            regularDefinitions.push_back(line);
        }
    }

    unordered_map<int, vector<string>> regularExpressionsAndDefinitions;
    regularExpressionsAndDefinitions[0] = regularExpressions;
    regularExpressionsAndDefinitions[1] = regularDefinitions;

    return regularExpressionsAndDefinitions;
}

void parse(string line, vector<string> &tokens) {

}

