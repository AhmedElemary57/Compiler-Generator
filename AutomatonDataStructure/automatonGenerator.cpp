#include "automatonGenerator.h"
#include "../Postfix-expression/Postfix_expression.h"

using namespace std;

// function to generate the automaton from the regular definitions map.
unordered_map<string, Automaton> generateAutomatonFromRegularDefinitions(unordered_map<string, string>& regularDefinitionsMap) {
    /**
     * This function generates an automaton map from a regular definitions map.
     * @param regularDefinitionsMap: a map of strings to vectors of strings that represents the regular definitions.
     * @return a map of strings to automata that represents the regular definitions.
     */
    unordered_map<string, Automaton> automatonMap;

    for (auto & i : regularDefinitionsMap) {
        string regularDefinition = i.second;
        Automaton newAutomaton = postfix(regularDefinition, automatonMap);
        automatonMap[i.first] = newAutomaton;
    }

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
        string  regularExpressions = i.second;
        Automaton newAutomaton = postfix(regularExpressions, regularDefinitionsAutoMap);
        automatonMap[i.first] = newAutomaton;
    }

    return automatonMap;
}

unordered_map<string, Automaton> generateAutomatonFromKeyWords(const vector<string>& keywordsVector) {
    /**
     * This function generates an automaton map from keywords.
     * @param keywordsVector: a vector of strings that represents the keywords.
     * @return a map of strings to automata that represents the keywords.
     */

    unordered_map<string, Automaton> automatonMap;
    for (auto & i : keywordsVector) {
        Automaton newAutomaton;
        newAutomaton.setStartNode(new Node());
        newAutomaton.setFinalNode(new Node());
        newAutomaton.getStartNode()->addNextNode(newAutomaton.getFinalNode(), i[0]);

        for (int j = 1; j < i.length(); j++) {
            Node* tempNode = new Node();
            newAutomaton.getFinalNode()->addNextNode(tempNode, i[j]);
            newAutomaton.setFinalNode(tempNode);
        }

        automatonMap[i] = newAutomaton;
    }

    return automatonMap;
}
Automaton generateAutomatonFromPunctuations(const vector<char>& keywordsVector){
    Automaton punctuationsAutomaton;
    punctuationsAutomaton.setStartNode(new Node());
    punctuationsAutomaton.setFinalNode(new Node());
    for (auto & i : keywordsVector) {
        punctuationsAutomaton.getStartNode()->addNextNode(punctuationsAutomaton.getFinalNode(), i);
    }
    return punctuationsAutomaton;
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

Automaton positiveClosure(Automaton automaton) {
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

    automaton1.getFinalNode()->setIsFinal(false);
    automaton2.getFinalNode()->setIsFinal(false);

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