#include "automatonGenerator.h"

using namespace std;



Automaton generateAutomatonFromRegularDefinition(vector<string> regularDefinition) {
    /**
     * This function generates an automaton from a regular definition.
     * @param regularDefinition: a vector of strings that represents the regular definition.
     * @return an automaton that represents the regular definition.
     */
    Automaton automaton;
    automaton.setStartNode(new Node());
    automaton.setFinalNode(new Node());

    for( auto & i : regularDefinition ) {

        if (i == "\\L") {
            automaton.getStartNode()->addNextNode(automaton.getFinalNode(), 238);
            continue;
        }

        // remove spaces from the regular definition
        i.erase(remove(i.begin(), i.end(), ' '), i.end());

        // if the regular definition is a single character
        if (i.length() == 1) {
            automaton.getStartNode()->addNextNode(automaton.getFinalNode(), i[0]);
            continue;
        }

        // if the regular definition is a range of characters
        if (i.length() == 3 && i[1] == '-') {
            for (char j = i[0]; j <= i[2]; j++) {
                automaton.getStartNode()->addNextNode(automaton.getFinalNode(), j);
            }
            continue;
        }

    }
    return automaton;
}

// function to generate the automaton from the regular definitions map.
unordered_map<string, Automaton> generateAutomatonFromRegularDefinitions(const unordered_map<string, vector<string>>& regularDefinitionsMap) {
    /**
     * This function generates an automaton map from a regular definitions map. eg. {letter:  +[a-zA-Z], digit: [0-9]} -> {letter: automaton, digit: automaton}
     * @param regularDefinitionsMap: a map of strings to vectors of strings that represents the regular definitions.
     * @return a map of strings to automata that represents the regular definitions.
     */
    unordered_map<string, Automaton> automatonMap;
    for (auto & i : regularDefinitionsMap) {

        automatonMap[i.first] = generateAutomatonFromRegularDefinition(i.second);
    }

    return automatonMap;
}

void printAutomatonMap(const unordered_map<string, Automaton>& automatonMap) {
    /**
     * This function prints the automaton map.
     * @param automatonMap: a map of strings to automata that represents the regular definitions.
     */
    for (auto & i : automatonMap) {
        cout << i.first << " : " << endl;
        Automaton x =i.second;
        x.printAutomaton();
    }
}
