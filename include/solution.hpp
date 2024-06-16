//
// Created by Øystein Bringsli.
//

#ifndef RUBIKSSOLVER_SOLUTION_HPP
#define RUBIKSSOLVER_SOLUTION_HPP

#include <array>
#include <vector>
#include "Move.hpp"

class Solution {
public:
    std::vector<Move> crossMoves;
    std::vector<Move> twoLayerMoves;
    std::vector<Move> lastLayerMoves;
    std::array<short, 48> shuffledCube;
    std::array<short, 48> crossAndTwoCube;
    std::array<short, 48> twoLayerCube;
};


#endif //RUBIKSSOLVER_SOLUTION_HPP
