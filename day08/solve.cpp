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

    Cell(std::string name, std::string left, std::string right) {
        this->name = name;
        this->left = left;
        this->right = right;
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


int main() {
    std::filesystem::path inputFile("./day08/input");
    // std::filesystem::path inputFile("./day08/sample");
    std::vector<std::string> input = Helpers::ReadAllLinesInFile(inputFile);
    printf("input: %s\n", input[0].c_str());


    partA(input);
    // partB(input);
}
