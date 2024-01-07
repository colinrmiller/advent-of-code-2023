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

std::string day = "11";
bool sample = false;
// bool sample = true;

Helpers::Logger Logger = Helpers::Logger("./day" + day + "/log");

std::vector<int> getSubVector(const std::vector<int>& originalVector, int a, int b) {
    if (a < 0 || b >= originalVector.size() || a > b) {
        // Invalid range
        return {};
    }

    return std::vector<int>(originalVector.begin() + a, originalVector.begin() + b + 1);
}

struct Pos {
    int x;
    int y;

    Pos() {
        this->x = -1;
        this->y = -1;
    }

    Pos(int x, int y) {
        this->x = x;
        this->y = y;
    }

    void Print() {
        printf("x: %d, y: %d\n", this->x, this->y);
    }
};

struct Cell {
    Pos pos;
    char type;
    std::vector<Cell*> adjacencies;
    std::vector<long long> distances;

    Cell(Pos pos, char type) {
        this->pos = pos;
        this->type = type;
    }

    void Log() {
        Logger.log(std::string("") + this->type);
    }
};

struct Cells {
    std::vector<size_t> dims = {};
    std::vector<std::vector<Cell*>> cellMap;
    std::vector<Cell*> galaxies;
    std::vector<bool> emptyRows;
    std::vector<bool> emptyColumns;
    bool oldGalaxy = false;

    Cells(std::vector<size_t> dims) {
        this->dims = dims;
        this->cellMap = std::vector<std::vector<Cell*>>(dims[1], std::vector<Cell*>(dims[0], nullptr));
        this->emptyRows = std::vector<bool>(dims[1], true);
        this->emptyColumns = std::vector<bool>(dims[0], true);
    }

    void AddCell(Pos pos, char type) {
        Cell* cell = new Cell(pos, type);
        this->cellMap[pos.y][pos.x] = cell;

        if (type != '.') {
            this->emptyRows[pos.y] = false;
            this->emptyColumns[pos.x] = false;

            this->galaxies.push_back(cell);
        }
    }
    
    // void AddEmptyRow(int rowPos) {
    //     this->dims[1]++;
        
    //     std::vector<Cell*> newRow = std::vector<Cell*>(this->dims[0], nullptr);
    //     for (size_t i = 0; i < this->dims[0]; i++) {
    //         newRow[i] = new Cell(Pos(i, rowPos), '.');
    //     }
        
    //     this->cellMap.insert(this->cellMap.begin() + rowPos, newRow);

        
    //     for (size_t j = rowPos+1; j < this->dims[1]; j++) {
    //         for (size_t i = 0; i < this->dims[0]; i++) {
    //             Cell* cell = this->cellMap[j][i];
    //             cell->pos.y++;
    //         }
    //     }
    // }

    // void AddEmptyColumn(int columnPos) {
    //     this->dims[0]++;
    //     for (size_t i = 0; i < this->dims[1]; i++) {
    //         this->cellMap[i].insert(this->cellMap[i].begin() + columnPos, new Cell(Pos(columnPos, i), '.'));

    //         for (size_t j = columnPos+1; j < this->dims[0]; j++) {
    //             Cell* cell = this->cellMap[i][j];
    //             cell->pos.x++;
    //         }
    //     }
    // }

    // void Expand() {
    //     for (size_t i = 0; i < this->emptyRows.size(); i++) {
    //         if (this->emptyRows[i]) {
    //             this->AddEmptyRow(i);
    //         }
    //     }

    //     for (size_t i = 0; i < this->emptyColumns.size(); i++) {
    //         if (this->emptyColumns[i]) {
    //             this->AddEmptyColumn(i);
    //         }
    //     }
    // }

    long long CalculateDistance(Cell* cell, Cell* target) {
        int distance = std::abs(cell->pos.x - target->pos.x) + std::abs(cell->pos.y - target->pos.y);

        int minPosX = std::min(cell->pos.x, target->pos.x);
        int maxPosX = std::max(cell->pos.x, target->pos.x);
        int minPosY = std::min(cell->pos.y, target->pos.y);
        int maxPosY = std::max(cell->pos.y, target->pos.y);

        std::vector<int> expandingRows = std::vector<int>(this->emptyRows.begin() + minPosY, this->emptyRows.begin() + maxPosY + 1);
        std::vector<int> expandingColumns = std::vector<int>(this->emptyColumns.begin() + minPosX, this->emptyColumns.begin() + maxPosX + 1);
        
        long long addedDistance = 0;
        addedDistance += std::accumulate(expandingRows.begin(), expandingRows.end(), 0);
        addedDistance += std::accumulate(expandingColumns.begin(), expandingColumns.end(), 0);

        if (this->oldGalaxy) {
            addedDistance = addedDistance * 999999;
            // addedDistance *= 100;
        }

        return distance + addedDistance;
    }

    void CalculateDistances(Cell* cell) {
        cell->distances = {};

        for (Cell* target: this->galaxies) {
            if (cell != target) {
                cell->distances.push_back(CalculateDistance(cell, target));
            }
        }
    }

    void Log() {
        for (size_t i = 0; i < this->dims[1]; i++) {
            for (size_t j = 0; j < this->dims[0]; j++) {
                Cell* cell = this->cellMap[i][j];
                cell->Log();
            }
            Logger.log("\n");
        }
    }
};

void partA( std::vector<std::string>& input) {
    long long value = 0;
    std::vector<size_t> dims = {input[0].size(), input.size()};
    Cells cells = Cells(dims);

    for (size_t i = 0; i < input.size(); i++) {
        for (size_t j = 0; j < input[i].size(); j++) {
            Pos pos = Pos(j, i);
            cells.AddCell(pos, input[i][j]);
        }
    }

    for (Cell* cell: cells.galaxies) {
        cells.CalculateDistances(cell);
    }

    for (Cell* cell: cells.galaxies) {
        for (int distance: cell->distances) {
            value += distance;
        }
    }

    std::cout << "Value: " << value/2<< std::endl;
}

void partB( std::vector<std::string>& input) {
    long long value = 0;
    std::vector<size_t> dims = {input[0].size(), input.size()};
    Cells cells = Cells(dims);
    cells.oldGalaxy = true;

    for (size_t i = 0; i < input.size(); i++) {
        for (size_t j = 0; j < input[i].size(); j++) {
            Pos pos = Pos(j, i);
            cells.AddCell(pos, input[i][j]);
        }
    }

    for (Cell* cell: cells.galaxies) {
        cells.CalculateDistances(cell);
    }

    for (Cell* cell: cells.galaxies) {
        for (long long distance: cell->distances) {
            value += distance;
        }
    }

    std::cout << "Value: " << value/2 << std::endl;
}

int main() {
    std::string inputPath = sample ? "./day" + day + "/sample" : "./day" + day + "/input";
    std::filesystem::path inputFile(inputPath);

    std::vector<std::string> input = Helpers::ReadAllLinesInFile(inputFile);

    partA(input);
    partB(input);
}
