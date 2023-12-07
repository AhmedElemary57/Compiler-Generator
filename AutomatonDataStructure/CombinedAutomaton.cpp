//
// Created by USER on 12/2/2023.
//

#include "CombinedAutomaton.h"

CombinedAutomaton::CombinedAutomaton()
{
    this->startNode = nullptr;
    this->finalNodesMap = unordered_map<Node *, string>();
    this->priorityMap = unordered_map<string, int>();
}

CombinedAutomaton::CombinedAutomaton(Node *startNode, unordered_map<Node *, string> finalNodesMap, unordered_map<string, int> priorityMap)
{
    this->startNode = startNode;
    this->finalNodesMap = finalNodesMap;
    this->priorityMap = priorityMap;
}

void CombinedAutomaton::setStartNode(Node *startNode)
{
    this->startNode = startNode;
}

void CombinedAutomaton::setFinalNodesMap(unordered_map<Node *, string> finalNodesMap)
{
    this->finalNodesMap = finalNodesMap;
}

Node *CombinedAutomaton::getStartNode()
{
    return this->startNode;
}

unordered_map<Node *, string> CombinedAutomaton::getFinalNodesMap()
{
    return this->finalNodesMap;
}

void CombinedAutomaton::generateCombinedAutomaton(unordered_map<string, Automaton> &automatonMap)
{
    /***
     * This function generates the combined automaton from the automaton map
     * @param automatonMap: the map of the automata generated from the regular expressions
     * @return: the combined automaton
     */
    Node *tempStartNode = new Node();
    unordered_map<Node *, string> tempFinalNodesMap;
    for (auto &automaton : automatonMap)
    {
        tempStartNode->addNextNode(automaton.second.getStartNode(), char(238)); // epsilon input to go to each automaton
        tempFinalNodesMap[automaton.second.getFinalNode()] = automaton.first;
    }

    this->setStartNode(tempStartNode);
    this->setFinalNodesMap(tempFinalNodesMap);
}

void CombinedAutomaton::generateCombinedAutomaton(unordered_map<string, Automaton> &automatonMap, unordered_map<string, Automaton> &keywordsMap)
{
    /***
     * This function generates the combined automaton from the automaton map and the keywords map
     * @param automatonMap: the map of the automata generated from the regular expressions
     * @param keywordsMap: the map of the keywords
     * @return: void
     */
    CombinedAutomaton::generateCombinedAutomaton(automatonMap);
    Node *tempStartNode = this->getStartNode();
    unordered_map<Node *, string> tempFinalNodesMap = this->getFinalNodesMap();

    for (auto &keyword : keywordsMap)
    {
        tempStartNode->addNextNode(keywordsMap[keyword.first].getStartNode(), char(238)); // epsilon input to go to each automaton
        tempFinalNodesMap[keywordsMap[keyword.first].getFinalNode()] = keyword.first;
    }

    this->setFinalNodesMap(tempFinalNodesMap);
}

void CombinedAutomaton::generateCombinedAutomaton(unordered_map<string, Automaton> &automatonMap, unordered_map<string, Automaton> &keywordsMap, unordered_map<char, Automaton> &punctuationsAutomaton)
{
    generateCombinedAutomaton(automatonMap, keywordsMap);
    Node *tempStartNode = this->getStartNode();
    unordered_map<Node *, string> tempFinalNodesMap = this->getFinalNodesMap();

    for (auto &punctuation : punctuationsAutomaton)
    {
        tempStartNode->addNextNode(punctuationsAutomaton[punctuation.first].getStartNode(), char(238)); // epsilon input to go to each automaton
        tempFinalNodesMap[punctuationsAutomaton[punctuation.first].getFinalNode()] = string(1, punctuation.first);
    }

    this->setFinalNodesMap(tempFinalNodesMap);
}

void CombinedAutomaton::setPriorityMap(unordered_map<string, int> &priorityMap)
{
    /***
     * This function sets the priority map of the automata
     * @param priorityMap: the priority map to be set
     * @param automatonMap: the map of the automata generated from the regular expressions
     * @return: void
     */
    this->priorityMap = priorityMap;
}

int CombinedAutomaton::getPriority(Node *node)
{
    /***
     * This function returns the priority of the node lower priority means higher precedence
     * @param node: the node to get its priority
     * @return: the priority of the node
     */
    if (!node->getIsFinal())
    {
        return -1; // not a final node
    }

    string token = this->finalNodesMap[node];
    int priority = this->priorityMap[token];

    return priority;
}

int CombinedAutomaton::getPriority(string token)
{
    /***
     * This function returns the priority of the token lower priority means higher precedence
     * @param token: the token to get its priority
     * @return: the priority of the token
     */
    int priority = this->priorityMap[token];

    return priority;
}

string CombinedAutomaton::getTokenName(Node *node)
{
    /***
     * This function returns the token name of the node
     * @param node: the node to get its token name
     * @param finalNodesMap: the map of the final nodes of the automata
     * @return: the token name of the node
     */
    if (!node->getIsFinal())
    {
        return ""; // not a final node
    }

    string token = finalNodesMap[node];

    return token;
}

void CombinedAutomaton::print()
{
    cout << "Combined Automaton Nodes:" << endl;
    unordered_set<Node *> markedNodes;
    queue<Node *> nodesQueue;
    nodesQueue.push(startNode);
    markedNodes.insert(startNode);
    int nodesCounter = 0;
    while (!nodesQueue.empty())
    {
        nodesCounter++;
        Node *currentNode = nodesQueue.front();
        nodesQueue.pop();
        currentNode->printNode();
        for (auto const &element : currentNode->getNextNodes())
            for (Node *nextNode : element.second)
                if (markedNodes.find(nextNode) == markedNodes.end())
                {
                    nodesQueue.push(nextNode);
                    markedNodes.insert(nextNode);
                }
    }
    cout << "Combined Automaton Number of Nodes: " << nodesCounter << endl;
    cout << endl << "Combined Automaton Final Nodes RE:" << endl;
    for (const auto &element : finalNodesMap)
        cout << element.first->getNodeNumber() << " : " << element.second << endl;
    cout << "Combined Automaton Number of Final Nodes: " << finalNodesMap.size() << endl;
    cout << endl << "Combined Automaton REs Priorities:" << endl;
    for (const auto &element : priorityMap)
        cout << element.first << " : " << element.second << endl;
    cout << "Combined Automaton Number of REs: " << priorityMap.size() << endl;
}