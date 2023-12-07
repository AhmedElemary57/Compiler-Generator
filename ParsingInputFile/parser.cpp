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

int getLineType(const string& line) {

    // 1 - Regular Expression  2 - Regular Definition

    if(  (line[0] == '[' && line[line.size() - 1] == ']' )|| (line[0] == '{' && line[line.size() - 1] == '}')) {
        return -1;
    }
    int expressionIndex = line.find(':');
    int definitionIndex = line.find('=');

    if (expressionIndex != string::npos && definitionIndex == string::npos) {
        return 1;
    } else if (expressionIndex == string::npos && definitionIndex != string::npos) {
        return 2;
    } else if (expressionIndex != string::npos && definitionIndex != string::npos){
        return expressionIndex < definitionIndex?1:2;
    } else{
        return -1;
    }

}

vector<string> getRegularExpressions(const vector<string>& lines) {
    vector<string> regularExpressions;
    for (const auto& line : lines) {

        if (getLineType(line) == 1) {
            regularExpressions.push_back(line);
        }

    }
    return regularExpressions;
}

vector<string> getRegularDefinitions(const vector<string>& lines) {
    vector<string> regularDefinitions;
    for (const auto& line : lines) {
        if (getLineType(line) == 2) {
            regularDefinitions.push_back(line);
        }
    }
    return regularDefinitions;
}

vector<string> getKeywords(const vector<string>& lines) {
    vector<string> keywords;
    for (const auto& line : lines) {
        if (line[0] == '{' && line[line.size() - 1] == '}') {
            istringstream iss(line.substr(1, line.size() - 2));
            string word;
            while (iss >> word) {
                keywords.push_back(word);
            }
        }
    }
    return keywords;
}

vector<char> getPunctuations(const vector<string>& lines) {
    vector<char> punctuations;
    for (const auto& line : lines) {
        if (line[0] == '[' && line[line.size() - 1] == ']') {
            string l = line.substr(1, line.size() - 2);
            for( char c : l){
                if(c != ' ' && c != '\\' && c != '\\'){
                    punctuations.push_back(c);
                }
            }
        }
    }
    return punctuations;
}

set<char> getReservedSymbols(const vector<string>& lines) {
/*
    set<char> reservedSymbols;
    vector<string> regularExpressions = getRegularExpressions(lines);
    vector<char> punctuations = getPunctuations(lines);

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

*/

    return set<char>();

}

pair<unordered_map<string, string>, vector<string>> getRegularDefinitionsMap(const vector<string>& regularDefinitions) {
    unordered_map<string, string> regularDefinitionsMap;
    pair<unordered_map<string, string>, vector<string>> result;
    vector<string> regularDefinitionsVector;
    for (const auto& regularDefinition : regularDefinitions) {

        string regularDefinitionName = regularDefinition.substr(0, regularDefinition.find('='));
        string regularDefinitionExpression = regularDefinition.substr(regularDefinition.find('=') + 1);
        // split the regularDefinitionExpression by the | character.
        istringstream iss(regularDefinitionExpression);
        // remove the spaces from regularDefinitionName .
        regularDefinitionName.erase(remove_if(regularDefinitionName.begin(), regularDefinitionName.end(), ::isspace), regularDefinitionName.end());
        regularDefinitionsMap[regularDefinitionName]= regularDefinitionExpression;
        regularDefinitionsVector.push_back(regularDefinitionName);
    }
    result.first = regularDefinitionsMap;
    result.second = regularDefinitionsVector;
    return result;
}

unordered_map<string, string> getRegularExpressionsMap(const vector<string>& regularExpressions) {
    unordered_map<string, string> regularExpressionsMap;

    for (const auto& regularExpression : regularExpressions) {
        string regularExpressionName = regularExpression.substr(0, regularExpression.find(':'));
        string regularExpressionExpression = regularExpression.substr(regularExpression.find(':') + 1);
        // remove the spaces from regularExpressionName .
        regularExpressionName.erase(remove_if(regularExpressionName.begin(), regularExpressionName.end(), ::isspace), regularExpressionName.end());
        regularExpressionsMap[regularExpressionName] = regularExpressionExpression;
    }
    return regularExpressionsMap;
}

