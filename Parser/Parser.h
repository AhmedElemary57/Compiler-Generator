#ifndef COMPILERGENERATOR_PARSER_H
#define COMPILERGENERATOR_PARSER_H

#include "../LexicalAnalyzer/LexicalAnalyzer.h"
#include "../table/Table.h"

class Parser
{
private:
    LexicalAnalyzer *lexicalAnalyzer;
    Table *table;

public:
    Parser(LexicalAnalyzer *lexicalAnalyzer, Table *table);
};

#endif //COMPILERGENERATOR_PARSER_H
