#include <string>
#include <vector>
#include <unordered_map>

using namespace std;
#ifndef COMPILERGENERATOR_PARSER_H
#define COMPILERGENERATOR_PARSER_H

vector<string> readInputFile(const string& filepath);
unordered_map<int, vector<string>> getRegularExpressionsAndDefinition(const vector<string>& lines);

#endif //COMPILERGENERATOR_PARSER_H
