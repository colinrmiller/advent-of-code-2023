#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <exception>
#include <typeinfo>
#include <unordered_set>

// remove all non-numerical characters from line and return the int val
int pullInteger(const std::string line) {
    std::string numericLine = "";
    for (char ch : line) {
        if (ch >= '0' && ch <= '9') {
            numericLine += ch;
        }
    }
    try {
        return std::stoi(numericLine);
    } catch (const std::invalid_argument& e) {
        return -1;
    } catch (const std::out_of_range& e) {
        return -1;

    }
}

void processLineA(const std::string& line, int& value) {
    std::unordered_set<int> vals = {};
    int lineValue = 0;
    
    std::stringstream ss(line);
    std::vector<std::string> sets= {};
    std::string set;
    while (std::getline(ss, set, '|')) {
        sets.push_back(set);
    }
    
    std::stringstream ss0(sets[0]);
    std::string num;
    while (std::getline(ss0, num, ' ')) {
        int val = pullInteger(num);
        if (val > 0) { vals.insert(pullInteger(num)); };
    }

    std::stringstream ss1(sets[1]);
    while (std::getline(ss1, num, ' ')) {
        int val = pullInteger(num);
        if (vals.find(val) != vals.end()) {
            lineValue = lineValue == 0 ? 1 : lineValue * 2;
        }
    }
    value += lineValue;
}

int partA(std::ifstream& inputFile) {
    int value = 0;
    std::string line;

    while (std::getline(inputFile, line)) {
        processLineA(line, value);
    }

    std::cout << "Value: " << value << std::endl;

    inputFile.close(); 

    return 0;
}

int processLineB(const std::string& line) {
    std::unordered_set<int> vals = {};
    int total = 0;
    int lineValue = 0;
    
    std::stringstream ss(line);
    std::vector<std::string> sets= {};
    std::string set;
    while (std::getline(ss, set, '|')) {
        sets.push_back(set);
    }
    
    std::stringstream ss0(sets[0]);
    std::string num;
    while (std::getline(ss0, num, ' ')) {
        int val = pullInteger(num);
        if (val > 0) { vals.insert(pullInteger(num)); };
    }

    std::stringstream ss1(sets[1]);
    while (std::getline(ss1, num, ' ')) {
        int val = pullInteger(num);
        if (vals.find(val) != vals.end()) {
            lineValue++;
        }
    }
    return lineValue;
}

int partB(std::ifstream& inputFile) {
    int value = 0;
    int lineValue;
    std::vector<int> weights = {};
    std::string line;
    int n = 1;

    while (std::getline(inputFile, line)) {
        int lineWeight;
        
        if (weights.size() >= 1) {
            lineWeight = weights[0];
            weights.erase(weights.begin());
            value += lineWeight;
            printf("Value: %d\n", lineWeight);
        } else {
            lineWeight = 1;
            value++;
        }

        lineValue = processLineB(line);
        
        for (int i = 0; i < lineValue; i++) {
            if (weights.size() <= i) {
                weights.push_back(lineWeight + 1);
            } else {
                weights[i] += lineWeight;
            }
        }
    }

    std::cout << "Value: " << value << std::endl;

    inputFile.close(); 

    return 0;
}

int main() {
    std::ifstream inputFile("./day04/input");
    // std::ifstream inputFile("./day04/sample"); // Sample input
    if (!inputFile) {
        std::cerr << "Error opening file." << std::endl;
        return 1;
    }

    // partA(inputFile);
    partB(inputFile);
}
