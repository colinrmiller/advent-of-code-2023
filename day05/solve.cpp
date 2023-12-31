#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <exception>
#include <typeinfo>
#include <unordered_set>
#include "../helpers/helpers.h"

struct Range {
    long long destRangeStart;
    long long sourceRangeStart;
    long long sourceRangeEnd;
    long long rangeLength;
    long long shift;
    int pos;

    Range(long long destRangeStart, long long sourceRangeStart, long long rangeLength, int pos = 0) {
        this->destRangeStart = destRangeStart;
        this->sourceRangeStart = sourceRangeStart;
        this->sourceRangeEnd = sourceRangeStart + rangeLength;
        this->rangeLength = rangeLength;
        this->shift = destRangeStart - sourceRangeStart;
        this->pos = pos;

        // printf("pos: %d,destRangeStart: %d, sourceRangeStart: %d, sourceRangeEnd: %d, rangeLength: %d, shift: %d\n", pos, destRangeStart, sourceRangeStart, sourceRangeEnd, rangeLength, shift);
    }
};

struct Map {
    std::vector<Range> ranges;
    int id;
    int rangeCount;

    Map() {
        this->ranges = {};
        this->id = std::rand();
        // printf("map id: %d\n", this->id);
        this->rangeCount = 0;
    }

    void addRange(Range range) {
        Range rangeCopy = Range(range.destRangeStart, range.sourceRangeStart, range.rangeLength, this->rangeCount);
        this->rangeCount++;
        this->ranges.push_back(rangeCopy);
    }

    long long getDest(long long source) {
        for (Range range : this->ranges) {
            if (source >= range.sourceRangeStart && source < range.sourceRangeEnd) {
                // printf("map: %d, pos: %d, source: %d, shift: %d\n", this->id, range.pos, source, range.shift);
                return source + range.shift;
            }
        }
        return source;
    }
};

void partA( std::vector<std::string>& input) {
    std::vector<long long> seeds = {};
    std::vector<Map> maps = {};
    
    long long value = std::numeric_limits<long long>::max();

    seeds = Helpers::ParseStringToLongVector(input[0], ' ');
    input.erase(input.begin());

    Map map = {};
    for (std::string line : input) {
        // printf("line: %s\n", line.c_str());
        std::vector<long long> lineVals = Helpers::ParseStringToLongVector(line, ' ');
        for (long long val : lineVals) {
            printf("val: %lld\n", val);
        }
        if (lineVals.size() == 0) {
            if (map.ranges.size() > 0) {
                Map newMap;
                newMap = map;
                maps.emplace_back(newMap);
                map = {};
            }
            // std::cout << "New map" << std::endl;
            continue;
        }
        printf("lineVals: %lld, %lld, %lld\n", lineVals[0], lineVals[1], lineVals[2]);
        map.addRange(Range(lineVals[0], lineVals[1], lineVals[2]));
    }

    for (long long seed : seeds) {
        // printf("seed -> %d\n", seed);
        for (Map map : maps) {
            seed = map.getDest(seed);
            // printf("-> seed: %d\n", seed);
        }
        if (seed < value) {
            value = seed;
        }
        // std::cout << "Seed: " << seed  << std::endl;
    }

    std::cout << "Value: " << value << std::endl;
}

void partB( std::vector<std::string>& input) {
    std::vector<long long> seeds = {};
    std::vector<Map> maps = {};
    
    long long value = std::numeric_limits<long long>::max();

    seeds = Helpers::ParseStringToLongVector(input[0], ' ');
    input.erase(input.begin());

    Map map = {};
    for (std::string line : input) {
        // printf("line: %s\n", line.c_str());
        std::vector<long long> lineVals = Helpers::ParseStringToLongVector(line, ' ');
        for (long long val : lineVals) {
            printf("val: %lld\n", val);
        }
        if (lineVals.size() == 0) {
            if (map.ranges.size() > 0) {
                Map newMap;
                newMap = map;
                maps.emplace_back(newMap);
                map = {};
            }
            // std::cout << "New map" << std::endl;
            continue;
        }
        printf("lineVals: %lld, %lld, %lld\n", lineVals[0], lineVals[1], lineVals[2]);
        map.addRange(Range(lineVals[0], lineVals[1], lineVals[2]));
    }

    for (size_t i = 0; i < seeds.size(); i += 2) {
        printf("i: %d\n", i);
        long long seedStart = seeds[i];
        long long seedRange = 1;
        if (i + 1 < seeds.size()) {
            printf("i+1: %lld, %lld\n", i+1, seeds[i + 1]);
            seedRange = seeds[i + 1];
        }

        printf("seedStart: %lld, seedRange: %lld\n", seedStart, seedRange);
        for (long long seed = seedStart; seed < seedStart + seedRange; seed++) {
            // printf("seed -> %d\n", seed);
            long long result;
            for (Map map : maps) {
                result = map.getDest(seed);
                // printf("-> seed: %d\n", seed);
            }
            if (result < value) {
                value = result;
            }
            // std::cout << "Seed: " << seed  << std::endl;
        }
    }

    std::cout << "Value: " << value << std::endl;
}

int main() {
    std::filesystem::path inputFile("./day05/input");
    // std::filesystem::path inputFile("./day05/sample");
    std::vector<std::string> input = Helpers::ReadAllLinesInFile(inputFile);
    printf("input: %s\n", input[0].c_str());


    // partA(input);
    partB(input);
}
