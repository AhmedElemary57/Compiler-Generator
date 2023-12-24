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
    return true;
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

void NonTerminal::addTerminalToFollowSet(Terminal *terminal) {
    this->followSet.insert(terminal);
}

void NonTerminal::addSetToFollowSet(set<Terminal *> followSet) {
    this->followSet.insert(followSet.begin(), followSet.end());
}

void NonTerminal::setFollowSet(set<Terminal *> followSet) {
    this->followSet = followSet;

}

set<Terminal *> NonTerminal::getFollowSet() {
    return this->followSet;
}

CFG::CFG(vector<string> &nonTerminalsNames, unordered_map<string, NonTerminal *> namesNonTerminalsMap)
{
    this->nonTerminalsNames = nonTerminalsNames;
    this->namesNonTerminalsMap = namesNonTerminalsMap;
}

CFGEntry* CFG::create_entry(string name, bool terminal){
    if(terminal){
        Terminal *terminal = new Terminal(name);
        return terminal;
    }
    NonTerminal *no = new NonTerminal(name);
    NonTerminal *it = this->namesNonTerminalsMap[name];
    no->setProductions(it->getProductions());
    no->setHasEpsilonProduction(it->hasEpsilon());
    this->namesNonTerminalsMap[name] = no;
    return no;
}



string CFG::get_unique_non_terminal_name(string name){
    while(true){
        auto it = namesNonTerminalsMap.find(name);
        if (it == namesNonTerminalsMap.end()){
            return name;
        }
        name += "'";
    }
}

vector<vector<CFGEntry *>> CFG::build_string_from_production(vector<vector<CFGEntry *>> prod, vector<int> indices){
    
    vector<vector<CFGEntry *>> re;
    for (int i=0; i<indices.size(); i++){
        
        int index = indices[i];
        re.push_back(prod[index]);
        
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
                    
                    CFGEntry *cfgEntry = create_entry(B_prod[w][z]->getName(), B_prod[w][z]->isTerminal());
                    newEntries.push_back(cfgEntry);
                }
                for (int z=1; z<A_prod[k].size(); z++){
                    CFGEntry *cfgEntry = create_entry(A_prod[k][z]->getName(), A_prod[k][z]->isTerminal());
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
                
                CFGEntry *cfgEntry = create_entry(A_prod[k][w]->getName(), A_prod[k][w]->isTerminal());
                newEntries.push_back(cfgEntry);
            }
            alphas.push_back(newEntries);
        }
        else{
            vector<CFGEntry*> newEntries;
            for(int w=0; w<A_prod[k].size(); w++){
                
                CFGEntry *cfgEntry = create_entry(A_prod[k][w]->getName(), A_prod[k][w]->isTerminal());
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

    this->nonTerminalsNames.push_back(new_non_terminal);
    NonTerminal *no = new NonTerminal(new_non_terminal);
    this->namesNonTerminalsMap.insert({new_non_terminal, no});
    vector<CFGEntry *> new_A_dash_entries;
    new_A_dash_entries.push_back(new Terminal("\\L"));
    new_A_dash_prod.push_back(new_A_dash_entries);
    no->setProductions(new_A_dash_prod);
    no->setHasEpsilonProduction(true) ;
    CFGEntry* cfgEntry = create_entry(new_non_terminal, false);
    if(betas.size() == 0){
        
        new_A_prod.push_back({cfgEntry});
    }

    for (int k=0; k<betas.size(); k++){
        
        betas[k].push_back(cfgEntry);
        new_A_prod.push_back(betas[k]);
    }

    
    for (int k=0; k<alphas.size(); k++){
        
        alphas[k].push_back(cfgEntry);
        new_A_dash_prod.push_back(alphas[k]);
    }
    
    
    no = (NonTerminal*) cfgEntry;
    no->setProductions(new_A_dash_prod);
    
    A = this->namesNonTerminalsMap[A->getName()];
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

vector<CFGEntry*> CFG::findLongestCommonPrefix(vector<vector<CFGEntry*>> productions)
{
    vector<CFGEntry*> result;

    if (productions.empty())
        return result;

    size_t minLen = INT_MAX;
    
    for (int i=0; i<productions.size(); i++){
        minLen = min(minLen, productions[i].size());
    }

    for (int i=0; i<minLen; i++){
        CFGEntry *cfgEntry = productions[0][i];
        bool mismatch = false;
        for (const auto &prod : productions){
            if(cfgEntry->getName() != prod[i]->getName()){
                mismatch = true;
                break;
            }
        }
        if(mismatch){
            break;
        }
        result.push_back(this->create_entry(cfgEntry->getName(), cfgEntry->isTerminal()));
    }

    return result;
}
void CFG::left_factor_non_terminal(NonTerminal *A, string name){
    
    
    vector<vector<CFGEntry *>> A_prod = A->getProductions();
    
    bool leave = true;
    unordered_map<string, pair<int, vector<int>>> count;
    for (int i=0; i<A_prod.size(); i++){
        auto it = count.find(A_prod[i][0]->getName());
        if (it == count.end()){
            count.insert({A_prod[i][0]->getName(), {1, {i}}});
        }
        else{
            leave = false;
            it->second.first++;
            it->second.second.push_back(i);
        }
    }
    if(leave){
        return;
    }
    string new_non_terminal_name = get_unique_non_terminal_name(name);
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
            new_non_terminal_name = get_unique_non_terminal_name(new_non_terminal_name);
            vector<vector<CFGEntry *>> A_dash_prod;
            vector<vector<CFGEntry *>> strings = build_string_from_production(A_prod, c.second);
            vector<CFGEntry *> newEntries = findLongestCommonPrefix(strings);
            int taken_size = newEntries.size();
            
            NonTerminal *no = new NonTerminal(new_non_terminal_name);
            no->setHasEpsilonProduction(false);
            
            this->nonTerminalsNames.push_back(new_non_terminal_name);
            this->namesNonTerminalsMap.insert({new_non_terminal_name, no});

            CFGEntry *cfgEntry = create_entry(new_non_terminal_name, false);
            no = (NonTerminal *) cfgEntry;
            newEntries.push_back(cfgEntry);
            new_A_prod.push_back(newEntries);
            nonTerminals.push_back(no);
            
            
            
            for (int j=0; j<strings.size(); j++){
                vector<CFGEntry *> new_A_dash_entries;
                for(int k=taken_size; k<strings[j].size(); k++){
                   
                    CFGEntry *cfgEntry = create_entry(strings[j][k]->getName(), strings[j][k]->isTerminal()); 
                    new_A_dash_entries.push_back(cfgEntry);
                }
                if(new_A_dash_entries.size() == 0){
                    no->setHasEpsilonProduction(true);
                    new_A_dash_entries.push_back(new Terminal("\\L"));
                    A_dash_prod.push_back(new_A_dash_entries);

                }
                else{
                    A_dash_prod.push_back(new_A_dash_entries);
                }
            }
            no = this->namesNonTerminalsMap[new_non_terminal_name];
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

                
                if(A_prod[j][k]->isTerminal()){
                    prod += "'";
                    prod += A_prod[j][k]->getName();
                    prod+= "'";
                
                    prod += " ";
                }
                else{
                    prod += A_prod[j][k]->getName();
                    prod += " ";
                }
            }
            
            if (j != A_prod.size() -1 ){
                prod += " | ";
            }
        }
        cout << prod << endl;
        
    }
    
}

vector<string> CFG::get_non_terminals_names() {
    return this->nonTerminalsNames;
}

unordered_map<string, NonTerminal *> CFG::get_non_terminals_map() {
    return this->namesNonTerminalsMap;
}


