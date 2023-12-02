#ifndef COMPILERGENERATOR_NFADFACONVERTER_H
#define COMPILERGENERATOR_NFADFACONVERTER_H

#include "../AutomatonDataStructure/CombinedAutomaton.h"

// TODO: check if it is better to make this a namespace and extract common reusable functions.

class NFADFAConverter
{
public:
    static CombinedAutomaton convertNFAToDFA(CombinedAutomaton nfa, unordered_map<string, int> &regularExpressionsPriorityMap);
};

#endif