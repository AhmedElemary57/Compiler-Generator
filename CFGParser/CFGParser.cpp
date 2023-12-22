#include "CFGParser.h"

const string whiteSpaces = " \t\v\r\n";

string getTrimmed(string &input)
{
    size_t left = input.find_first_not_of(whiteSpaces);
    size_t right = input.find_last_not_of(whiteSpaces);
    return left == right ? "" : input.substr(left, right - left + 1);
}

pair<int, string> getWord(string &input, int start)
{
    size_t left = input.find_first_not_of(whiteSpaces);
    if (left == string::npos)
        return {-1, ""};
    size_t right = input.find_first_of(whiteSpaces, left + 1);
    if (left >= right - 1)
        return {-1, ""};
    else if (right == string::npos)
        return {input.length(), input.substr(left)};
    return {right - 1, input.substr(left, right - left)};
}

vector<string> readCFG(const string &filePath)
{
    ifstream file(filePath);
    if (!file.is_open())
    {
        std::cerr << "Error opening the file.\n";
        exit(1);
    }
    string line;
    vector<string> lines;
    while (getline(file, line))
        lines.push_back(getTrimmed(line));
    file.close();
    return lines;
}

void extractAddProductions(string line,
                           unordered_map<string, NonTerminal *> &namesNonTerminalsMap,
                           unordered_map<string, Terminal *> &namesTerminalsMap,
                           string &nonTerminalName)
{
    line += " ";
    string word = "";
    bool isTerminal = false;
    vector<CFGEntry *> production;
    for (size_t i = 0; i < line.length(); i++)
    {
        if (isspace(line[i]) || line[i] == '|')
        {
            if (word == "")
            {
                if ((line[i] == '|' || (i == line.length() - 1)) && production.size() != 0)
                {
                    namesNonTerminalsMap[nonTerminalName]->addProduction(production);
                    production = vector<CFGEntry *>();
                }
                continue;
            }
            else if (word == "L" && i > 1 && line[i - 2] == '\\')
            {
                if ((line[i] == '|' || (i == line.length() - 1)) && production.size() == 0)
                    namesNonTerminalsMap[nonTerminalName]->hasEpsilonProduction = true;
            }
            else if (isTerminal)
            {
                if (namesTerminalsMap.find(word) == namesTerminalsMap.end())
                    namesTerminalsMap[word] = new Terminal(word);
                production.push_back(namesTerminalsMap[word]);
            }
            else
            {
                if (namesNonTerminalsMap.find(word) == namesNonTerminalsMap.end())
                    namesNonTerminalsMap[word] = new NonTerminal(word);
                production.push_back(namesNonTerminalsMap[word]);
            }
            if ((line[i] == '|' || (i == line.length() - 1)) && production.size() != 0)
            {
                namesNonTerminalsMap[nonTerminalName]->addProduction(production);
                production = vector<CFGEntry *>();
            }
            isTerminal = false;
            word = "";
        }
        else if (line[i] == '\\')
        {
            i++;
            if (i < line.length())
                word += line[i];
        }
        else if (line[i] == '\'')
        {
            isTerminal = true;
        }
        else
        {
            word += line[i];
        }
    }
}

CFG parseCFG(const string filePath)
{
    vector<string> nonTerminalsNames;
    unordered_map<string, NonTerminal *> namesNonTerminalsMap;
    unordered_map<string, Terminal *> namesTerminalsMap;
    vector<string> lines = readCFG(filePath);
    for (string &line : lines)
    {
        if (line == "")
            continue;
        int start = 0;
        string nonTerminalName;
        if (line[0] == '#')
        {
            start = line.find('=') + 1;
            nonTerminalName = line.substr(1, start - 2);
            nonTerminalName = getTrimmed(nonTerminalName);
            nonTerminalsNames.push_back(nonTerminalName);
            if (namesNonTerminalsMap.find(nonTerminalName) == namesNonTerminalsMap.end())
                namesNonTerminalsMap[nonTerminalName] = new NonTerminal(nonTerminalName);
        }
        else
        {
            nonTerminalName = nonTerminalsNames.back();
        }
        extractAddProductions(line.substr(start), namesNonTerminalsMap, namesTerminalsMap, nonTerminalName);
    }
    return CFG(nonTerminalsNames, namesNonTerminalsMap);
}