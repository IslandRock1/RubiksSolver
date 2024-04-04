//
// Created by Øystein Bringsli.
//

#include <iostream>
#include <chrono>

#include "Lookup.hpp"

bool Lookup::prune(Move &currentMove, Move &prevMove, Move &doublePrevMove) {
    if (currentMove.face == prevMove.face) { return true;}

    if ((currentMove.face == doublePrevMove.face) && (RubiksConst::oppositeFaceAll[currentMove.face] == prevMove.face)) {
        return true;
    }

    if (currentMove.face < 3) {
        if (prevMove.face == RubiksConst::oppositeFace[currentMove.face]) {
            return true;
        }
    }

    return false;
}

void generateLookup(
        std::map<std::array<unsigned int, 4>, std::vector<Move>> &map,
        std::vector<Move> &moves,
        RubiksCube &cube,
        int depth
        )
{
    if (depth == 0) { return;}

    auto vals = cube.hashCrossAnd2Corners();

    if (map.find(vals) != map.end()) {
        if (moves.size() < map[vals].size()) {
            map[vals] = moves;
        }
    } else {
        map[vals] = moves;
    }

    auto size = moves.size();
    Move prevMove {7, 7};
    Move doublePrevMove {7, 7};

    if (size > 1) {
        prevMove = moves[size - 1];
        doublePrevMove = moves[size - 2];
    } else if (size > 0) {
        prevMove = moves[size - 1];
    }

    for (auto m : RubiksConst::everyMove) {
        if (Lookup::prune(m, prevMove, doublePrevMove)) { continue;}

        moves.push_back(m);
        cube.turn(m.face, m.rotations);
        generateLookup(map, moves, cube, depth - 1);
        cube.turn(m.face, 4 - m.rotations);
        moves.pop_back();
    }
}

Lookup::Lookup(int depth) {
    RubiksCube cube;
    std::vector<Move> moves;

    auto start = std::chrono::high_resolution_clock::now();
    generateLookup(firstTwoLayers, moves, cube, depth + 1);
    auto end = std::chrono::high_resolution_clock::now();
    auto durLookup = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "Size of lookup table is " << firstTwoLayers.size() << " in " << durLookup.count() / 1000 / 1000 << " seconds." << "\n";
}
