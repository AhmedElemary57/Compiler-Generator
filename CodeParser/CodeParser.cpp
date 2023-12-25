#include "CodeParser.h"

CodeParser::CodeParser(LexicalAnalyzer *lexicalAnalyzer, Table *table, string startNonTerminalName)
{
    this->lexicalAnalyzer = lexicalAnalyzer;
    this->table = table;
    this->startNonTerminalName = startNonTerminalName;
}

void CodeParser::performDrivation(string outputFilePath)
{
    ofstream outFile(outputFilePath);
    if (!outFile.is_open())
    {
        cout << "Error opening the file" << endl;
        return;
    }
    stack<pair<string, bool>> drivationStack;
    drivationStack.push({startNonTerminalName, false});
    string currentToken = lexicalAnalyzer->getNextToken().first;
    while (lexicalAnalyzer->hasNext() || !drivationStack.empty())
    {
        if (!lexicalAnalyzer->hasNext())
        {
            currentToken = "$";
        }
        else if (drivationStack.empty())
        {
            drivationStack.push({startNonTerminalName, false});
        }
        else if ((currentToken.length() == 1 && isspace(currentToken[0])) || currentToken.length() == 0)
        {
            currentToken = lexicalAnalyzer->getNextToken().first;
            continue;
        }
        pair<string, bool> top = drivationStack.top();
        if (!top.second)
        {
            pair<return_t, std::vector<CFGEntry *>> entry = table->getProduction(top.first, currentToken);
            switch (entry.first)
            {
            case Production:
                drivationStack.pop();
                for (int i = entry.second.size() - 1; i >= 0; i--)
                    drivationStack.push({entry.second[i]->getName(), entry.second[i]->isTerminal()});
                break;
            
            case Epsilon:
                drivationStack.pop();
                break;

            case Sync:
                drivationStack.pop();
                break;

            default:
                currentToken = lexicalAnalyzer->getNextToken().first;
                break;
            }
        }
        else
        {
            drivationStack.pop();
            if (top.first == currentToken)
            {
                currentToken = lexicalAnalyzer->getNextToken().first;
            }
        }
    }
}
