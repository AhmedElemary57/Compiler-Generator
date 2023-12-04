#include "../AutomatonDataStructure/CombinedAutomaton.h"


#ifndef COMPILERGENERATOR_LEXICALANALYZER_H
#define COMPILERGENERATOR_LEXICALANALYZER_H


class LexicalAnalyzer
{
private:
    CombinedAutomaton *DFA;
    string program;
    int current_index;
    vector<pair<string, string>> tokens;
    unordered_set<string> symbol_table;
public:
    LexicalAnalyzer(CombinedAutomaton *DFA, string program);
    pair<string, string> getNextToken();
    vector<pair<string, string>> getAllTokens();
    bool hasNext();
    void print_tokens(vector<pair<string, string>> tokens);
    void print_tokens();
    void write_tokens(string path);
    void write_tokens();
    void print_symbol_table();
};



#endif