#include "../AutomatonDataStructure/CombinedAutomaton.h"


#ifndef COMPILERGENERATOR_LEXICALANALYZER_H
#define COMPILERGENERATOR_LEXICALANALYZER_H


class LexicalAnalyzer
{
private:
    CombinedAutomaton *DFA;
    string program;
    int current_index;
public:
    LexicalAnalyzer(CombinedAutomaton *DFA, string program);
    pair<string, string> getNextToken();
    vector<pair<string, string>> getAllTokens();
    bool hasNext();
    void print_tokens(vector<pair<string, string>> tokens);
};



#endif