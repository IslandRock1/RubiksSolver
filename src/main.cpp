#include <iostream>
#include <chrono>
#include <bitset>

#include "RubiksCube.hpp"
#include "Lookup.hpp"

long long int globalStates = 0;
int fewestMoves = 10;
std::chrono::time_point<std::chrono::system_clock, std::chrono::duration<long long, std::ratio<1, 1000000000>>> globalStartTime;

void execEveryMove(
        RubiksCube &cube,
        int depth,
        std::vector<Move> &moves,
        Lookup &lookup)
{

    auto lookupSolution = lookup.firstTwoLayers.find(cube.hashCrossAnd3Corners());

    if (lookupSolution != lookup.firstTwoLayers.end()) {
        auto end = std::chrono::high_resolution_clock::now();
        auto dur = std::chrono::duration_cast<std::chrono::microseconds>(end - globalStartTime).count() / 1000;

        auto numMoves = moves.size() + lookup.firstTwoLayers[cube.hashCrossAnd3Corners()].size();
        if (numMoves < fewestMoves) {
            fewestMoves = numMoves;
            std::cout << "Found solution for cross and three corners in " << dur << " milliseconds and " <<
                      numMoves << " moves. Moves.size() = " << moves.size() << " | Lookup moves.size = " << lookup.firstTwoLayers[cube.hashFirstTwoLayers()].size() << "\n";

            std::cout << "Moves: ";
            for (auto m : moves) {
                std::cout << "f" << m.face << "n" << m.rotations << " | ";
            }
            std::cout << "\n" << "And: ";
            for (auto m : lookup.firstTwoLayers[cube.hashCrossAnd3Corners()]) {
                std::cout << "f" << m.face << "n" << m.rotations << " | ";
            }

        }
    }

    if (depth == 0) {
        globalStates++;
        return;
    }

    for (Move m : cube.everyMove)
    {
        auto size = moves.size();
        Move prevMove {7, 7};
        Move doublePrevMove {7, 7};

        if (size > 1) {
            prevMove = moves[size - 1];
            doublePrevMove = moves[size - 2];
        } else if (size > 0) {
            prevMove = moves[size - 1];
        }

        if (Lookup::prune(m, prevMove, doublePrevMove)) { continue;}

        moves.push_back(m);
        cube.turn(m.face, m.rotations);
        execEveryMove(cube, depth - 1, moves, lookup);
        cube.turn(m.face, 4 - m.rotations);
        moves.pop_back();
    }
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

int main() {
    RubiksCube myCube;
    myCube.shuffle(30);

    std::vector<Move> moves;

    auto start = std::chrono::high_resolution_clock::now();
    globalStartTime = start;

    Lookup lookup(6);

    auto endLookup = std::chrono::high_resolution_clock::now();
    auto durLookup = std::chrono::duration_cast<std::chrono::microseconds>(endLookup - start);
    std::cout << "Time to generate lookup: " << durLookup.count() / 1000 << " milliseconds" << std::endl;

    execEveryMove(myCube, 3, moves, lookup);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Time taken by function: " << duration.count() / 1000 << " milliseconds" << std::endl;
    std::cout << "States visited: " << globalStates / 1000 / 1000 << " million." << std::endl;

    return 0;
}

