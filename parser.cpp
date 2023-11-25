#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <set>

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
 * 8- Any reserved symbol needed to be used within the language, is preceded by an escape backslash character.
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

vector<string> getRegularExpressions(const vector<string>& lines) {
    vector<string> regularExpressions;
    for (const auto& line : lines) {
        if (line.find(':') != string::npos) {
            regularExpressions.push_back(line);
        }
    }
    return regularExpressions;
}

vector<string> getRegularDefinitions(const vector<string>& lines) {
    vector<string> regularDefinitions;
    for (const auto& line : lines) {
        if (line.find('=') != string::npos) {
            regularDefinitions.push_back(line);
        }
    }
    return regularDefinitions;
}

vector<string> getKeywords(const vector<string>& lines) {
    vector<string> keywords;
    for (const auto& line : lines) {
        if (line.find('{') != string::npos && line.find('}') != string::npos && line[0] == '{' && line[line.size() - 1] == '}') {
            istringstream iss(line);
            string word;
            while (iss >> word) {
                if (word.find('{') != string::npos || word.find('}') != string::npos) {
                    continue;
                }
                keywords.push_back(word);
            }
        }
    }
    return keywords;
}

vector<string> getPunctuations(const vector<string>& lines) {
    vector<string> punctuations;
    for (const auto& line : lines) {
        if (line[0] == '[' && line[line.size() - 1] == ']') {
            istringstream iss(line.substr(1, line.size() - 2));
            string word;
            while (iss >> word) {
                punctuations.push_back(word);
            }
        }
    }
    return punctuations;
}

set<char> getReservedSymbols(const vector<string>& lines) {
    set<char> reservedSymbols;
    vector<string> regularExpressions = getRegularExpressions(lines);
    vector<string> punctuations = getPunctuations(lines);

    // loop over the regular expressions and add the reserved symbols to the set.
    for (const auto& regularExpression : regularExpressions ) {
        istringstream iss(regularExpression);
        string word;
        while (iss >> word) {
           if (word.find('\\') != string::npos && word.find('\\') != word.size() - 1 && word[word.find('\\') + 1] != 'L') {
               // Insert the next character after the backslash.
                reservedSymbols.insert(word[word.find('\\') + 1]);
           }
        }
    }

    // loop over the punctuations and add the reserved symbols to the set.
    for (const auto& punctuation : punctuations ) {
        istringstream iss(punctuation);
        string word;
        while (iss >> word) {
           if (word.find('\\') != string::npos && word.find('\\') != word.size() - 1 && word[word.find('\\') + 1] != 'L') {
               // Insert the next character after the backslash.
                reservedSymbols.insert(word[word.find('\\') + 1]);
           }
        }
    }


    return reservedSymbols;
}






