#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>

using namespace std;
#ifndef COMPILERGENERATOR_PARSER_H
#define COMPILERGENERATOR_PARSER_H

vector<string> readInputFile(const string& filepath);
vector<string> getRegularExpressions(const vector<string>& lines);
vector<string> getRegularDefinitions(const vector<string>& lines);
vector<string> getKeywords(const vector<string>& lines);
vector<char> getPunctuations(const vector<string>& lines);
set<char> getReservedSymbols(const vector<string>& lines);
string handle_range(const std::string& str);
string handle_spaces(string str);
pair<unordered_map<string, string>, vector<string>> getRegularDefinitionsMap(const vector<string>& regularDefinitions);
unordered_map<string, string> getRegularExpressionsMap(const vector<string>& regularExpressions);
unordered_map<string, int> getPriorityMap(const vector<string>& lines);
unordered_map<string, int> getPriorityMap(vector<string> regularExpressions, vector<string> keywords, vector<char> punctuations);
vector<string> getRegularExpressions(const vector<string>& lines);
#endif //COMPILERGENERATOR_PARSER_H
