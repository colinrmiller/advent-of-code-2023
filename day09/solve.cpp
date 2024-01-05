#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <exception>
#include <typeinfo>
#include <unordered_set>
#include "../helpers/helpers.h"
#include <map>

std::string day = "09";
// bool sample = false;
bool sample = true;

Helpers::Logger Logger = Helpers::Logger("./day" + day + "/log");

struct Sequence {
    std::vector<int> sequence;
    Sequence* prev = nullptr;
    Sequence* next = nullptr;
    bool isLinear = false;
    int nextNumber;
    bool nextNumberFound = false;

    Sequence(std::vector<int> sequence, Sequence* prev = nullptr) {
        this->sequence = sequence;
        this->prev = prev;
        if (this->prev != nullptr) {
            this->prev->next = this;
        }

        // this->next = this->Diff();
    }

    std::vector<int> Diff() {
        std::vector<int> diff;
        
        for (int i = 0; i < this->sequence.size() - 1; i++) {
            diff.push_back(this->sequence[i + 1] - this->sequence[i]);
        }

        return diff;
    }

    static Sequence* DiffList(std::vector<int> sequence) {
        Sequence* head = new Sequence(sequence);

        Sequence* current = head;
        Sequence* nextSequence = nullptr;
        while (!current->isLinear) {
            std::vector<int> diff = current->Diff();
            Sequence* nextSequence = new Sequence(diff, current);

            if (diff[0] == diff[diff.size() - 1]) {
                nextSequence->isLinear = true;
                nextSequence->nextNumber = diff[0];
                nextSequence->nextNumberFound = true;
            }

            current = nextSequence;
        }

        return head;
    }

    int NextNumber() {
        if (this->next != nullptr && this->next->nextNumberFound) {
            int lastNumber = this->sequence[this->sequence.size() - 1];
            this->nextNumber = lastNumber + this->next->nextNumber;
            this->nextNumberFound = true;

            if (this->prev != nullptr) {
                return this->prev->NextNumber();
            }

            return this->nextNumber;
        }

        Sequence* current = this;

        while (!current->isLinear) {
            current = current->next;
        }

        return current->prev->NextNumber();
    }

    void Log() {
        Sequence* current = this;
        while (current != nullptr) {
            for (int i = 0; i < current->sequence.size(); i++) {
                Logger.log(std::to_string(current->sequence[i]) + " ");
            }
            Logger.log(": " + std::to_string(current->nextNumber) + "\n");
            current = current->next;
        }
        Logger.log("\n");
    }
};

void partA( std::vector<std::string>& input) {
    int value = 0;

    for (std::string line : input) {
        std::vector<std::string> parts = Helpers::SplitString(line, ' ');
        std::vector<int> sequence = {};
        for (std::string part : parts) {
            sequence.push_back(std::stoi(part));
        }

        Sequence* seqList = Sequence::DiffList(sequence);

        int nextNumber = seqList->NextNumber();
        seqList->Log();
        std::cout << "Next Number: " << nextNumber << std::endl;
        value += nextNumber;
    }

    printf("Value: %d\n", value);
}

void partB(std::vector<std::string> input) {
    int value = 0;

    std::cout << "Value: " << value << std::endl;
}


int main() {
    std::string inputPath = sample ? "./day" + day + "/sample" : "./day" + day + "/input";
    std::filesystem::path inputFile(inputPath);

    std::vector<std::string> input = Helpers::ReadAllLinesInFile(inputFile);

    partA(input);
    // partB(input);
}
