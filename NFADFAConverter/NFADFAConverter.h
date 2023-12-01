#ifndef COMPILERGENERATOR_NFADFACONVERTER_H
#define COMPILERGENERATOR_NFADFACONVERTER_H

#include "../AutomatonDataStructure/CombinedAutomaton.h"

class NFADFAConverter
{
public:
    static CombinedAutomaton convertNFAToDFA(CombinedAutomaton nfa, unordered_map<string, int> &regularExpressionsPriorityMap);
};

#endif