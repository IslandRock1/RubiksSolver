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

void generateLookupFirstTwoLayers(
        std::map<std::array<unsigned int, 4>, std::vector<Move>> &map,
        std::vector<Move> &moves,
        RubiksCube &cube,
        int depth
        )
{
    if (depth == 0) { return;}

    auto vals = cube.hashFirstTwoLayers();

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
        generateLookupFirstTwoLayers(map, moves, cube, depth - 1);
        cube.turn(m.face, 4 - m.rotations);
        moves.pop_back();
    }
}

void generateLookupCrossAnd2Corners(
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
        generateLookupCrossAnd2Corners(map, moves, cube, depth - 1);
        cube.turn(m.face, 4 - m.rotations);
        moves.pop_back();
    }
}

void generateLookupCrossAnd3Corners(
        std::map<std::array<unsigned int, 4>, std::vector<Move>> &map,
        std::vector<Move> &moves,
        RubiksCube &cube,
        int depth
)
{
    if (depth == 0) { return;}

    auto vals = cube.hashCrossAnd3Corners();

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
        generateLookupCrossAnd3Corners(map, moves, cube, depth - 1);
        cube.turn(m.face, 4 - m.rotations);
        moves.pop_back();
    }
}

void Lookup::makeFirstTwoLayers(int depth) {
    auto start = std::chrono::high_resolution_clock::now();
    RubiksCube cube;
    std::vector<Move> moves;
    generateLookupFirstTwoLayers(firstTwoLayers, moves, cube, depth + 1);

    auto end = std::chrono::high_resolution_clock::now();
    auto durLookup = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "Size of first 2 layers table is " << firstTwoLayers.size() << " in " << durLookup.count() / 1000 / 1000 << " seconds." << "\n";
}

void Lookup::makeCrossAnd2Corners(int depth) {
    auto start = std::chrono::high_resolution_clock::now();
    RubiksCube cube;
    std::vector<Move> moves;
    generateLookupCrossAnd2Corners(crossAnd2Corners, moves, cube, depth + 1);

    auto end = std::chrono::high_resolution_clock::now();
    auto durLookup = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "Size of 2 corner table is " << crossAnd2Corners.size() << " in " << durLookup.count() / 1000 / 1000 << " seconds." << "\n";
}

void Lookup::makeCrossAnd3Corners(int depth) {
    auto start = std::chrono::high_resolution_clock::now();
    RubiksCube cube;
    std::vector<Move> moves;
    generateLookupCrossAnd3Corners(crossAnd3Corners, moves, cube, depth + 1);

    auto end = std::chrono::high_resolution_clock::now();
    auto durLookup = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "Size of 3 corner table is " << crossAnd3Corners.size() << " in " << durLookup.count() / 1000 / 1000 << " seconds." << "\n";
}

