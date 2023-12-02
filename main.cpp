//#include <iostream>
//#include <string>
//#include <vector>
//#include <set>
//
//#include "ParsingInputFile/parser.h"
//#include "AutomatonDataStructure/Node.h"
//#include "AutomatonDataStructure/Automaton.h"
//#include "AutomatonDataStructure/automatonGenerator.h"
//
//using namespace std;
//
//int Node::nodeCounter = 0;
//int main() {
//    // Read the file into a string.
//    string filepath = "/home/elsaber/compilers/Compiler-Generator/input.txt";
//    vector<string> lines = readInputFile(filepath);
//
//    // Get the regular expressions from the file.
//    vector<string> regularExpressionsVector = getRegularExpressions(lines);
//    vector<string> regularDefinitionsVector = getRegularDefinitions(lines);
//    vector<string> keywordsVector = getKeywords(lines);
//    vector<string> punctuationsVector = getPunctuations(lines);
//    set<char> reservedSymbolsSet = getReservedSymbols(lines);
//
//
//    cout << "Regular Expressions: \n";
//    for (auto & i : regularExpressionsVector) {
//        cout << i << endl;
//    }
//
//    cout << "Regular Definitions: \n";
//    for (auto & i : regularDefinitionsVector) {
//        cout << i << endl;
//    }
//
//    cout << "Keywords: \n";
//    for (auto & i : keywordsVector) {
//        cout << i << endl;
//    }
//
//    cout << "Punctuations: \n";
//    for (auto & i : punctuationsVector) {
//        cout << i << endl;
//    }
//
//    cout << "Reserved Symbols: \n";
//    for (auto & i : reservedSymbolsSet) {
//        cout << i << endl;
//    }
//
//    unordered_map<string, vector<string>> regularDefinitionsMap = getRegularDefinitionsMap(regularDefinitionsVector);
//
//    cout << "Regular Definitions Map: \n";
//    for (auto & i : regularDefinitionsMap) {
//        cout << i.first << " : ";
//        for (auto & j : i.second) {
//            cout << j << " ";
//        }
//        cout << endl;
//    }
//
//
//    unordered_map<string, Automaton> automatonMap = generateAutomatonFromRegularDefinitions(regularDefinitionsMap);
//
//    cout << "Automaton Map: \n";
//
//    printAutomatonMap(automatonMap);
//
//    return 0;
//}
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>

#include "ParsingInputFile/parser.h"
#include "AutomatonDataStructure/Node.h"
#include "AutomatonDataStructure/Automaton.h"
#include "AutomatonDataStructure/automatonGenerator.h"
#include "Postfix-expression/Postfix_expression.h"
int Node::nodeCounter = 0;
bool is_special_t(char c) {
    return c == ' ' || c == '(' || c == ')' || c == '+' || c == '*' || c == '|';
}
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
        else if(is_special_t(c) && (i - 1 < 0 || str[i  - 1] != '\\')){
           if(c=='|'){
               if(new_str[new_str.length() - 1] ==' '){
                   new_str.pop_back();
               }
               new_str.push_back(c);

               continue;
           }
           else if(is_special_t(c) && c != '('){
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
// compare two character using





int main(){
//    vector<string> v;
//    v.emplace_back("0-9");
//    unordered_map<string, Automaton> regularDef;
//    regularDef["digit"] = generateAutomatonFromRegularDefinition(v);
//    regularDef["digits"] = positiveClosure(regularDef["digit"]);
    Postfix_expression postfixExpression;
//    Automaton num = postfixExpression.postfix("\\=\\=|!\\=|>|>\\=|<|<\\=", regularDef);
    cout<<handle_spaces("z   -        a     |      b ")<<endl;
//    cout<<handle_range("a - z")<<endl;
return 0;

}