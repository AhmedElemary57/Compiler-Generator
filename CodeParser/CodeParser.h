#ifndef COMPILERGENERATOR_CODEPARSER_H
#define COMPILERGENERATOR_CODEPARSER_H

#include "../LexicalAnalyzer/LexicalAnalyzer.h"
#include "../table/Table.h"

class CodeParser
{
private:
    LexicalAnalyzer *lexicalAnalyzer;
    Table *table;
    string startNonTerminalName;

public:
    CodeParser(LexicalAnalyzer *lexicalAnalyzer, Table *table, string startNonTerminalName);
    void performDrivation(string outputFilePath);
};

#endif //COMPILERGENERATOR_CODEPARSER_H
