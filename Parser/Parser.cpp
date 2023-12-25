#include "Parser.h"

Parser::Parser(LexicalAnalyzer *lexicalAnalyzer, Table *table)
{
    this->lexicalAnalyzer = lexicalAnalyzer;
    this->table = table;
}