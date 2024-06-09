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
    std::array<short, 10> currPos = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
};

class Lookup {
public:
    std::map<std::array<unsigned int, 4>, std::vector<char>> firstTwoLayers;
    std::map<std::array<unsigned int, 4>, std::vector<char>> crossAnd2Corners;
    std::map<std::array<unsigned int, 4>, std::vector<char>> crossAnd3Corners;
    std::map<std::array<unsigned int, 4>, std::vector<char>> solveLastLayer;
    std::map<std::array<unsigned int, 4>, std::vector<char>> solveTwoLayer;
    std::map<std::array<unsigned int, 4>, std::vector<char>> wholeCube;

    void makeFirstTwoLayers(int depth);
    void makeCrossAnd2Corners(int depth);
    void makeCrossAnd3Corners(int depth);
    void makeWholeCube(int depth);

    static bool prune(Move &currentMove, Move &prevMove, Move &doublePrevMove);

    static Move charToMove(char move);
    static char moveToChar(Move move);

    static void save(std::map<std::array<unsigned int, 4>, std::vector<char>> &map, std::string &title);
    static void load(std::map<std::array<unsigned int, 4>, std::vector<char>> &map, std::string &title);
};


#endif //RUBIKSSOLVER_LOOKUP_HPP
