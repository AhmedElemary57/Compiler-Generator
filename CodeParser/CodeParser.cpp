#include "CodeParser.h"

CodeParser::CodeParser(LexicalAnalyzer *lexicalAnalyzer, Table *table, string startNonTerminalName, string outputFilePath)
{
    this->lexicalAnalyzer = lexicalAnalyzer;
    this->table = table;
    this->startNonTerminalName = startNonTerminalName;
    this->outputFilePath = outputFilePath;
    outFile.open(outputFilePath);
    if (!outFile.is_open())
    {
        cout << "Error opening the file" << endl;
        return;
    }
    outFile.close();
}

void CodeParser::showDrivationOutput(string newProduction, int lengthToRemove, bool isTerminal)
{
    outFile.open(outputFilePath, ios::app);
    if (!outFile.is_open())
    {
        cout << "Error opening the file" << endl;
        return;
    }
    static string left;
    static string right;
    if (isTerminal)
        left += right.substr(0, lengthToRemove) + " ";
    right.erase(0, lengthToRemove == right.length() ? lengthToRemove : lengthToRemove + 1);
    right = newProduction + right;
    outFile << left << right << endl;
    outFile.close();
}

void CodeParser::startDrivation()
{
    stack<pair<string, bool>> drivationStack;
    drivationStack.push({startNonTerminalName, false});
    showDrivationOutput(startNonTerminalName, 0, false);
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
            {
                drivationStack.pop();
                string newProduction = "";
                for (int i = entry.second.size() - 1; i >= 0; i--)
                {
                    drivationStack.push({entry.second[i]->getName(), entry.second[i]->isTerminal()});
                    newProduction = entry.second[i]->getName() + " " + newProduction;
                }
                cout << top.first << " --> " << newProduction << endl;
                showDrivationOutput(newProduction, top.first.length(), false);
                break;
            }

            case Epsilon:
                cout << top.first << " --> \\L" << endl;
                drivationStack.pop();
                showDrivationOutput("", top.first.length(), false);
                break;

            case Sync:
                cout << "Error: illegal: \'" << top.first << "\', discarded nonterminal: \'" << top.first << "\'" << endl;
                drivationStack.pop();
                showDrivationOutput("", top.first.length(), false);
                break;

            default:
                cout << "Error: illegal: \'" << top.first << "\', discarded token: \'" << currentToken << "\'" << endl;
                currentToken = lexicalAnalyzer->getNextToken().first;
                break;
            }
        }
        else
        {
            drivationStack.pop();
            if (top.first == currentToken)
            {
                cout << "Match: \'" << currentToken << "\'" << endl;
                currentToken = lexicalAnalyzer->getNextToken().first;
            }
            else
            {
                cout << "Error: missing token: \'" << top.first << "\', inserted" << endl;
            }
            showDrivationOutput("", top.first.length(), true);
        }
    }
}
