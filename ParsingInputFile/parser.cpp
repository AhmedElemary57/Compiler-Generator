#include <algorithm>
#include "../Postfix-expression/Postfix_expression.h"
#include "parser.h"

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
void add_space(string &str,char next){
    if(str.empty()){
        return;
    }
    char c= str[str.length()-1];
    if(c == ' ' && (next == '*'|| next == '+')){
        str.pop_back();
    }
    if(c == ' ' || c == '(' || c == '|') {
        return;
    }
    else if(next == ')' || next == '|' || next == '*'){
        return;
    }
    else{
        str.push_back(' ');
    }
}
bool is_range(char before, char after){
    return before < after;
}
std::vector<std::string> split_string(const std::string& inputString) {
    std::istringstream iss(inputString);
    std::vector<std::string> tokens;

    std::string word;
    while (iss >> word) {
        tokens.push_back(word);
    }
    return tokens;
}
// Function to handle ranges in a string
std::string handle_range(const std::string& str) {
    std::vector<std::string> tokens = split_string(str);
    std::string new_str;

    for (size_t i = 0; i < tokens.size(); ++i) {
        if (tokens[i].size() == 1 && tokens[i] == "-") {
            if (i > 0 && i + 1 < tokens.size() &&
                tokens[i - 1].size() == 1 && tokens[i + 1].size() == 1 &&
                is_range(tokens[i - 1][0], tokens[i + 1][0])) {
                new_str.push_back('-');
                new_str.append(tokens[i + 1]);
                ++i;
            } else {
                new_str.push_back(' ');
                new_str.push_back('-');
            }
        } else if (tokens[i].size() == 2 && tokens[i][0] == '-') {
            if (i > 0 && tokens[i - 1].size() == 1 &&
                is_range(tokens[i - 1][0], tokens[i][1])) {
                new_str.append(tokens[i]);
            } else {
                new_str.push_back(' ');
                new_str.append(tokens[i]);
            }
        } else if (tokens[i].size() == 2 && tokens[i][1] == '-') {
            if (i + 1 < tokens.size() && tokens[i + 1].size() == 1 &&
                is_range(tokens[i][0], tokens[i + 1][0])) {
                new_str.append(tokens[i]);
                new_str.append(tokens[i + 1]);
                ++i;
            } else {
                new_str.push_back(' ');
                new_str.append(tokens[i]);
            }
        } else {
            if (i != 0) {
                new_str.push_back(' ');
            }
            new_str.append(tokens[i]);

        }
    }

    return new_str;
}
string handle_spaces(string str){
    str = handle_range(str);
    string new_str = "";
    for (int i = 0; i < str.length(); ++i) {
        char c = str[i];
        if(c == ' ' && i + 1 < str.length()){
            add_space(new_str, str[i + 1]);
        }
        else if(is_special(c) && (i - 1 < 0 || str[i  - 1] != '\\')){
            if(c=='|'){
                if(new_str[new_str.length() - 1] ==' '){
                    new_str.pop_back();
                }
                new_str.push_back(c);

                continue;
            }
            else if(is_special(c) && c != '('){
                if(i + 1 < str.length() ) {
                    new_str.push_back(c);
                    add_space(new_str, str[i + 1]);
                }
                else{
                    new_str.push_back(c);

                }
            }
            else{
                if(i + 1 < str.length()) {
                    add_space(new_str, str[i + 1]);
                    new_str.push_back(c);
                }
                else{
                    new_str.push_back(c);

                }
            }
        }
        else{
            new_str.push_back(c);
        }
    }

    return  new_str;
}
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
        if (   line.find(':') != string::npos
            && line[0] != '[' && line[line.size() - 1] != ']'
            && line[0] != '{' && line[line.size() - 1] != '}') {
            regularExpressions.push_back(line);
        }
    }
    return regularExpressions;
}

vector<string> getRegularDefinitions(const vector<string>& lines) {
    vector<string> regularDefinitions;
    for (const auto& line : lines) {
        if (line.find('=') != string::npos && line.find(':') == string::npos
        && line[0] != '[' && line[line.size() - 1] != ']'
        && line[0] != '{' && line[line.size() - 1] != '}') {
            regularDefinitions.push_back(line);
        }
    }
    return regularDefinitions;
}

vector<string> getKeywords(const vector<string>& lines) {
    vector<string> keywords;
    for (const auto& line : lines) {
        if (line[0] == '{' && line[line.size() - 1] == '}') {
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

unordered_map<string, vector<string>> getRegularDefinitionsMap(const vector<string>& regularDefinitions) {
    unordered_map<string, vector<string>> regularDefinitionsMap;

    for (const auto& regularDefinition : regularDefinitions) {
        string regularDefinitionName = regularDefinition.substr(0, regularDefinition.find('='));
        string regularDefinitionExpression = regularDefinition.substr(regularDefinition.find('=') + 1);
        // split the regularDefinitionExpression by the | character.
        istringstream iss(regularDefinitionExpression);
        // remove the spaces from regularDefinitionName .
        regularDefinitionName.erase(remove_if(regularDefinitionName.begin(), regularDefinitionName.end(), ::isspace), regularDefinitionName.end());
        string word;
        while (iss >> word) {
            if(word != "|")
                regularDefinitionsMap[regularDefinitionName].push_back(word);
        }

    }
    return regularDefinitionsMap;
}


