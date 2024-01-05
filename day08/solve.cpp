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

struct Cell {
    std::string name;
    std::string left;
    std::string right;
    int mapPos = -1;

    Cell(std::string name, std::string left, std::string right) {
        this->name = name;
        this->left = left;
        this->right = right;
    }

    void setMapPos(int pos) {
        if (this->mapPos != -1) {
            // printf("SHIT HAPPENING");
            return;
        }
        this->mapPos = pos;

    }

    void print() {
        std::cout << this->name << ": " << this->left << ", " << this->right << std::endl;
    }
};

struct Cells {
    std::map<std::string, Cell*> cells;

    Cells() {
        this->cells = {};
    }

    void addCell(std::string name, std::string left, std::string right) {
        Cell* cell = new Cell(name, left, right);
        this->cells[name] = cell;
    }

    void print() {
        for (auto const& [key, val] : this->cells) {
            std::cout << key << ": " << val->left << ", " << val->right << std::endl;
        }
    }

    Cell* lookup(std::string name) {
        return this->cells[name];
    }
};

struct CellGroup {
    std::map<std::string, Cell*> cells;
    std::map<int, int> cycleLengths;
    Cells cellMap;

    CellGroup() {
        this->cells = {};
    }

    void seedGroup(Cells cells) {
        this->cellMap = cells;
        
        int pos = 0;
        for (auto const& [key, val] : cells.cells) {
            if (key.substr(2, 1) == "A") {
                this->cells[key] = val;
                val->setMapPos(pos);
                pos++;
            }
        }
        printf("Seeded group with %d cells\n", pos);
    }

    void stepGroup(char step, int stepNum) {
        std::map<std::string, Cell*> newCells = {};

        for (auto const& [key, val] : this->cells) {
            Cell* endCell = nullptr; 

            if (step == 'L') {
                endCell = this->cellMap.lookup(val->left);
                newCells[val->left] = endCell;
            } else {
                endCell = this->cellMap.lookup(val->right);
                newCells[val->right] = endCell;
            }

            if (endCell != nullptr) {
                endCell->setMapPos(val->mapPos);
            }

            if (endCell != nullptr && endCell->name.size() > 2 && endCell->name.substr(2, 1) == "Z") {
                int pos = endCell->mapPos;
                cycleLengths[pos] = stepNum+1;
                printf("Found cycle length %d: %d\n", pos, stepNum);
                printf("endCell: %s\n", endCell->name.c_str());
            }
        }
        this->cells = newCells;
    }


    bool isFinished() {
        return (this->cells.size() == this->cycleLengths.size());
    }

    void print() {
        for (auto const& [key, val] : this->cells) {
            std::cout << val->mapPos << " " << key << ": " << val->left << ", " << val->right << std::endl;
        }
    }

    long long gcd(long long a, long long b) {
        if (b == 0)
            return a;
        return gcd(b, a % b);
    }

    long long lcm(long long a, int b) {
        long long longB = static_cast<long long>(b);
        printf("a: %lld, b: %d, res: %lld\n", a, b, (a * longB) / gcd(a, longB));
        return (a * longB) / gcd(a, longB);
    }

    long long calculateTotalCycleLength() {
        long long totalCycleLength = 1;
        for (auto const& [key, val] : this->cycleLengths) {
            totalCycleLength = this->lcm(totalCycleLength, val);
        }
        return totalCycleLength;
    }
};

void partA( std::vector<std::string>& input) {
    int value = 0;

    Cells cells = Cells();
    const std::string directions = input[0].c_str();

    input.erase(input.begin(), input.begin() + 2);

    for (std::string line : input) {
        std::vector<std::string> parts = Helpers::SplitString(line, '=');
        std::string name = parts[0];
        std::string adjacencies = parts[1];
        cells.addCell(name.substr(0,3), adjacencies.substr(2, 3), adjacencies.substr(7, 3));
    }

    std::string currentCell = "AAA";
    while (currentCell != "ZZZ") {
        Cell* cell = cells.cells[currentCell];

        int pos = value % (directions.size());
        char step = directions[pos];

        currentCell = step == 'L' ? cell->left : cell->right;
        
        value++;
    }

    std::cout << "Value: " << value << std::endl;
}

void partB(std::vector<std::string> input) {
    int value = 0;

    Cells cells = Cells();
    const std::string directions = input[0].c_str();

    input.erase(input.begin(), input.begin() + 2);

    for (std::string line : input) {
        std::vector<std::string> parts = Helpers::SplitString(line, '=');
        std::string name = parts[0];
        std::string adjacencies = parts[1];
        cells.addCell(name.substr(0,3), adjacencies.substr(2, 3), adjacencies.substr(7, 3));
    }

    CellGroup cellGroup = CellGroup();
    cellGroup.seedGroup(cells);
    cellGroup.print();

    while (!cellGroup.isFinished()) {
        int pos = value % (directions.size());
        char step = directions[pos];

        cellGroup.stepGroup(step, value);

        if (value % 100000 == 0) {
            std::cout << "Value: " << value << std::endl;
        }

        // for (auto const& [key, val] : cellGroup.cells) {
        //     std::cout << val->mapPos << " " << key << ": " << val->left << ", " << val->right << std::endl;
        // }
        value++;
    }

    long long totalCycleLength = cellGroup.calculateTotalCycleLength();

    std::cout << "Value: " << totalCycleLength << std::endl;
    printf("Value: %lld\n", totalCycleLength);

}


int main() {
    std::filesystem::path inputFile("./day08/input");
    // std::filesystem::path inputFile("./day08/sample");
    std::vector<std::string> input = Helpers::ReadAllLinesInFile(inputFile);
    printf("input: %s\n", input[0].c_str());


    // partA(input);
    partB(input);
}
