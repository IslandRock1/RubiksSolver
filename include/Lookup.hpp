//
// Created by Øystein Bringsli.
//

#ifndef RUBIKSSOLVER_LOOKUP_HPP
#define RUBIKSSOLVER_LOOKUP_HPP

#include <map>
#include <array>
#include <vector>

#include "RubiksCube.hpp"

class Lookup {
public:
    std::map<std::array<unsigned int, 4>, std::vector<char>> firstTwoLayers;
    std::map<std::array<unsigned int, 4>, std::vector<char>> crossAnd2Corners;
    std::map<std::array<unsigned int, 4>, std::vector<char>> crossAnd3Corners;

    void makeFirstTwoLayers(int depth);
    void makeCrossAnd2Corners(int depth);
    void makeCrossAnd3Corners(int depth);

    static bool prune(Move &currentMove, Move &prevMove, Move &doublePrevMove);

    static Move charToMove(char move);
    static char moveToChar(Move move);
};


#endif //RUBIKSSOLVER_LOOKUP_HPP
