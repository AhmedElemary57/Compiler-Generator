#include <iostream>
#include <string>
#include <vector>
#include "parser.h"

using namespace std;


int main() {
    // Read the file into a string.
    string filepath = "D:\\.CSED\\CompilerGenerator\\input.txt";
    vector<string> c = readInputFile(filepath);
    cout << "The first line is in the file is: \n" << c[0] << endl;

    return 0;
}
