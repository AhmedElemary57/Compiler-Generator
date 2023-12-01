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
            automaton.getStartNode()->addNextNode(automaton.getFinalNode(), char(238));
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


Automaton plusOperation(Automaton automaton) {
    /**
     * This function generates an automaton that represents the plus operation on an automaton.
     * @param automaton: an automaton.
     * @return an automaton that represents the plus operation on the automaton.
     */
    Automaton newAutomaton;
    // define a new automaton that represents the plus operation on the automaton.
    newAutomaton.setStartNode(new Node());
    newAutomaton.setFinalNode(new Node());

    // define the next state of the start node of the new automaton, which is the start node of the automaton.
    newAutomaton.getStartNode()->addNextNode(automaton.getStartNode(), char(238)); // 238 is the epsilon input.

    automaton.getFinalNode()->addNextNode(newAutomaton.getFinalNode(), char(238));
    automaton.getFinalNode()->addNextNode(automaton.getStartNode(), char(238));

    return newAutomaton;
}

Automaton closure(const Automaton& automaton) {
    /**
     * This function generates an automaton that represents the closure operation on an automaton.
     * @param automaton: an automaton.
     * @return an automaton that represents the closure operation on the automaton.
     */

    Automaton newAutomaton = positiveClosure(automaton);
    newAutomaton.getStartNode()->addNextNode(newAutomaton.getFinalNode(), char(238));

    return newAutomaton;
}

Automaton union_op(Automaton& automaton1, Automaton& automaton2) {
    /**
     * This function generates an automaton that represents the union operation on two automata.
     * @param automaton1: an automaton.
     * @param automaton2: an automaton.
     * @return an automaton that represents the union operation on the two automata.
     */
    Automaton newAutomaton;
    newAutomaton.setStartNode(new Node());
    newAutomaton.setFinalNode(new Node());

    newAutomaton.getStartNode()->addNextNode(automaton1.getStartNode(), char(238));
    newAutomaton.getStartNode()->addNextNode(automaton2.getStartNode(), char(238));

    automaton1.getFinalNode()->addNextNode(newAutomaton.getFinalNode(), char(238));
    automaton2.getFinalNode()->addNextNode(newAutomaton.getFinalNode(), char(238));

    return newAutomaton;
}

Automaton concatenate(Automaton& automaton1, Automaton& automaton2) {
    /**
     * This function generates an automaton that represents the concatenation operation on two automata.
     * @param automaton1: an automaton.
     * @param automaton2: an automaton.
     * @return an automaton that represents the concatenation operation on the two automata.
     */
    Automaton newAutomaton = *new Automaton(automaton1);
    newAutomaton.getFinalNode()->addNextNode(automaton2.getStartNode(), char(238));
    newAutomaton.setFinalNode(automaton2.getFinalNode());
    return newAutomaton;
}

void handleRegularDefinitionsInTermsOfOtherRegularDefinitions(unordered_map<string, vector<string>>& regularDefinitionsMap, unordered_map<string, Automaton>& automatonMap) {
    /**
     * This function handles the regular definitions that are defined in terms of other regular definitions.
     * @param regularDefinitionsMap: a map of strings to vectors of strings that represents the regular definitions.
     * @param automatonMap: a map of strings to automata that represents the regular definitions.
     */
    for (auto & i : regularDefinitionsMap) {
        for (auto & j : i.second) {
            if (j.find('+') != string::npos) {
                string regularDefinitionName = j.substr(0, j.length() - 1); // get the name of the regular definition
                vector<string> regularDefinition = regularDefinitionsMap[regularDefinitionName];
                Automaton oldAutomaton = automatonMap[regularDefinitionName];
                automatonMap[i.first] = positiveClosure(oldAutomaton);
                regularDefinitionsMap[i.first].insert(regularDefinitionsMap[i.first].end(), regularDefinition.begin(), regularDefinition.end());
            }
            else if (j.find('*') != string::npos) {
                string regularDefinitionName = j.substr(1, j.length() - 2); // get the name of the regular definition
                vector<string> regularDefinition = regularDefinitionsMap[regularDefinitionName];
                Automaton oldAutomaton = automatonMap[regularDefinitionName];
                automatonMap[i.first] = closure(oldAutomaton);
                regularDefinitionsMap[i.first].insert(regularDefinitionsMap[i.first].end(), regularDefinition.begin(), regularDefinition.end());
            }
        }
    }
}

// function to generate the automaton from the regular definitions map.
unordered_map<string, Automaton> generateAutomatonFromRegularDefinitions(unordered_map<string, vector<string>>& regularDefinitionsMap) {
    /**
     * This function generates an automaton map from a regular definitions map.
     * @param regularDefinitionsMap: a map of strings to vectors of strings that represents the regular definitions.
     * @return a map of strings to automata that represents the regular definitions.
     */
    unordered_map<string, Automaton> automatonMap;
    for (auto & i : regularDefinitionsMap) {
        automatonMap[i.first] = generateAutomatonFromRegularDefinition(i.second);
    }

    // handle the regular definitions that are defined in terms of other regular definitions.
    handleRegularDefinitionsInTermsOfOtherRegularDefinitions(regularDefinitionsMap,  automatonMap);

    return automatonMap;
}

unordered_map<string, Automaton> generateAutomatonFromRegularExpressions(unordered_map<string, string>& regularExpressionsMap, unordered_map<string, Automaton>& regularDefinitionsAutoMap) {
    /**
     * This function generates an automaton map from regular expressions.
     * @param regularExpressionsMap: a map of strings to regular expressions.
     * @param regularDefinitionsAutoMap: a map of strings to automata that represents the regular definitions.
     * @return a map of strings to automata that represents the regular expressions.
     */
    unordered_map<string, Automaton> automatonMap;
    for (auto & i : regularExpressionsMap) {
        Postfix_expression postfixExpression;
        string  regularExpressions = i.second;
        Automaton newAutomaton = postfixExpression.postfix(regularExpressions, regularDefinitionsAutoMap);
        automatonMap[i.first] = newAutomaton;
    }

    return automatonMap;
}

void printAutomatonMap(const unordered_map<string, Automaton>& automatonMap) {
    /**
     * This function prints information about the automaton map.
     * @param automatonMap: a map of strings to automata.
     */
    cout << "Automaton Map:" << endl;

    for (const auto& entry : automatonMap) {
        const string& automatonName = entry.first;
        Automaton automaton = entry.second;

        cout << "Automaton Name: " << automatonName << endl;
        cout << "Start Node: " << automaton.getStartNode()->getNodeNumber() << endl;
        cout << "Final Node: " << automaton.getFinalNode()->getNodeNumber() << endl;

        // Optionally, you can print more information about the automaton, depending on your needs.

        cout << endl;
    }
}
