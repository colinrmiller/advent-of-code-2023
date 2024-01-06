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

std::string day = "10";
bool sample = false;
// bool sample = true;

Helpers::Logger Logger = Helpers::Logger("./day" + day + "/log");

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

    int Hash() {
        return this->x * 1000 + this->y;
    }
};

struct Cell {
    Pos pos;
    char type;
    std::vector<Cell*> adjacencies;
    int distance = -1;
    bool isInitial = false;

    Cell(Pos pos, char type) {
        this->pos = pos;
        this->type = type;
    }

    void Log() {
        Logger.log(std::string("") + this->type);
    }

    void setDistance(int distance) {
        this->distance = distance+1;
    }
};

struct Cells {
    std::vector<size_t> dims = {};
    std::vector<std::vector<Cell*>> cellMap;
    std::vector<int> initialPos = {-1, -1};
    std::vector<int> finalPos = {-1, -1};
    int finalDistance = -1;
    std::vector<std::vector<int>> bridges;

    Cells(std::vector<size_t> dims) {
        this->dims = dims;
        this->cellMap = std::vector<std::vector<Cell*>>(dims[1], std::vector<Cell*>(dims[0], nullptr));
        bridges = std::vector<std::vector<int>>(dims[1] * 2 - 1, std::vector<int>(dims[0] * 2 - 1, 0));
    }

