#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;
// This function reads the first character from a file and returns it.
vector<string> readInputFile(const string& filepath) {
    ifstream file(filepath);

    if (!file.is_open()) {
        std::cerr << "Error opening the file.\n";
        exit(1);
    }

    string line;
    vector<string> lines;
    while (getline(file, line)) {
        lines.push_back(line);
    }
    file.close();
    return lines;
}



void parse(string line, vector<string> &tokens) {

}

