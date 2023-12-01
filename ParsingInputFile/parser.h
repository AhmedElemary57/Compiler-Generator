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
vector<string> getPunctuations(const vector<string>& lines);
set<char> getReservedSymbols(const vector<string>& lines);
unordered_map<string, vector<string> > getRegularDefinitionsMap(const vector<string>& regularDefinitions);
unordered_map<string, string> getRegularExpressionsMap(const vector<string>& regularExpressions);

#endif //COMPILERGENERATOR_PARSER_H
