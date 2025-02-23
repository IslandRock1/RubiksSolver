
#include <iostream>
#include <vector>

#include "RubiksCube.hpp"
#include "Lookup.hpp"
#include "Solver.hpp"

int main() {
    std::vector<char> moves = {'J', 'B', 'D', 'K', 'A', 'M', 'H', 'Q', 'E', 'J', 'O', 'A', 'R', 'G', 'R', 'I', 'R', 'B', 'L', 'R', 'J', 'A', 'D', 'R', 'O', 'P', 'F', 'R', 'M'};

    auto solvingMoves = Move::convertVectorCharToMove(moves);

    Move::printMoves(solvingMoves);

}
