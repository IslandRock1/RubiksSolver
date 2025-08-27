//
// Created by Øystein Bringsli.
//

#ifndef RUBIKSSOLVER_LOOKUP_HPP
#define RUBIKSSOLVER_LOOKUP_HPP

#include <map>
#include <array>
#include <vector>

#include "RubiksCube.hpp"

struct Position {
    std::array<int, 10> currPos = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
};

class Lookup {
public:
    std::map<std::array<unsigned int, 4>, std::vector<char>> firstTwoLayers;
    std::map<std::array<unsigned int, 4>, std::vector<char>> crossAnd2Corners;
    std::map<std::array<unsigned int, 4>, std::vector<char>> crossAnd3Corners;
    std::map<std::array<unsigned int, 4>, std::vector<char>> solveLastLayer;
    std::map<std::array<unsigned int, 4>, std::vector<char>> solveTwoLayer;
    std::map<std::array<unsigned int, 4>, std::vector<char>> wholeCube;
    std::map<std::array<unsigned int, 4>, std::vector<char>> combined;
    std::map<std::array<unsigned int, 4>, std::vector<char>> solveFromCrossAnd2Corners;

    void makeFirstTwoLayers(int depth);
    void makeCrossAnd2Corners(int depth);
    void makeCrossAnd3Corners(int depth);
    void makeWholeCube(int depth);

    static bool prune(Move &currentMove, Move &prevMove, Move &doublePrevMove);

    static void save(std::map<std::array<unsigned int, 4>, std::vector<char>> &map, std::string &title);
    static void load(std::map<std::array<unsigned int, 4>, std::vector<char>> &map, std::string &title);

    static uint64_t getSize(std::map<std::array<unsigned int, 4>, std::vector<char>> &map);
    static uint64_t getSize(std::map<uint64_t, std::vector<char>> &map);
    static uint64_t getSize(std::map<uint64_t, uint32_t> &map);
    static uint64_t getSize(std::map<std::pair<uint32_t, uint16_t>, uint32_t> &map);
    static Lookup loadAllMaps();
};


#endif //RUBIKSSOLVER_LOOKUP_HPP
