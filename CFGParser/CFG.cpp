#include "CFG.h"

CFGEntry::CFGEntry(string name)
{
    this->name = name;
}

string CFGEntry::getName()
{
    return this->name;
}

bool CFGEntry::isTerminal(){
    return false;
}

Terminal::Terminal(string name) : CFGEntry(name) {}

bool Terminal::isTerminal()
{
    return true;
}

NonTerminal::NonTerminal(string name) : CFGEntry(name) {
    this->hasEpsilonProduction = false;
}

bool NonTerminal::isTerminal()
{
    return false;
}

void NonTerminal::addProduction(vector<CFGEntry *> production)
{
    this->productions.push_back(production);
}

void NonTerminal::setProductions(vector<vector<CFGEntry *>> productions)
{
    this->productions.clear();
    this->productions = productions;
}

vector<vector<CFGEntry *>> NonTerminal::getProductions()
{
    return this->productions;
}
CFG::CFG(vector<string> &nonTerminalsNames, unordered_map<string, NonTerminal *> namesNonTerminalsMap)
{
    this->nonTerminalsNames = nonTerminalsNames;
    this->namesNonTerminalsMap = namesNonTerminalsMap;
}

vector<CFGEntry *> string_to_vector(string s){
    
    vector<CFGEntry *> newEntries;
    for (int j=0; j<s.size(); j++){
        string c(1, s[j]);
        CFGEntry *cfgEntry = new CFGEntry(c);
        newEntries.push_back(cfgEntry);
    }
    return newEntries;
}

vector<string> CFG::build_string_from_production(vector<vector<CFGEntry *>> prod, vector<int> indices){
    vector<string> re;
    for (int i=0; i<indices.size(); i++){
        string s = "";
        int index = indices[i];
        for (int j=0; j<prod[index].size(); j++){
            s += prod[index][j]->getName();
        }
        re.push_back(s);
    }
    return re;
}

void CFG::non_immediate_left_recursion_elimination(int i, int j){
    string first_name = this->nonTerminalsNames[i];
    string second_name = this->nonTerminalsNames[j];
    auto it = this->namesNonTerminalsMap.find(first_name);
    if(it == this->namesNonTerminalsMap.end()){
        return;
    }
    NonTerminal *A = it->second;

    it = this->namesNonTerminalsMap.find(second_name);
    if(it == this->namesNonTerminalsMap.end()){
        return;
    }
    // vector<vector<CFGEntry *>>
    NonTerminal *B = it->second;

    vector<vector<CFGEntry *>> A_prod = A->getProductions();
    vector<vector<CFGEntry *>> B_prod = B->getProductions();
    vector<vector<CFGEntry *>> new_A_prod;
    for (int k=0; k<A_prod.size(); k++){
        if (A_prod[k][0]->getName() == B->getName()){
            
            for (int w=0; w<B_prod.size(); w++){
                vector<CFGEntry*> newEntries;
                for (int z=0; z<B_prod[w].size(); z++){
                    CFGEntry *cfgEntry = new CFGEntry(B_prod[w][z]->getName());
                    newEntries.push_back(cfgEntry);
                }
                for (int z=1; z<A_prod[k].size(); z++){
                    CFGEntry *cfgEntry = new CFGEntry(A_prod[k][z]->getName());
                    newEntries.push_back(cfgEntry);
                }
                new_A_prod.push_back(newEntries);
            }
        }
        else{
            new_A_prod.push_back(A_prod[k]);
        }
    }
    A->setProductions(new_A_prod);

}

void CFG::immediate_left_recursion_elimination(int i){
    vector<vector<CFGEntry*>> alphas;
    vector<vector<CFGEntry*>> betas;
    
    auto it = this->namesNonTerminalsMap.find(this->nonTerminalsNames[i]);
    if(it == this->namesNonTerminalsMap.end()){
        return;
    }
    NonTerminal *A = it->second;
    vector<vector<CFGEntry *>> A_prod = A->getProductions();
    for (int k=0; k<A_prod.size(); k++){
        if (A_prod[k][0]->getName() == A->getName()){
            vector<CFGEntry*> newEntries;
            for(int w=1; w<A_prod[k].size(); w++){
                CFGEntry *cfgEntry = new CFGEntry(A_prod[k][w]->getName());
                newEntries.push_back(cfgEntry);
            }
            alphas.push_back(newEntries);
        }
        else{
            vector<CFGEntry*> newEntries;
            for(int w=0; w<A_prod[k].size(); w++){
                CFGEntry *cfgEntry = new CFGEntry(A_prod[k][w]->getName());
                newEntries.push_back(cfgEntry);
            }
            betas.push_back(newEntries);
            
        }
    }
    if(alphas.size() == 0){
        return;
    }

    string new_non_terminal = A->getName() + "'";
    vector<vector<CFGEntry *>> new_A_prod;
    vector<vector<CFGEntry *>> new_A_dash_prod;
    if(betas.size() == 0){
        new_A_prod.push_back({new CFGEntry(new_non_terminal)});
    }

    for (int k=0; k<betas.size(); k++){
        CFGEntry *cfgEntry = new CFGEntry(new_non_terminal);
        betas[k].push_back(cfgEntry);
        new_A_prod.push_back(betas[k]);
    }

    
    for (int k=0; k<alphas.size(); k++){
        CFGEntry *cfgEntry = new CFGEntry(new_non_terminal);
        alphas[k].push_back(cfgEntry);
        new_A_dash_prod.push_back(alphas[k]);
    }
    this->nonTerminalsNames.push_back(new_non_terminal);
    NonTerminal *no = new NonTerminal(new_non_terminal);
    this->namesNonTerminalsMap.insert({new_non_terminal, no});
    no->setProductions(new_A_dash_prod);
    no->hasEpsilonProduction = true;
    A->setProductions(new_A_prod);

}

