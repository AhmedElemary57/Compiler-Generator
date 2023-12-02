#ifndef COMPILERGENERATOR_NFADFACONVERTER_H
#define COMPILERGENERATOR_NFADFACONVERTER_H

#include "../AutomatonDataStructure/Automaton.h"

class NFADFAConverter
{
private:
    vector<Node *> EClosure(vector<Node *> nodes);

public:
    static Automaton convertNFAToDFA(Automaton nfa);
};

#endif