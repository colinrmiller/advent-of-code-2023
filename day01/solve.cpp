#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <exception>
#include <typeinfo>
#include <unordered_set>


bool isInteger(const std::string& s) {
    try {
        size_t pos;
        int num = std::stoi(s);
        return num == s.length(); 
    } catch (...) {
        return false; 
    }
}


bool charIsInteger(const char& ch) {
    int digitMin = '0';
    int digitMax = '9';
    if (ch < digitMin || ch > digitMax) {
        return false;
    }
    return true;
}

int charToInt(const char& ch) {
    int digitMin = '0';
    int digitMax = '9';
    if (ch < digitMin || ch > digitMax) {
        throw std::invalid_argument("Invalid character");
    }
    return ch - digitMin;
}

int translateWordsToInts(const std::string& word) {
    int value = 0;
    int multiplier = 1;
    for (int i = word.length() - 1; i >= 0; i--) {
        int digit = charToInt(word[i]);
        value += multiplier * digit;
        multiplier *= 10;
    }
    return value;
}

void preprocessLine(std::string& line) {
    std::unordered_map<int, std::string> numberDictionary = {
        {0, "zero"},
        {1, "one"},
        {2, "two"},
        {3, "three"},
        {4, "four"},
        {5, "five"},
        {6, "six"},
        {7, "seven"},
        {8, "eight"},
        {9, "nine"}
    };

    std::vector<int> totalNumbers = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::vector<int> availableNumbers = totalNumbers;

    char ch;
    int wordPos = 0;

    auto printSet = [](std::vector<int> set) {
        printf("availableNumbers: ");
        for (auto it = set.begin(); it != set.end(); it++) {
            printf("%d ", *it);
        }
        printf("\n");
    };

    for (int pos = 0; pos < line.length(); pos++) {
        ch = line[pos];
        if (charIsInteger(ch)) {
            wordPos = 0;
            availableNumbers = totalNumbers;
        } else {
            for (auto it = availableNumbers.begin(); it != availableNumbers.end(); ) {
                int num = *it;
                std::string numWord = numberDictionary[num];
                if (numWord[wordPos] != ch) {
                    availableNumbers.erase(it);
                } else {
                    it++;
                    if (availableNumbers.size() == 1 && wordPos == numWord.length() - 1) {
                        line[pos - wordPos] = '0' + num;
                        pos -= wordPos;
                        wordPos = -1;
                        availableNumbers = totalNumbers;
                        break;
                    }
                }
            }
            if (availableNumbers.size() == 0) {
                pos -= wordPos;
                wordPos = 0;
                availableNumbers = totalNumbers;
            } else {
                wordPos++;            
            }
        }
    }
}

void processLine(const std::string& line, int& value) {
    std::stringstream ss(line);
    char ch;
    std::string ints;
    int firstInt;
    int lastInt;
    bool isFirstInt = 1;

    while (ss >> ch) {
        if (charIsInteger(ch)) {
            int intVal = charToInt(ch);
            if (isFirstInt == 1) {
                firstInt = intVal;
                isFirstInt = 0;
            }
            lastInt = intVal;
        }
    }
    int lineVal = 10*firstInt + lastInt;
    std::cout << "Line value: " << lineVal << std::endl;
    value += lineVal;
}

int partA(std::ifstream& inputFile) {
    int value = 0;
    std::string line;

    while (std::getline(inputFile, line)) {
        processLine(line, value);
    }

    std::cout << "Value: " << value << std::endl;

    inputFile.close(); 

    return 0;
}

int partB(std::ifstream& inputFile) {
    int value = 0;
    std::string line;

    while (std::getline(inputFile, line)) {
        preprocessLine(line);
        printf("%s\n", line.c_str());

        processLine(line, value);
    }

    std::cout << "Value: " << value << std::endl;

    inputFile.close(); 

    return 0;
}

int main() {
    std::ifstream inputFile("./input");
    // std::ifstream inputFile("./day01/sample"); // Sample input
    if (!inputFile) {
        std::cerr << "Error opening file." << std::endl;
        return 1;
    }

    // partA(inputFile);
    partB(inputFile);
}