void CFG::left_recursion_elimination(){
    int n = this->nonTerminalsNames.size();
    for (int i=0; i<n; i++){
        for (int j=0; j<i; j++){
            non_immediate_left_recursion_elimination(i, j);
        }
        immediate_left_recursion_elimination(i);
    }
}

string longestCommonPrefix(vector<string> strings) {
    if (strings.empty()) {
        return "";
    }

    
    int minLen = min_element(strings.begin(), strings.end(),
        [](const string& a, const string& b) {
            return a.length() < b.length();
        })->length();

    
    for (int i = 0; i < minLen; ++i) {
        char currentChar = strings[0][i];
        for (int j = 1; j < strings.size(); ++j) {
            if (strings[j][i] != currentChar) {
                return strings[0].substr(0, i);
            }
        }
    }

    
    return strings[0].substr(0, minLen);
}

void CFG::left_factor_non_terminal(NonTerminal *A, string name){
    
    
    vector<vector<CFGEntry *>> A_prod = A->getProductions();
    
    
    unordered_map<string, pair<int, vector<int>>> count;
    for (int i=0; i<A_prod.size(); i++){
        auto it = count.find(A_prod[i][0]->getName());
        if (it == count.end()){
            count.insert({A_prod[i][0]->getName(), {1, {i}}});
        }
        else{
            it->second.first++;
            it->second.second.push_back(i);
        }
    }
    string new_non_terminal_name = name;
    vector<NonTerminal *> nonTerminals;
    vector<vector<CFGEntry*>> new_A_prod;
    for (int i=0; i<A_prod.size(); i++){
        pair<int, vector<int>> c;
        auto it = count.find(A_prod[i][0]->getName());
        c = it->second;
        if (c.first == 1){
            new_A_prod.push_back(A_prod[i]);
        }
        else if(c.first == -1){
            continue;
        }
        else{
            vector<vector<CFGEntry *>> A_dash_prod;
            vector<string> strings = build_string_from_production(A_prod, c.second);
            string pre = longestCommonPrefix(strings);
            vector<CFGEntry *> newEntries = string_to_vector(pre);
            CFGEntry *cfgEntry = new CFGEntry(new_non_terminal_name);
            newEntries.push_back(cfgEntry);
            new_A_prod.push_back(newEntries);

            NonTerminal *no = new NonTerminal(new_non_terminal_name);
            no->hasEpsilonProduction = false;
            nonTerminals.push_back(no);
            this->nonTerminalsNames.push_back(new_non_terminal_name);
            this->namesNonTerminalsMap.insert({new_non_terminal_name, no});
            new_non_terminal_name += "'";
            
            for (int j=0; j<strings.size(); j++){
                string left_over = strings[j].substr(pre.size());
                if(left_over.size() == 0){
                    no->hasEpsilonProduction = true;
                    continue;
                }
                vector<CFGEntry *> new_A_dash_entries = string_to_vector(left_over);
                A_dash_prod.push_back(new_A_dash_entries);
            }
            no->setProductions(A_dash_prod);
        }
        it->second.first = -1; 
    }
    A->setProductions(new_A_prod);
    for (int i=0; i<nonTerminals.size(); i++){
        left_factor_non_terminal(nonTerminals[i], new_non_terminal_name);
    }

}

void CFG::left_factoring(){
    int n = this->nonTerminalsNames.size();
    for (int i=0; i<n; i++){
        auto it = this->namesNonTerminalsMap.find(this->nonTerminalsNames[i]);
        if(it == this->namesNonTerminalsMap.end()){
            return;
        }
        NonTerminal *A = it->second;
        left_factor_non_terminal(A, A->getName() + "'");
    }
}

void CFG::print_productions(){
    for (int i=0; i<this->nonTerminalsNames.size(); i++){
        string first_name = this->nonTerminalsNames[i];
        auto it = this->namesNonTerminalsMap.find(first_name);
        if(it == this->namesNonTerminalsMap.end()){
            return;
        }
        NonTerminal *A = it->second;
        vector<vector<CFGEntry *>> A_prod = A->getProductions();
        string prod = A->getName();
        prod += " -> ";
        for (int j=0; j<A_prod.size(); j++){
            for (int k=0; k<A_prod[j].size(); k++){
                prod += A_prod[j][k]->getName();
            }
            if (j != A_prod.size() -1 ){
                prod += " | ";
            }
        }
        if(A->hasEpsilonProduction){
            prod += " | epsilon";
        }
        cout << prod << endl;
    }
    
}