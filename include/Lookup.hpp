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
    Lookup(int depth);
    std::map<std::array<unsigned int, 4>, std::vector<Move>> firstTwoLayers;

    static bool prune(Move &currentMove, Move &prevMove, Move &doublePrevMove);
};


#endif //RUBIKSSOLVER_LOOKUP_HPP
