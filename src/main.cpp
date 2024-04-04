#include <iostream>
#include <chrono>
#include <bitset>
#include <map>

#include "RubiksCube.hpp"
#include "Lookup.hpp"

int fewestMoves = 100;
std::chrono::time_point<std::chrono::system_clock, std::chrono::duration<long long, std::ratio<1, 1000000000>>> globalStartTime;

std::vector<Move> execEveryMove(
        RubiksCube &cube,
        int depth,
        std::vector<Move> &moves,
        Lookup &lookup)
{

    // std::cout << "Searching solution in lookup." << "\n";
    auto lookupSolution = lookup.firstTwoLayers.find(cube.hashCrossAnd2Corners());

    if (lookupSolution != lookup.firstTwoLayers.end()) {
        // std::cout << "Init early exit." << "\n";
        bool earlyExit = true;
        if (earlyExit) {
            std::vector<Move> outMoves;

            for (auto m : moves) {
                outMoves.emplace_back(m);
            }

            auto hashMoves = lookup.firstTwoLayers[cube.hashCrossAnd2Corners()];
            std::reverse(hashMoves.begin(), hashMoves.end());

            for (auto m : hashMoves) {
                outMoves.push_back({m.face, 4 - m.rotations});
            }

            return outMoves;
        }



        auto end = std::chrono::high_resolution_clock::now();
        auto dur = std::chrono::duration_cast<std::chrono::microseconds>(end - globalStartTime).count() / 1000;

        auto numMoves = moves.size() + lookup.firstTwoLayers[cube.hashCrossAnd3Corners()].size();
        if (numMoves < fewestMoves) {
            fewestMoves = numMoves;
            std::cout << "Found solution for cross and three corners in " << dur << " milliseconds and " <<
                      numMoves << " moves. Moves.size() = " << moves.size() << " | Lookup moves.size = " << lookup.firstTwoLayers[cube.hashCrossAnd3Corners()].size() << "\n";

            std::cout << "Moves: ";
            for (auto m : moves) {
                std::cout << "f" << m.face << "n" << m.rotations << " | ";
            }
            std::cout << "\n" << "And: ";
            auto hashMoves = lookup.firstTwoLayers[cube.hashCrossAnd3Corners()];
            std::reverse(hashMoves.begin(), hashMoves.end());

            for (auto m : hashMoves) {
                std::cout << "f" << m.face << "n" << 4 - m.rotations << " | ";
            }

            std::cout << "\n";

        }
    }

    if (depth == 0) {
        // std::cout << "Depth limit reached." << "\n";
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

        // std::cout << "Starting prune." << "\n";
        if (Lookup::prune(m, prevMove, doublePrevMove)) { continue;}

        moves.push_back(m);
        cube.turn(m.face, m.rotations);

        auto fromRecursed = execEveryMove(cube, depth - 1, moves, lookup);
        if (!fromRecursed.empty()) {
            return fromRecursed;
        }

        cube.turn(m.face, 4 - m.rotations);
        moves.pop_back();
    }

    return {};
}

void printMoves(std::vector<Move> &moves) {
    std::cout << "Moves: ";

    for (auto m : moves) {
        switch (m.face) {
            case 0:
            {
                std::cout << "T";
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

void timeSomething() {
    RubiksCube c;
    auto start = std::chrono::high_resolution_clock::now();
    c.hashCrossAnd3Corners();
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Time taken by function: " << duration.count() << " microseconds" << std::endl;
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
    double totalSizeKB = totalSize / 1024.0;
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "Total size of map: " << std::fixed << std::setprecision(2) << totalSizeKB << " KB. Found in " << duration.count() / 1000 << " milliseconds." << std::endl;
}

void testSolver() {
    Lookup lookup(5);
    printMapSize(lookup.firstTwoLayers);

    RubiksCube myCube;
    auto shuffleMoves = myCube.shuffle(30);

    std::cout << "Shuffling moves..." << "\n";
    printMoves(shuffleMoves);

    std::vector<Move> moves;

    for (unsigned short depth = 4; depth < 10; depth++) {
        auto solvingMoves = execEveryMove(myCube, depth, moves, lookup);
        if (!solvingMoves.empty()) {
            std::cout << "Solving moves..." << "\n";
            printMoves(solvingMoves);
            break;
        }
    }
}

int main() {
    testSolver();
    return 69;

    Lookup lookup(6);
    printMapSize(lookup.firstTwoLayers);

    unsigned long solvedCubes = 0;
    unsigned long long usedMoves = 0;

    auto start = std::chrono::high_resolution_clock::now();
    int numShuffle = 50;

    for (unsigned long iter = 1; iter < 10000; iter++) {

        RubiksCube myCube;
        myCube.shuffle(numShuffle);

        std::vector<Move> moves;

        for (unsigned short depth = 4; depth < 10; depth++) {
            auto solvingMoves = execEveryMove(myCube, depth, moves, lookup);
            if (!solvingMoves.empty()) {
                solvedCubes++;
                usedMoves += solvingMoves.size();
                break;
            }
        }

        if ((iter % 50) == 0) {
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
            auto avgTime = duration.count() / static_cast<long long>(solvedCubes) / 1000;

            double avgMoves = static_cast<double>(usedMoves) / static_cast<double>(solvedCubes);
            std::cout << "Solved " << solvedCubes << " cubes with " << usedMoves << " moves. Resulting in average movecount of " << avgMoves << " with avg solvetime of " << avgTime << " milliseconds." << "\r";

            if ((iter % 200) == 0) {
                numShuffle++;
                solvedCubes = 0;
                usedMoves = 0;

                std::cout << "\nNow solving shuffle lenght of " << numShuffle << " moves." << "\n";

                start = std::chrono::high_resolution_clock::now();
            }
        }
    }

//    auto end = std::chrono::high_resolution_clock::now();
//    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
//    std::cout << "Time taken by function: " << duration.count() / 1000 << " milliseconds" << std::endl;

    return 69;
}

