#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <exception>
#include <typeinfo>
#include <unordered_set>

bool charIsInteger(const char& ch) {
    int digitMin = '0';
    int digitMax = '9';
    if (ch < digitMin || ch > digitMax) {
        return false;
    }
    return true;
}

class Cell {
    private:
    public:
        bool isInt;
        bool isChar;
        bool isSpace;
        bool isGear;
        int value;
        char ch;
        int gearAdjacencies = 0;
        int gearValue = 1;
        int x, y = -1;

        Cell* left;
        Cell* prev;

        Cell(char ch, Cell* left, Cell* prev) {
            this->ch = ch;
            this->isInt = charIsInteger(ch);
            this->value = this->isInt ? ch - '0' : -1;
            this->isChar = !this->isInt && ch != 46;
            this->isSpace = !this->isInt && ch == 46;
            this->isGear = !this->isInt && ch == 42;
            this->left = left;
            this->prev = prev;
            
            this->setPos();
        }   
        
        void print() {
            std::cout << this->ch;
        }

        void setPos() {
            if (this->prev == nullptr) {
                this->x = 0; 
                this->y = 0;
            } else if (this->left != nullptr) {
                this->x = this->left->x + 1;
                this->y = this->left->y;
            } else if (this->left == nullptr && this->prev != nullptr) {
                this->y = this->prev->y + 1;
                this->x = 0;
            }
        }

        bool operator==(const Cell& other) const {
            return x == other.x && y == other.y;
        }

};

namespace std {
    template <>
    struct hash<Cell> {
        size_t operator()(const Cell& cell) const {
        std::size_t hash_x = std::hash<int>()(cell.x);
        std::size_t hash_y = std::hash<int>()(cell.y);

        // Combine hashes using bitwise XOR and bit shifts for better distribution
        return (hash_x << 16) ^ hash_y;
        }
    };
}


void buildGrid(std::vector<std::vector<Cell>>& grid, std::ifstream& inputFile) {
    std::string line;
    Cell* prev = nullptr;
    Cell* left = nullptr;

    while (std::getline(inputFile, line)) {
        std::vector<Cell> chs;
        for (char ch : line) {
            chs.push_back(Cell(ch, left, prev));
            prev = &chs.back();
            left = &chs.back();
        }
        grid.push_back(chs);
        left = nullptr;
    }
}

bool findLocalChar(std::vector<std::vector<Cell>>& grid, int row, int col, std::unordered_set<Cell*>& localGears) {
    int rowMin = row - 1;
    int rowMax = row + 1;
    int colMin = col - 1;
    int colMax = col + 1;

    for (int r = rowMin; r <= rowMax; r++) {
        for (int c = colMin; c <= colMax; c++) {
            if (r == row && c == col) {
                continue;
            }
            if (r < 0 || r >= grid[0].size()) {
                continue;
            }
            if (c < 0 || c >= grid.size()) {
                continue;
            }
            if (grid[r][c].isChar) {
                if (grid[r][c].isGear) {
                    localGears.insert(&grid[r][c]);
                }
                return true;
            }
        }
    }
    return false;
}

int vectorToInt(std::vector<int>& intStack) {
    int value = 0;
    int multiplier = 1;
    for (int i = intStack.size() - 1; i >= 0; i--) {
        value += intStack[i] * multiplier;
        multiplier *= 10;
    }
    return value;
}

int solve(std::ifstream& inputFile) {
    int value = 0;
    std::unordered_set<Cell*>gears = {};

    std::vector<std::vector<Cell>> grid;

    buildGrid(grid, inputFile);
    std::vector<int> intStack = {};

    for (int row = 0; row < grid[0].size(); row++) {
        bool localChar = false;
        std::unordered_set<Cell*> localGears = {};
        for (int col = 0; col < grid.size(); col++) {
            Cell cell = grid[row][col];
            if (cell.isInt) {
                intStack.push_back(cell.value);
                localChar = localChar || findLocalChar(grid, row, col, localGears);
            } else {
                if (localChar && intStack.size() > 0) {
                    value += vectorToInt(intStack);
                    if (localGears.size() > 0) {
                        for (Cell* gear : localGears) {
                            gear->gearValue *= vectorToInt(intStack);
                            if (gear->gearAdjacencies > 2) {
                                int test = 3;
                            }

                            gear->gearAdjacencies++;
                            gears.insert(gear);
                        }
                    }
                }
                intStack = {};
                localChar = false;
                localGears = {};
            }
        }

        if (localChar && intStack.size() > 0) {
            value += vectorToInt(intStack);

            for (Cell* gear : localGears) {
                gear->gearValue *= vectorToInt(intStack);
                gear->gearAdjacencies++;
                gears.insert(gear);
            }
        }
    }

    std::cout << "Value: " << value << std::endl;

    int valueB = 0;
    for (Cell* gear : gears) {
        if (gear->gearAdjacencies == 2) {
            valueB += gear->gearValue;
        }
    }
    std::cout << "ValueB: " << valueB << std::endl;
    inputFile.close(); 

    return 0;
}

int main() {
    std::ifstream inputFile("./day03/input");
    // std::ifstream inputFile("./day03/sample"); // Sample input
    if (!inputFile) {
        std::cerr << "Error opening file." << std::endl;
        return 1;
    }

    solve(inputFile);
}
