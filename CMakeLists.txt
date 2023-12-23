cmake_minimum_required(VERSION 3.21)
project(CompilerGenerator)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE Debug)

add_executable(
        CompilerGenerator main.cpp
        ParsingInputFile/parser.cpp
        ParsingInputFile/parser.h
        AutomatonDataStructure/Node.cpp
        AutomatonDataStructure/Node.h
        AutomatonDataStructure/Automaton.cpp
        AutomatonDataStructure/Automaton.h
        AutomatonDataStructure/automatonGenerator.cpp
        AutomatonDataStructure/CombinedAutomaton.h
        AutomatonDataStructure/CombinedAutomaton.cpp
        AutomatonDataStructure/automatonGenerator.h
        Postfix-expression/Postfix_expression.h
        Postfix-expression/Postfix_expression.cpp
        NFADFAConverter/NFADFAConverter.h
        NFADFAConverter/NFADFAConverter.cpp
        LexicalAnalyzer/LexicalAnalyzer.h
        LexicalAnalyzer/LexicalAnalyzer.cpp
        CFGParser/CFG.h
        CFGParser/CFG.cpp
        CFGParser/CFGParser.h
        CFGParser/CFGParser.cpp
        tablePreparation/FirstCalculation.h
        tablePreparation/FirstCalculation.cpp
        tablePreparation/FollowCalculation.h
        tablePreparation/FollowCalculation.cpp
        )