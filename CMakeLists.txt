cmake_minimum_required(VERSION 3.21)
project(CompilerGenerator)

set(CMAKE_CXX_STANDARD 14)

add_executable(CompilerGenerator main.cpp ParsingInputFile/parser.cpp ParsingInputFile/parser.h AutomatonDataStructure/Node.cpp AutomatonDataStructure/Node.h AutomatonDataStructure/Automaton.cpp AutomatonDataStructure/Automaton.h AutomatonDataStructure/automatonGenerator.cpp AutomatonDataStructure/automatonGenerator.h
        Postfix-expression/Postfix_expression.h
        Postfix-expression/Postfix_expression.cpp)

