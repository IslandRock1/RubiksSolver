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
    std::map<std::array<unsigned int, 4>, std::vector<Move>> firstTwoLayers;
    std::map<std::array<unsigned int, 4>, std::vector<Move>> crossAnd2Corners;
    std::map<std::array<unsigned int, 4>, std::vector<Move>> crossAnd3Corners;

    void makeFirstTwoLayers(int depth);
    void makeCrossAnd2Corners(int depth);
    void makeCrossAnd3Corners(int depth);

    static bool prune(Move &currentMove, Move &prevMove, Move &doublePrevMove);
};


#endif //RUBIKSSOLVER_LOOKUP_HPP
