#include <iostream>
#include <chrono>
#include <bitset>
#include <map>
#include <stdexcept>

#include "RubiksCube.hpp"
#include "Lookup.hpp"

std::vector<Move> addMovesWithLookup(std::vector<Move> &moves, std::vector<Move> &lookupMoves, bool reverse = true) {
    std::vector<Move> outMoves;

    for (auto m : moves) {
        outMoves.emplace_back(m);
    }

    if (reverse) {
        std::reverse(lookupMoves.begin(), lookupMoves.end());
    }

    for (auto m : lookupMoves) {
        outMoves.push_back({m.face, 4 - m.rotations});
    }

    return outMoves;
}

enum Hash {
    Two,
    Three,
    Full
};

std::vector<Move> execEveryMove(
        RubiksCube &cube,
        int depth,
        std::vector<Move> &moves,
        std::map<std::array<unsigned int, 4>, std::vector<Move>> &lookup,
        Hash hash
        )
{

    std::_Rb_tree_iterator<std::pair<const std::array<unsigned int, 4>, std::vector<Move>>> lookupSolution;
    switch (hash) {
        case Two:
        {
            lookupSolution = lookup.find(cube.hashCrossAnd2Corners());
            if (lookupSolution != lookup.end()) {

//                auto tmp = lookup[cube.hashCrossAnd2Corners()];
//                std::reverse(tmp.begin(), tmp.end());
//
//                for (auto m : tmp) {
//                    cube.turn(m.face, 4 - m.rotations);
//                }

                return addMovesWithLookup(moves, lookup[cube.hashCrossAnd2Corners()]);
            }
        } break;

        case Three:
        {
            lookupSolution = lookup.find(cube.hashCrossAnd3Corners());
            if (lookupSolution != lookup.end()) {

//                auto tmp = lookup[cube.hashCrossAnd3Corners()];
//                std::reverse(tmp.begin(), tmp.end());
//
//                for (auto m : tmp) {
//                    cube.turn(m.face, 4 - m.rotations);
//                }

                return addMovesWithLookup(moves, lookup[cube.hashCrossAnd3Corners()]);
            }
        } break;

        case Full:
        {
            lookupSolution = lookup.find(cube.hashFirstTwoLayers());
            if (lookupSolution != lookup.end()) {

//                auto tmp = lookup[cube.hashFirstTwoLayers()];
//                std::reverse(tmp.begin(), tmp.end());
//
//                for (auto m : tmp) {
//                    cube.turn(m.face, 4 - m.rotations);
//                }

                return addMovesWithLookup(moves, lookup[cube.hashFirstTwoLayers()]);
            }
        }
    }

    if (depth == 0) {
        return {};
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

    for (Move m : RubiksConst::everyMove)
    {
        if (Lookup::prune(m, prevMove, doublePrevMove)) { continue;}

        moves.push_back(m);
        cube.turn(m.face, m.rotations);

        auto fromRecursed = execEveryMove(cube, depth - 1, moves, lookup, hash);

        cube.turn(m.face, 4 - m.rotations);
        moves.pop_back();

        if (!fromRecursed.empty()) {
            return fromRecursed;
        }
    }

    return {};
}

void printMoves(std::vector<Move> &moves) {
    std::cout << "Moves: ";

    for (auto m : moves) {
        switch (m.face) {
            case 0:
            {
                std::cout << "U";
            } break;

            case 1:
            {
                std::cout << "L";
            } break;

            case 2:
            {
                std::cout << "F";
            } break;

            case 3:
            {
                std::cout << "B";
            } break;

            case 4:
            {
                std::cout << "R";
            } break;

            case 5:
            {
                std::cout << "D";
            } break;
        }

        std::cout << m.rotations << " ";
    }

    std::cout << "\n";
}

void testMoveSpeed() {
    RubiksCube cube;

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 10000000; i++) {
        cube.turn(3, 2);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Time taken by function: " << duration.count() << " microseconds" << std::endl;
}

void printHash(std::array<unsigned int, 4> &hash, bool bit = false) {
    if (bit) {
        std::cout << std::bitset<32>(hash[0]) << " " << std::bitset<32>(hash[1]) << " " << std::bitset<32>(hash[2]) << " " << std::bitset<32>(hash[3]) << "\n";
    } else {
        std::cout << hash[0] << " " << hash[1] << " " << hash[2] << " " << hash[3] << "\n";
    }
}

void testHashSpeed() {
    RubiksCube cube;

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000000; i++) {
        cube.hashCrossAnd3Corners();
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Time taken by hash-function: " << duration.count() / 1000 << " milliseconds" << std::endl;
}

void printMapSize(const std::map<std::array<unsigned int, 4>, std::vector<Move>>& myMap) {
    auto start = std::chrono::high_resolution_clock::now();
    std::size_t totalSize = 0;

    for (const auto& entry : myMap) {
        totalSize += sizeof(entry.first); // Size of the key
        for (const auto& move : entry.second) {
            totalSize += sizeof(move); // Size of each Move object
        }
    }

    // Convert total size to kilobytes
    double totalSizeKB = static_cast<double>(totalSize) / 1024.0;
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "Total size of map: " << std::fixed << std::setprecision(2) << totalSizeKB << " KB. Size found in " << duration.count() / 1000 << " milliseconds." << std::endl;
}

std::vector<Move> solveCrossAnd2Corners(RubiksCube &cube, Lookup &lookup) {
    if ((cube.numCornerSolved() == 2) && cube.solvedWhiteCross()) {return {};}

    auto lookupSolution = lookup.crossAnd2Corners.find(cube.hashCrossAnd2Corners());
    if (lookupSolution != lookup.crossAnd2Corners.end()) {
        std::vector<Move> outMoves;

        auto hashMoves = lookup.crossAnd2Corners[cube.hashCrossAnd2Corners()];
        std::reverse(hashMoves.begin(), hashMoves.end());

        for (auto m : hashMoves) {
            outMoves.push_back({m.face, 4 - m.rotations});
        }

        std::cout << "Found solution with lookup" << "\n";
        return outMoves;
    }

    std::vector<Move> moves;
    for (unsigned short depth = 4; depth < 10; depth++) {
        auto solvingMoves = execEveryMove(cube, depth, moves, lookup.crossAnd2Corners, Hash::Two);
        if (!solvingMoves.empty()) {
            return solvingMoves;
        }
    }

    throw std::runtime_error("No solution found for this depth-limit and lookup combo.");
}

std::vector<Move> solveCrossAnd3Corners(RubiksCube &cube, Lookup &lookup) {
    if ((cube.numCornerSolved() == 3) && cube.solvedWhiteCross()) {return {};}

    auto lookupSolution = lookup.crossAnd3Corners.find(cube.hashCrossAnd3Corners());
    if (lookupSolution != lookup.crossAnd3Corners.end()) {
        std::vector<Move> outMoves;

        auto hashMoves = lookup.crossAnd3Corners[cube.hashCrossAnd3Corners()];
        std::reverse(hashMoves.begin(), hashMoves.end());

        for (auto m : hashMoves) {
            outMoves.push_back({m.face, 4 - m.rotations});
        }

        std::cout << "Found solution with lookup" << "\n";
        return outMoves;
    }

    std::vector<Move> moves;
    for (unsigned short depth = 4; depth < 10; depth++) {
        auto solvingMoves = execEveryMove(cube, depth, moves, lookup.crossAnd3Corners, Hash::Three);
        if (!solvingMoves.empty()) {
            return solvingMoves;
        }
    }

    throw std::runtime_error("No solution found for this depth-limit and lookup combo.");
}

std::vector<Move> solveFirstTwoLayers(RubiksCube &cube, Lookup &lookup) {
    if ((cube.numCornerSolved() == 4) && cube.solvedWhiteCross()) {return {};}

    auto lookupSolution = lookup.firstTwoLayers.find(cube.hashFirstTwoLayers());
    if (lookupSolution != lookup.firstTwoLayers.end()) {
        std::vector<Move> outMoves;

        auto hashMoves = lookup.firstTwoLayers[cube.hashFirstTwoLayers()];
        std::reverse(hashMoves.begin(), hashMoves.end());

        for (auto m : hashMoves) {
            outMoves.push_back({m.face, 4 - m.rotations});
        }

        std::cout << "Found solution with lookup" << "\n";
        return outMoves;
    }

    std::vector<Move> moves;
    for (unsigned short depth = 4; depth < 10; depth++) {
        auto solvingMoves = execEveryMove(cube, depth, moves, lookup.firstTwoLayers, Hash::Full);
        if (!solvingMoves.empty()) {
            return solvingMoves;
        }
    }

    throw std::runtime_error("No solution found for this depth-limit and lookup combo.");
}

int main() {
    Lookup lookup;

//    lookup.makeCrossAnd2Corners(5);
//    printMapSize(lookup.crossAnd2Corners);
    lookup.makeCrossAnd3Corners(6);
    printMapSize(lookup.crossAnd3Corners);

    return 69;
//    lookup.makeFirstTwoLayers(6);
//    printMapSize(lookup.firstTwoLayers);

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 10; i++) {
        RubiksCube cube;

        auto shuffleMoves = cube.shuffle(30);
        std::cout << "Shuffling moves..." << "\n";
        printMoves(shuffleMoves);

        auto movesCross3Corners = solveCrossAnd3Corners(cube, lookup);
        std::cout << "Three corners found with these moves:" << "\n";
        printMoves(movesCross3Corners);
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Found solutions in " << duration / 1000 << " milliseconds." << std::endl;

//    auto start = std::chrono::high_resolution_clock::now();
//    auto movesCross2Corners = solveCrossAnd2Corners(cube, lookup);
//    std::cout << "Two corners found with these moves:" << "\n";
//    printMoves(movesCross2Corners);
//    auto movesCross3Corners = solveCrossAnd3Corners(cube, lookup);
//    std::cout << "Three corners found with these moves:" << "\n";
//    printMoves(movesCross3Corners);
//    auto movesFullLayers = solveFirstTwoLayers(cube, lookup);
//    std::cout << "Full layer found with these moves:" << "\n";
//    printMoves(movesFullLayers);
//
//    auto end = std::chrono::high_resolution_clock::now();
//    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
//    std::cout << "Found solution in " << duration / 1000 << " milliseconds." << std::endl;
//
//    std::cout << "Solving moves..." << "\n";
//
//    std::vector<Move> moves;
//    for (auto m : movesCross2Corners) {
//        moves.push_back(m);
//    }
//
//    for (auto m : movesCross3Corners) {
//        moves.push_back(m);
//    }
//
//    for (auto m : movesFullLayers) {
//        moves.push_back(m);
//    }
//
//    printMoves(moves);
}