    void AddCell(Pos pos, char type) {
        Cell* cell = new Cell(pos, type);
        this->cellMap[pos.y][pos.x] = cell;

        if (type == 'S') {
            this->initialPos = {pos.x, pos.y};
            cell->distance = 0;
            cell->isInitial = true;
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

    std::vector<int> GetConnections(char type) {
        std::vector<int> connections = std::vector<int>(4, 0);
        switch (type) {
            case '|':
                connections[2] = 1;
                connections[3] = 1;
                break;
            case '-':
                connections[0] = 1;
                connections[1] = 1;
                break;
            case 'J':
                connections[0] = 1;
                connections[2] = 1;
                break;
            case 'F':
                connections[1] = 1;
                connections[3] = 1;
                break;
            case '7':
                connections[0] = 1;
                connections[3] = 1;
                break;
            case 'L':
                connections[1] = 1;
                connections[2] = 1;
                break;
            default:
                break;
        }

        return connections;
    }

    // [Left, Right, Up, Down]
    std::vector<Cell*> GetAdjacencies(Cell* cell) {
        if (cell->isInitial) {
            return cell->adjacencies;
        }

        std::vector<Cell*> adjacencies = std::vector<Cell*>(4, nullptr);
        std::vector<int> connections = this->GetConnections(cell->type);

        //Left
        if (cell->pos.x > 0 && connections[0] == 1) {
            adjacencies[0] = this->cellMap[cell->pos.y][cell->pos.x - 1];
            this->bridges[cell->pos.y * 2][cell->pos.x * 2 - 1]++;
        }

        //Right
        if (cell->pos.x < this->dims[0] - 1 && connections[1] == 1) {
            adjacencies[1] = this->cellMap[cell->pos.y][cell->pos.x + 1];
            this->bridges[cell->pos.y * 2][cell->pos.x * 2 + 1]++;
        }

        //Up
        if (cell->pos.y > 0 && connections[2] == 1) {
            adjacencies[2] = this->cellMap[cell->pos.y - 1][cell->pos.x];
            this->bridges[cell->pos.y * 2 - 1][cell->pos.x * 2]++;
        }

        //Down
        if (cell->pos.y < this->dims[1] - 1 && connections[3] == 1) {
            adjacencies[3] = this->cellMap[cell->pos.y + 1][cell->pos.x];
            this->bridges[cell->pos.y * 2 +1][cell->pos.x * 2]++;
        }

        return adjacencies;
    }

    void GetInitialAdjacencies() {
        Cell* cell = this->cellMap[this->initialPos[1]][this->initialPos[0]];
        std::vector<Cell*> adjacencies = std::vector<Cell*>(4, nullptr);
        std::vector<int> connections = this->GetConnections(cell->type);

        //Left
        if (cell->pos.x > 0) {
            adjacencies[0] = this->cellMap[cell->pos.y][cell->pos.x - 1];
            this->bridges[cell->pos.y * 2][cell->pos.x * 2 - 1]++;
        }

        //Right
        if (cell->pos.x < this->dims[0] - 1) {
            adjacencies[1] = this->cellMap[cell->pos.y][cell->pos.x + 1];
            this->bridges[cell->pos.y * 2][cell->pos.x * 2 + 1]++;
        }

        //Up
        if (cell->pos.y > 0) {
            adjacencies[2] = this->cellMap[cell->pos.y - 1][cell->pos.x];
            this->bridges[cell->pos.y * 2 - 1][cell->pos.x * 2]++;
        }

        //Down
        if (cell->pos.y < this->dims[1] - 1) {
            adjacencies[3] = this->cellMap[cell->pos.y + 1][cell->pos.x];
            this->bridges[cell->pos.y * 2 +1][cell->pos.x * 2]++;
        }

        cell->adjacencies = adjacencies;
    }

    void LinkCells() {
        this->GetInitialAdjacencies();
        
        for (size_t i = 0; i < this->dims[0]; i++) {
            for (size_t j = 0; j < this->dims[0]; j++) {
                Cell* cell = this->cellMap[i][j];
                
                cell->adjacencies = this->GetAdjacencies(cell);
            }
            std::cout << std::endl;
        }
    }

    bool isBridgeConnective(Pos pos1, Pos pos2) {
        //above
        if (pos1.y - pos2.y == 1 && pos1.x == pos2.x) {
            return this->bridges[pos1.y * 2 - 1][pos1.x * 2] == 2;
        }
        //below
        if (pos2.y - pos1.y == 1 && pos1.x == pos2.x) {
            return this->bridges[pos1.y * 2 + 1][pos1.x * 2] == 2;
        }
        //left
        if (pos1.x - pos2.x == 1 && pos1.y == pos2.y) {
            return this->bridges[pos1.y * 2][pos1.x * 2 - 1] == 2;
        } 
        //right
        if (pos2.x - pos1.x == 1 && pos1.y == pos2.y) {
            return this->bridges[pos1.y * 2][pos1.x * 2 + 1] == 2;
        }

        return false;
    }

    void ExpandCell(Pos pos) {
        Cell* cell = this->cellMap[pos.y][pos.x];
        std::vector<Cell*> adjacencies = cell->adjacencies;
        for (size_t i = 0; i < adjacencies.size(); i++) {
            if (cell->pos.x == 0 && cell->pos.y == 2) {
                printf("here\n");
            }
            
            Cell* adj = adjacencies[i];
            if (adj == nullptr || !this->isBridgeConnective(cell->pos, adj->pos)) {
                continue;
            }

            if (adj->distance == -1) {
                adj->setDistance(cell->distance);
                this->ExpandCell(adj->pos);
            } else if (adj->distance == 0) {
                this->finalDistance = (cell->distance + 1) / 2;
            }
        }
    }

    void CalculateDistances() {
        this->ExpandCell(Pos(this->initialPos[0], this->initialPos[1]));
    }

    void LogDistances() {
        for (size_t i = 0; i < this->dims[1]; i++) {
            for (size_t j = 0; j < this->dims[0]; j++) {
                Cell* cell = this->cellMap[i][j];
                Logger.log(std::to_string(cell->distance));
                if (cell->distance == -1) {
                    Logger.log("");
                } else {
                    Logger.log(" ");
                }
            }
            Logger.log("\n");
        }
    }
};

void partA( std::vector<std::string>& input) {
    int value = 0;
    std::vector<size_t> dims = {input[0].size(), input.size()};
    Cells cells = Cells(dims);

    for (size_t i = 0; i < input.size(); i++) {
        for (size_t j = 0; j < input[i].size(); j++) {
            Pos pos = Pos(j, i);
            cells.AddCell(pos, input[i][j]);
        }
    }

    cells.LinkCells();
    cells.Log();

    cells.CalculateDistances();
    cells.LogDistances();

    value = cells.finalDistance;

    std::cout << "Value: " << value << std::endl;
}

int main() {
    std::string inputPath = sample ? "./day" + day + "/sample" : "./day" + day + "/input";
    std::filesystem::path inputFile(inputPath);

    std::vector<std::string> input = Helpers::ReadAllLinesInFile(inputFile);
    printf("input: %s\n", input[0].c_str());


    partA(input);
    // partB(input);
}
