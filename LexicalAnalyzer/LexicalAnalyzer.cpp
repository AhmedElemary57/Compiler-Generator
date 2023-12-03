#include "LexicalAnalyzer.h"
#include "../AutomatonDataStructure/CombinedAutomaton.h"


using namespace std;

LexicalAnalyzer::LexicalAnalyzer(CombinedAutomaton *DFA, string program)
{

    this->DFA = DFA;
    program.erase(remove(program.begin(), program.end(), ' '), program.end());
    cout << program << endl;
    this->program = program;
    
    this->current_index = 0;
}

bool LexicalAnalyzer::hasNext()
{
    return current_index != program.size();
}

pair<string, string> LexicalAnalyzer::getNextToken()
{

    if (current_index == program.size())
    {
        return {"", ""};
    }

    Node *start = DFA->getStartNode();
    unordered_map<Node *, string> finalNodesMap = DFA->getFinalNodesMap();

    Node *accepted_node = nullptr;
    string accepted_token = "";
    int accepted_index = -1;

    unordered_map<char, vector<Node *>> nextNodes = start->getNextNodes();
    auto iterator = nextNodes.find(program[current_index]);
    Node *nextNode = nullptr;
    string current_token = "";

    if (iterator != nextNodes.end())
    {
        nextNode = iterator->second[0];
    }
    else
    {
        nextNode = nullptr;
    }
    current_token += program[current_index];
    current_index++;

    while (nextNode != nullptr && current_index < program.size())
    {

        auto it = finalNodesMap.find(nextNode);

        if (it != finalNodesMap.end())
        {
            accepted_node = nextNode;
            accepted_token = current_token;
            accepted_index = current_index;
        }

        unordered_map<char, vector<Node *>> nextNodes = nextNode->getNextNodes();
        iterator = nextNodes.find(program[current_index]);

        if (iterator != nextNodes.end())
        {
            nextNode = iterator->second[0];
            current_token += program[current_index];
            current_index++;
        }
        else
        {
            nextNode = nullptr;
        }
        
    }

    auto it = finalNodesMap.find(nextNode);

    if (it != finalNodesMap.end())
    {
        accepted_node = nextNode;
        accepted_token = current_token;
        accepted_index = current_index;
    }

    if (accepted_node == nullptr)
    {
        cout << "Error string " << current_token << " didn't match any RE" << endl;
        return {"", ""};
    }
    else
    {
        current_index = accepted_index;
        return {finalNodesMap[accepted_node], accepted_token};
    }
}

vector<pair<string, string>> LexicalAnalyzer::getAllTokens()
{
    vector<pair<string, string>> tokens;
    while (this->hasNext())
    {
        pair<string, string> p = this->getNextToken();
        if (p.first != "")
        {
            tokens.push_back(p);
        }
    }
    return tokens;
}


void LexicalAnalyzer::print_tokens(vector<pair<string, string>> tokens){
    for (int i=0; i<tokens.size(); i++){
        cout << tokens[i].first << " " << tokens[i].second << endl;
    }
}