// This function returns priority map for the regular expressions.
unordered_map<string, int> getRegularExpressionsPriorityMap(const vector<string>& regularExpressions) {
    unordered_map<string, int> regularExpressionsPriorityMap;
    int priority = 1;
    for (const auto& regularExpression : regularExpressions) {
        string regularExpressionName = regularExpression.substr(0, regularExpression.find(':'));
        // remove the spaces from regularExpressionName .
        regularExpressionName.erase(remove_if(regularExpressionName.begin(), regularExpressionName.end(), ::isspace), regularExpressionName.end());
        regularExpressionsPriorityMap[regularExpressionName] = priority++;
    }

    return regularExpressionsPriorityMap;
}

// This function returns priority map for keywords. The priority of keywords is always -1.
unordered_map<string, int> getKeywordsPriorityMap(const vector<string>& keywords) {
    unordered_map<string, int> keywordsPriorityMap;
    for (const auto& keyword : keywords) {
        keywordsPriorityMap[keyword] = -1;
    }
    return keywordsPriorityMap;
}

// This function returns priority map for punctuations. The priority of punctuations is always -1.
unordered_map<char, int> getPunctuationsPriorityMap(const vector<char>& punctuations) {
    unordered_map<char, int> punctuationsPriorityMap;
    for (const auto& punctuation : punctuations) {
        punctuationsPriorityMap[punctuation] = -1;
    }
    return punctuationsPriorityMap;
}

// This function returns priority map.
unordered_map<string, int> getPriorityMap(const vector<string>& lines) {
    unordered_map<string, int> priorityMap;
    vector<string> regularExpressions = getRegularExpressions(lines);
    vector<string> keywords = getKeywords(lines);
    vector<char> punctuations = getPunctuations(lines);

    // get the priority map for regular expressions.
    unordered_map<string, int> regularExpressionsPriorityMap = getRegularExpressionsPriorityMap(regularExpressions);
    // get the priority map for keywords.
    unordered_map<string, int> keywordsPriorityMap = getKeywordsPriorityMap(keywords);
    // get the priority map for punctuations.
    unordered_map<char, int> punctuationsPriorityMap = getPunctuationsPriorityMap(punctuations);

    // merge the priority maps.
    priorityMap.insert(regularExpressionsPriorityMap.begin(), regularExpressionsPriorityMap.end());
    priorityMap.insert(keywordsPriorityMap.begin(), keywordsPriorityMap.end());
    for (const auto& punctuationPriority : punctuationsPriorityMap) {
        priorityMap[string(1, punctuationPriority.first)] = punctuationPriority.second;
    }

    return priorityMap;
}

unordered_map<string, int> getPriorityMap(vector<string> regularExpressions, vector<string> keywords, vector<char> punctuations) {

    // get the priority map for regular expressions.
    unordered_map<string, int> regularExpressionsPriorityMap = getRegularExpressionsPriorityMap(regularExpressions);
    // get the priority map for keywords.
    unordered_map<string, int> keywordsPriorityMap = getKeywordsPriorityMap(keywords);
    // get the priority map for punctuations.
    unordered_map<char, int> punctuationsPriorityMap = getPunctuationsPriorityMap(punctuations);

    // merge the priority maps.
    unordered_map<string, int> priorityMap;
    priorityMap.insert(regularExpressionsPriorityMap.begin(), regularExpressionsPriorityMap.end());
    priorityMap.insert(keywordsPriorityMap.begin(), keywordsPriorityMap.end());
    for (const auto& punctuationPriority : punctuationsPriorityMap) {
        priorityMap[string(1, punctuationPriority.first)] = punctuationPriority.second;
    }

    return priorityMap;
}



