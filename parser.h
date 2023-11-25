#include <string>
#include <vector>
#include <unordered_map>

using namespace std;
#ifndef COMPILERGENERATOR_PARSER_H
#define COMPILERGENERATOR_PARSER_H

vector<string> readInputFile(const string& filepath);
vector<string> getRegularExpressions(const vector<string>& lines);
vector<string> getRegularDefinitions(const vector<string>& lines);
vector<string> getKeywords(const vector<string>& lines);
vector<string> getPunctuations(const vector<string>& lines);

#endif //COMPILERGENERATOR_PARSER_H
