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
    return std::stoi(numericLine);
}

void processLineA(const std::string& line, int& value, int gameNum) {
    int maxR = 12;
    int maxG = 13;
    int maxB = 14;
    std::vector<std::string> rounds = {};

    bool validGame = true;

    std::stringstream ss(line);
    std::string round;
    while (std::getline(ss, round, ';')) {
        rounds.push_back(round);
    }

    for (std::string& round : rounds) {
        std::vector<int> gameRGB = {};
        std::stringstream ss(round);
        std::string color;

        while (std::getline(ss, color, ',')) {
            // check if color contains a substr of 'Red'
            if (color.find("red") != std::string::npos) {
                int tiles = pullInteger(color);
                if (tiles > maxR) {
                    validGame = false;
                    break;
                }
            }
            if (color.find("green") != std::string::npos) {
                int tiles = pullInteger(color);
                if (tiles > maxG) {
                    validGame = false;
                    break;
                }
            }
            if (color.find("blue") != std::string::npos) {
                int tiles = pullInteger(color);
                if (tiles > maxB) {
                    validGame = false;
                    break;
                }
            }
        }
    }

    if (validGame) {
        value += gameNum;
    }
}

int partA(std::ifstream& inputFile) {
    int value = 0;
    std::string line;
    int gameNum = 1;

    while (std::getline(inputFile, line)) {
        processLineA(line, value, gameNum);
        gameNum++;
    }

    std::cout << "Value: " << value << std::endl;

    inputFile.close(); 

    return 0;
}

void processLineB(const std::string& line, int& value) {
    int minR = 0;
    int minG = 0;
    int minB = 0;
    
    std::vector<std::string> rounds = {};

    std::stringstream ss(line);
    std::string round;
    while (std::getline(ss, round, ';')) {
        rounds.push_back(round);
    }

    for (std::string& round : rounds) {
        std::vector<int> gameRGB = {};
        std::stringstream ss(round);
        std::string color;

        while (std::getline(ss, color, ',')) {
            if (color.find("red") != std::string::npos) {
                int tiles = pullInteger(color);
                if (tiles > minR) {
                    minR = tiles;
                }
            }
            if (color.find("green") != std::string::npos) {
                int tiles = pullInteger(color);
                if (tiles > minG) {
                    minG = tiles;
                }
            }
            if (color.find("blue") != std::string::npos) {
                int tiles = pullInteger(color);
                if (tiles > minB) {
                    minB = tiles;
                }
            }
        }
    }

printf("minR: %d, minG: %d, minB: %d, sum: %d\n", minR, minG, minB, minR*minG*minB);
    value += minB * minG * minR;
}

int partB(std::ifstream& inputFile) {
    int value = 0;
    std::string line;

    while (std::getline(inputFile, line)) {
        processLineB(line, value);
    }

    std::cout << "Value: " << value << std::endl;

    inputFile.close(); 

    return 0;
}


int main() {
    std::ifstream inputFile("./day02/input");
    // std::ifstream inputFile("./day02/sample"); // Sample input
    if (!inputFile) {
        std::cerr << "Error opening file." << std::endl;
        return 1;
    }

    // partA(inputFile);
    partB(inputFile);
}
