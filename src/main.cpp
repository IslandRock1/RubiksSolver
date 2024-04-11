#include <iostream>
#include <chrono>
#include <bitset>
#include <map>
#include <stdexcept>

#include "RubiksCube.hpp"
#include "Lookup.hpp"

std::vector<Move> addMovesWithLookup(const std::vector<Move> moves, std::vector<Move> lookupMoves, bool reverse = true) {
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

std::vector<char> convertVectorMovesToChar(std::vector<Move> moves) {
    std::vector<char> out;

    for (auto m : moves) {
        out.push_back(Lookup::moveToChar(m));
    }

    return out;
}

std::vector<Move> convertVectorCharToMove(std::vector<char> moves) {
    std::vector<Move> out;

    for (auto m : moves) {
        out.push_back(Lookup::charToMove(m));
    }

    return out;
}

std::vector<Move> execEveryMove(
        RubiksCube &cube,
        int depth,
        std::vector<Move> &moves,
        std::map<std::array<unsigned int, 4>, std::vector<char>> &lookup,
        Hash hash
        )
{

    std::_Rb_tree_iterator<std::pair<const std::array<unsigned int, 4>, std::vector<char>>> lookupSolution;
    switch (hash) {
        case Two:
        {
            lookupSolution = lookup.find(cube.hashCrossAnd2Corners());
            if (lookupSolution != lookup.end()) {
                return addMovesWithLookup(moves, convertVectorCharToMove(lookup[cube.hashCrossAnd2Corners()]));
            }
        } break;

        case Three:
        {
            lookupSolution = lookup.find(cube.hashCrossAnd3Corners());
            if (lookupSolution != lookup.end()) {
                return addMovesWithLookup(moves, convertVectorCharToMove(lookup[cube.hashCrossAnd3Corners()]));
            }
        } break;

        case Full:
        {
            lookupSolution = lookup.find(cube.hashFirstTwoLayers());
            if (lookupSolution != lookup.end()) {
                return addMovesWithLookup(moves, convertVectorCharToMove(lookup[cube.hashFirstTwoLayers()]));
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

//    std::cout << "Returned from latest" << "\n";
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

void printMapSize(const std::map<std::array<unsigned int, 4>, std::vector<char>>& myMap) {
    auto start = std::chrono::high_resolution_clock::now();
    std::size_t totalSize = 0;

    for (const auto& entry : myMap) {
        totalSize += sizeof(entry.first); // Size of the key
        for (const auto& move : entry.second) {
            totalSize += sizeof(move); // Size of each Move object
        }
    }

    // Convert total size to megabytes
    double totalSizeMB = static_cast<double>(totalSize) / 1024.0 / 1024.0;
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "Total size of map: " << std::fixed << std::setprecision(2) << totalSizeMB << " MB. Size found in " << duration.count() / 1000 << " milliseconds." << std::endl;
}

std::vector<Move> solveCrossAnd2Corners(RubiksCube &cube, Lookup &lookup) {
    if ((cube.numCornerSolved() == 2) && cube.solvedWhiteCross()) {return {};}

    auto lookupSolution = lookup.crossAnd2Corners.find(cube.hashCrossAnd2Corners());
    if (lookupSolution != lookup.crossAnd2Corners.end()) {
        std::vector<Move> outMoves;

        auto hashMoves = lookup.crossAnd2Corners[cube.hashCrossAnd2Corners()];

        std::reverse(hashMoves.begin(), hashMoves.end());

        for (auto m : hashMoves) {

            Move mov = Lookup::charToMove(m);
            outMoves.push_back({mov.face, 4 - mov.rotations});
        }

        std::reverse(hashMoves.begin(), hashMoves.end());
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
            Move mov = Lookup::charToMove(m);
            outMoves.push_back({mov.face, 4 - mov.rotations});
        }

        std::reverse(hashMoves.begin(), hashMoves.end());
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
            Move mov = Lookup::charToMove(m);
            outMoves.push_back({mov.face, 4 - mov.rotations});
        }

        std::reverse(hashMoves.begin(), hashMoves.end());
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

    int num = 4;

    lookup.makeCrossAnd2Corners(num);
    printMapSize(lookup.crossAnd2Corners);

    lookup.makeCrossAnd3Corners(num);
    printMapSize(lookup.crossAnd3Corners);

    lookup.makeFirstTwoLayers(num);
    printMapSize(lookup.firstTwoLayers);

    return 69;

    int numSolves = 1000;
    auto startGlob = std::chrono::high_resolution_clock::now();

    long long sumTime = 0;
    double maxTime = 0;

    for (int i = 0; i < numSolves; i++) {
        auto start = std::chrono::high_resolution_clock::now();

        RubiksCube cube;

        auto shuffleMoves = cube.shuffle(30, false, i);

        auto start2Corners = std::chrono::high_resolution_clock::now();
        auto movesCross2Corners = solveCrossAnd2Corners(cube, lookup);
        auto end2Corners = std::chrono::high_resolution_clock::now();

        for (auto m : movesCross2Corners) {
            cube.turn(m.face, m.rotations);
        }

        int num2 = cube.numCornerSolved();

        if (num2 < 2) {
            std::cout << "Nuh uh!" << "\n";
            return 69;
        }

//        auto start3Corners = std::chrono::high_resolution_clock::now();
//        auto movesCross3Corners = solveCrossAnd3Corners(cube, lookup);
//        auto end3Corners = std::chrono::high_resolution_clock::now();
//
//
//        for (auto m : movesCross3Corners) {
//            cube.turn(m.face, m.rotations);
//        }
//
//        int num3 = cube.numCornerSolved();
//
//        if (num3 < 3) {
//            std::cout << "Nuh uh!" << "\n";
//            return 69;
//        }

        auto start4Corners = std::chrono::high_resolution_clock::now();
        auto movesFullLayer = solveFirstTwoLayers(cube, lookup);
        auto end4Corners = std::chrono::high_resolution_clock::now();

        for (auto m : movesFullLayer) {
            cube.turn(m.face, m.rotations);
        }

        int num4 = cube.numCornerSolved();
        if (num4 != 4) {
            std::cout << "Bruh.." << "\r";
            return 2;
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        sumTime += duration;

        auto currentTime = static_cast<double>(duration) / 1000.0;

        if (currentTime > maxTime) {
            maxTime = currentTime;
        }

        // Time substeps!

        std::cout << "Solved " << i + 1 << "/" << numSolves << " cubes.";
        std::cout << " This one in " << currentTime << " ms.";
        std::cout << " Average time: " << static_cast<double>(sumTime) / (static_cast<double>(i + 1) * 1000.0) << " ms.";
        std::cout <<  " Max time: " << maxTime << " ms." << "\n";

    }

    auto endGlob = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endGlob - startGlob).count();
    std::cout << "Found solutions in " << static_cast<double>(duration) / 1000.0 << " milliseconds." << std::endl;
    std::cout << "Average time: " << static_cast<double>(duration) / 1000.0 / static_cast<double>(numSolves) << " milliseconds." << std::endl;
}

