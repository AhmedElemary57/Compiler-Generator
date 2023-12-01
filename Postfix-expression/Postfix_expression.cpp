// Created by elsaber on 28/11/23.

#include "Postfix_expression.h"

using namespace std;

// Function to return precedence of operators
int prec(char c) {
    if (c == '*' || c == '+')
        return 3;
    else if (c == ' ')
        return 2;
    else if (c == '|')
        return 1;
    else
        return -1;
}

// Function to check if a character is a special character
bool is_special(char c) {
    return c == ' ' || c == '(' || c == ')' || c == '+' || c == '*' || c == '|';
}

// Function to handle operations during postfix expression evaluation
void handle_operation(stack<Automaton> &automaton_stack, char op) {
    Automaton res;
    Automaton top = automaton_stack.top();
    automaton_stack.pop();

    if (op == ' ') {
        // To be implemented: Concatenation operation
    } else if (op == '|') {
        // To be implemented: Parallel operation
    } else if (op == '+') {
        res = plusOperation(top);
    } else if (op == '*') {
        res = closure(top);
    }

    automaton_stack.push(res);
}

// Function to convert a postfix expression to an automaton
Automaton Postfix_expression::postfix(std::string expression, unordered_map<std::string, Automaton> &automatons) {
    stack<char> op;
    stack<Automaton> automaton_stack;

    // Prepare characters and handle escape sequences
    prepareCharacters(expression, automatons);

    string cur = "";
    for (int i = 0; i < expression.length(); ++i) {
        // Extract current Automation
        while (!is_special(expression[i])) {
            if (expression[i] == '\\' && i + 1 < expression.length() && is_special(expression[i + 1])) {
                cur.push_back('\\');
                cur.push_back(expression[i + 1]);
                i += 2;
            } else {
                cur.push_back(expression[i]);
                i++;
            }
        }

        // Add it to the stack
        if (cur.length() > 0) {
            if (automatons.find(cur) == automatons.end()) {
                cerr << "Error: Undefined automaton '" << cur << "'" << endl;
                exit(-1);
            }
            automaton_stack.push(automatons.at(cur));
            cur.clear();
        } else {
            // Handle operators and parentheses
            if (expression[i] == '(') {
                op.push('(');
            } else if (expression[i] == ')') {
                while (op.top() != '(') {
                    handle_operation(automaton_stack, op.top());
                    op.pop();
                }
                op.pop();
            } else {
                while (!op.empty() && prec(expression[i]) <= prec(op.top())) {
                    handle_operation(automaton_stack, op.top());
                    op.pop();
                }
                op.push(expression[i]);
            }
        }
    }

    // Process any remaining operators
    while (!op.empty()) {
        handle_operation(automaton_stack, op.top());
        op.pop();
    }
    Automaton result = automaton_stack.top();
    automaton_stack.pop();
    if(!automaton_stack.empty()){
        cerr << "Error: Some thing wrong '" << cur << "'" << endl;
    }
    return result;
}

// Function to create an automaton for a range of characters (case 1)
Automaton case1(string str) {
    Automaton automaton;
    automaton.setStartNode(new Node());
    automaton.setFinalNode(new Node());

    for (char j = str[0]; j <= str[2]; j++) {
        automaton.getStartNode()->addNextNode(automaton.getFinalNode(), j);
    }

    return automaton;
}

// Function to create a simple automaton for a single character (case 2)
Automaton make_simple_automaton(char a) {
    Automaton automaton;
    automaton.setStartNode(new Node());
    automaton.setFinalNode(new Node());
    automaton.getStartNode()->addNextNode(automaton.getFinalNode(), a);
    return automaton;
}

// Function to create an automaton for a sequence of characters (case 2)
Automaton case2(string str) {
    bool first = true;
    Automaton result;

    for (int i = 0; i < str.length(); ++i) {
        if (str[i] == '\\' && i + 1 < str.length() && is_special(str[i + 1])) {
            i++;
        }
        if (first) {
            result = make_simple_automaton(str[i]);
            first = false;
        } else {
            // TO DO: Concatenate operation
            // result = concatenate(result, make_simple_automaton(str[i]));
        }
    }

    return result;
}

// Function to handle simple expressions (case 1 and case 2)
void handle_simple_expression(string str, unordered_map<string, Automaton> &automatons) {
    if (str.length() == 3 && str[1] == '-') {
        automatons[str] = case1(str);
    } else {
        automatons[str] = case2(str);
    }
}

// Function to prepare characters and handle escape sequences
string Postfix_expression::prepareCharacters(string str, unordered_map<string, Automaton> &automatons) {
    unsigned long n = str.length();
    string cur;

    for (auto i = n - 1; i >= 0; --i) {
        if (is_special(str[i]) && i - 1 >= 0 && str[i - 1] == '\\') {
            cur.push_back(str[i]);
            cur.push_back(str[i - 1]);
            i--;
            continue;
        }
        if (is_special(str[i])) {
            if (cur.length() != 0 && automatons.find(cur) == automatons.end()) {
                // Key not found, handle the simple expression
                handle_simple_expression(cur, automatons);
            }
            cur.clear();
        } else {
            cur.push_back(str[i]);
        }
    }

    if (cur.length() != 0 && automatons.find(cur) == automatons.end()) {
        // Handle the last part of the string
        handle_simple_expression(cur, automatons);
    }

    return str;
}
