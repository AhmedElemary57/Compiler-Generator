#ifndef COMPILERGENERATOR_NFADFACONVERTER_H
#define COMPILERGENERATOR_NFADFACONVERTER_H

#include "../AutomatonDataStructure/Automaton.h"

class NFADFAConverter
{
public:
    static Automaton convertNFAToDFA(Automaton nfa);
};

#endif