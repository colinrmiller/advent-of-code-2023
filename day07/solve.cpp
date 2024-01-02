#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <exception>
#include <typeinfo>
#include <unordered_set>
#include "../helpers/helpers.h"

struct Hand {
    std::vector<char> hand;
    std::string handString;
    float handRank = 0;
    int handValue = 0;
    int bet;

    Hand(std::string handString, bool jacksWild = false) {
        this->handString = handString;
        hand = {};
        for (char ch : handString) {
            this->hand.push_back(ch);
        }

        if (!jacksWild) {
            this->calculateRankA();
            this->calculateValueA();
        } else {
            this->calculateRankB();
            this->calculateValueB();
        }
    }

    int cardValueA(char ch) {
        switch (ch) {
            case 'T':
                return 10;
            case 'J':
                return 11;
            case 'Q':
                return 12;
            case 'K':
                return 13;
            case 'A':
                return 14;
            default:
                if (ch >= '2' && ch <= '9') {
                    return ch - '0';
                }
                printf("Invalid card value: %c\n", ch);
                return -1; // Indicate an invalid card value
        }
    }

    void calculateRankA() {
        // create dictionary of char counts
        std::unordered_map<char, int> charCounts;
        for (char ch : this->hand) {
            if (charCounts.find(ch) == charCounts.end()) {
                charCounts[ch] = 0;
            }
            charCounts[ch]++;
        }

        // generate a number representing the hand rank. Larger numbers represent greater hand strength
        for (const auto& charCount : charCounts) {
            if (charCount.second > 1) {
                if (this->handRank == 0) {
                    this->handRank = charCount.second;
                } else if (charCount.second > this->handRank) {
                    this->handRank = charCount.second + 0.1 * this->handRank;
                } else if (charCount.second < this->handRank) {
                    this->handRank = this->handRank + 0.1 * charCount.second;
                } else if (charCount.second == this->handRank) {
                    this->handRank = this->handRank * 1.1;
                }
            }
        }
        printf("hand: %s, handRank: %f\n", this->handString.c_str(), this->handRank);
    }

    void calculateValueA() {
        for (char ch : this->hand) {
            this->handValue = 16 * this->handValue + cardValueA(ch); // 16 > 14 (max card value)
        }
    }

    int cardValueB(char ch) {
        switch (ch) {
            case 'T':
                return 10;
            case 'J':
                return 1;
            case 'Q':
                return 12;
            case 'K':
                return 13;
            case 'A':
                return 14;
            default:
                if (ch >= '2' && ch <= '9') {
                    return ch - '0';
                }
                printf("Invalid card value: %c\n", ch);
                return -1;
        }
    }

    void calculateRankB() {
        printf("calculateRank B");
        // create dictionary of char counts
        std::unordered_map<char, int> charCounts;
        for (char ch : this->hand) {
            if (charCounts.find(ch) == charCounts.end()) {
                charCounts[ch] = 0;
            }
            charCounts[ch]++;
        }

        int jacks = charCounts['J'];
        //remove jacks from list
        charCounts['J'] = 0;

        // generate a number representing the hand rank. Larger numbers represent greater hand strength
        std::vector<int> charAmounts = {};
        for (const auto& charCount : charCounts) {
            charAmounts.push_back(charCount.second);
        }
        // order charAmounts in decreasing value
        std::sort(charAmounts.begin(), charAmounts.end(), [](int a, int b) { return a > b; });
        charAmounts[0] += jacks;

        this->handRank = charAmounts[0] + 0.1 * charAmounts[1];
        
        printf("hand: %s, handRank: %f\n", this->handString.c_str(), this->handRank);
    }

    void calculateValueB() {
        for (char ch : this->hand) {
            this->handValue = 16 * this->handValue + cardValueB(ch); // 16 > 14 (max card value)
        }
    }
};

void sortHands(std::vector<Hand>& hands) {
    auto sortFunc = [](const Hand& a, const Hand& b) {
        if (a.handRank == b.handRank) {
            return a.handValue < b.handValue;
        }
        return a.handRank < b.handRank;
    };

    std::sort(hands.begin(), hands.end(), sortFunc);
}

void partA( std::vector<std::string>& input) {
    std::vector<Hand> hands = {};
    
    long long value = 0;

    for (std::string line : input) {
        std::vector<std::string> parsedLine = Helpers::splitString(line);
        hands.emplace_back(Hand(parsedLine[0]));
        hands.back().bet = std::stoi(parsedLine[1]);
    }

    sortHands(hands);

    for (size_t i = 0; i < hands.size(); i++) {
        printf("hand: %s, bet: %d\n", hands[i].handString.c_str(), hands[i].bet);
        value += hands[i].bet * (i + 1);
    }

    std::cout << "Value: " << value << std::endl;
}

void partB( std::vector<std::string>& input) {
    std::vector<Hand> hands = {};
    
    long long value = 0;

    for (std::string line : input) {
        std::vector<std::string> parsedLine = Helpers::splitString(line);
        hands.emplace_back(Hand(parsedLine[0], true));
        hands.back().bet = std::stoi(parsedLine[1]);
    }

    sortHands(hands);

    for (size_t i = 0; i < hands.size(); i++) {
        printf("hand: %s, bet: %d\n", hands[i].handString.c_str(), hands[i].bet);
        value += hands[i].bet * (i + 1);
    }

    std::cout << "Value: " << value << std::endl;
}

int main() {
    std::filesystem::path inputFile("./day07/input");
    // std::filesystem::path inputFile("./day07/sample");
    std::vector<std::string> input = Helpers::ReadAllLinesInFile(inputFile);
    printf("input: %s\n", input[0].c_str());


    // partA(input);
    partB(input);
}
