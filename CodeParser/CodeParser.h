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
    string outputFilePath;
    ofstream outFile;

    void showDrivationOutput(string newProduction, int lengthToRemove, bool isTerminal);

public:
    CodeParser(LexicalAnalyzer *lexicalAnalyzer, Table *table, string startNonTerminalName, string outputFilePath);
    void startDrivation();
};

#endif //COMPILERGENERATOR_CODEPARSER_H
