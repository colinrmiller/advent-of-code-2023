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
};

struct Cell {
    Pos pos;
    char type;
    std::vector<Cell*> adjacencies;
    int distance = -1;
    bool isInitial = false;
    // wrapperNum: count the number of times the main loop traverses (up on the left + right on the up) - (down on the right + left on the down)
    // determines whether the cell is contained in the loop (wrapperNum != 0 && / 4) or not (wrapperNum == 0)
    int wrapperNum = 0; 
    bool onLoop = false;

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
    std::vector<Pos> loop;

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
        
        for (size_t i = 0; i < this->dims[1]; i++) {
            for (size_t j = 0; j < this->dims[0]; j++) {
                Cell* cell = this->cellMap[i][j];
                
                cell->adjacencies = this->GetAdjacencies(cell);
            }
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

    void ExpandCell(Pos pos, std::vector<Pos> visited = {}) {
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
                visited.push_back(adj->pos);
                // clone visited
                std::vector newVisited(visited);
                this->ExpandCell(adj->pos, visited);
            } else if (adj->distance == 0) { // if returning to starting cell
                std::vector<Pos> finalVisited(visited);
                finalVisited.push_back(Pos(this->initialPos[0], this->initialPos[1]));
                this->loop = finalVisited;
                this->finalDistance = (cell->distance + 1) / 2;
                
                this->SetLoop();
            }
        }
    }

    void CalculateDistances() {
        Pos initialPos = Pos(this->initialPos[0], this->initialPos[1]);
        this->ExpandCell(initialPos, { initialPos });
    }

    void IncrementWrapperNums(Pos prev, Pos posStart, Pos posEnd) {
        Cell* cell = this->cellMap[posStart.y][posStart.x];
        bool exitDown = posStart.y < posEnd.y;
        bool exitUp = posStart.y > posEnd.y;
        bool exitLeft = posStart.x > posEnd.x;
        bool exitRight = posStart.x < posEnd.x;
        bool enterDown = prev.y > posStart.y;
        bool enterUp = prev.y < posStart.y;
        bool enterLeft = prev.x < posStart.x;
        bool enterRight = prev.x > posStart.x;

        if (exitDown || enterUp) {
            for (size_t i = 0; i < posStart.x; i++) {
                Cell* cell = this->cellMap[posStart.y][i];
                cell->wrapperNum += 1 * exitDown + 1 * enterUp;
            }

            for (size_t i = posStart.x + 1; i < this->dims[0]; i++) {
                Cell* cell = this->cellMap[posStart.y][i];
                cell->wrapperNum -= 1 * exitDown + 1 * enterUp;
            }
        } 
        if (exitUp || enterDown) {
            for (size_t i = 0; i < posStart.x; i++) {
                Cell* cell = this->cellMap[posStart.y][i];
                cell->wrapperNum -= 1 * exitUp + 1 * enterDown;
            }

            for (size_t i = posStart.x + 1; i < this->dims[0]; i++) {
                Cell* cell = this->cellMap[posStart.y][i];
                cell->wrapperNum += 1 * exitUp + 1 * enterDown;
            }
        } 
        if (exitLeft || enterRight) {
            for (size_t i = 0; i < posStart.y; i++) {
                Cell* cell = this->cellMap[i][posStart.x];
                cell->wrapperNum += 1 * exitLeft + 1 * enterRight;
            }

            for (size_t i = posStart.y + 1; i < this->dims[1]; i++) {
                Cell* cell = this->cellMap[i][posStart.x];
                cell->wrapperNum -= 1 * exitLeft + 1 * enterRight;
            }
        }
        if (exitRight || enterLeft) {
            for (size_t i = 0; i < posStart.y; i++) {
                Cell* cell = this->cellMap[i][posStart.x];
                cell->wrapperNum -= 1 * exitRight + 1 * enterLeft;
            }

            for (size_t i = posStart.y + 1; i < this->dims[1]; i++) {
                Cell* cell = this->cellMap[i][posStart.x];
                cell->wrapperNum += 1 * exitRight + 1 * enterLeft;
            }
        }
    }

    void SetLoop() {
        for (size_t i = 0; i < this->loop.size(); i++) {
            Pos pos = this->loop[i];
            Cell* cell = this->cellMap[pos.y][pos.x];
            cell->onLoop = true;
        }
    }

    // TODO: doesn't adjust for variable length of numbers
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

    void LogWrapperNums() {
        for (size_t i = 0; i < this->dims[1]; i++) {
            for (size_t j = 0; j < this->dims[0]; j++) {
                Cell* cell = this->cellMap[i][j];
                if (cell->onLoop) {
                    std::string stringType(1, cell->type);
                    Logger.log(stringType + " ");
                } else {
                    Logger.log(std::to_string(cell->wrapperNum) + " ");
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

void partB( std::vector<std::string>& input) {
    std::vector<size_t> dims = {input[0].size(), input.size()};
    Cells cells = Cells(dims);

    for (size_t i = 0; i < input.size(); i++) {
        for (size_t j = 0; j < input[i].size(); j++) {
            Pos pos = Pos(j, i);
            cells.AddCell(pos, input[i][j]);
        }
    }

    cells.LinkCells();

    cells.CalculateDistances();

    cells.loop.insert(cells.loop.begin(), cells.loop[cells.loop.size()-2]);
    for (size_t i = 1; i < cells.loop.size() - 1; i++) { // initial cell is visited twice so we can ignore the last cell
        cells.IncrementWrapperNums(cells.loop[i-1], cells.loop[i], cells.loop[i+1]);
    }

    cells.LogWrapperNums();

    int value = 0;
    for (size_t i = 0; i < cells.dims[1]; i++) {
        for (size_t j = 0; j < cells.dims[0]; j++) {
            Cell* cell = cells.cellMap[i][j];
            if (cell->wrapperNum != 0 && !cell->onLoop) {
                value++;
            }
        }
    }
    std::cout << "Value: " << value << std::endl;
}
int main() {
    std::string inputPath = sample ? "./day" + day + "/sample" : "./day" + day + "/input";
    std::filesystem::path inputFile(inputPath);

    std::vector<std::string> input = Helpers::ReadAllLinesInFile(inputFile);
    printf("input: %s\n", input[0].c_str());


    // partA(input);
    partB(input);
}
