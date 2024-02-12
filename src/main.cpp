#include <iostream>
#include <chrono>
#include <bitset>

#include "RubiksCube.hpp"
#include "Lookup.hpp"

long long int globalStates = 0;
int maxLevel = 0;
std::chrono::time_point<std::chrono::system_clock, std::chrono::duration<long long, std::ratio<1, 1000000000>>> globalStartTime;

//bool prune(Move &currentMove, Move &prevMove, Move &doublePrevMove) {
//    if (currentMove.face == prevMove.face) { return true;}
//
//    if ((currentMove.face == doublePrevMove.face) && (RubiksCube::oppositeFace[currentMove.face] == prevMove.face)) {
//        return true;
//    }
//
//    if (currentMove.face < 3) {
//        if (prevMove.face == RubiksCube::oppositeFace[currentMove.face]) {
//            return true;
//        }
//    }
//
//    return false;
//}

void execEveryMove(
        RubiksCube &cube,
        int depth,
        std::vector<Move> &moves,
        Lookup &lookup)
{

    auto lookupSolution = lookup.firstTwoLayers.find(cube.hashFirstTwoLayers());

    if (lookupSolution != lookup.firstTwoLayers.end()) {
        auto end = std::chrono::high_resolution_clock::now();
        auto dur = std::chrono::duration_cast<std::chrono::microseconds>(end - globalStartTime).count() / 1000;
        std::cout << "Found solution for first two layers in " << dur << " milliseconds and " <<
            moves.size() + lookup.firstTwoLayers[cube.hashFirstTwoLayers()].size() << " moves." << "\n";
    }

//    int level = cube.solvedWhiteCross();
//
//    if (level) {
//        level += cube.numCornerSolved();
//
//        if (level > maxLevel) {
//            auto end = std::chrono::high_resolution_clock::now();
//            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - globalStartTime);
//            std::cout << "Reached level: " << level << " after " << duration.count() / 1000 << " milliseconds" << "\n";
//            maxLevel = level;
//        }
//    }

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
    RubiksCube c;
    auto hash = c.hashCrossAnd2Corners();
    printHash(hash, false);

    // c.turn(4, 1);
    c.turn(5, 2);
    // c.turn(4, 3);

    hash = c.hashCrossAnd2Corners();
    printHash(hash, false);
    return 69;

//    auto st = std::chrono::high_resolution_clock::now();
//    Lookup l(6);
//    auto en = std::chrono::high_resolution_clock::now();
//    auto du = std::chrono::duration_cast<std::chrono::microseconds>(en - st);
//    std::cout << "Time taken by function: " << du.count() / 1000 << " milliseconds" << std::endl;
//
//    return 69;

    RubiksCube myCube;
    myCube.shuffle(100);

    std::vector<Move> moves;

    auto start = std::chrono::high_resolution_clock::now();
    globalStartTime = start;

    Lookup lookup(7);

    auto endLookup = std::chrono::high_resolution_clock::now();
    auto durLookup = std::chrono::duration_cast<std::chrono::microseconds>(endLookup - start);
    std::cout << "Time to generate lookup: " << durLookup.count() / 1000 << " milliseconds" << std::endl;

    execEveryMove(myCube, 7, moves, lookup);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Time taken by function: " << duration.count() / 1000 << " milliseconds" << std::endl;
    std::cout << "States visited: " << globalStates / 1000 / 1000 << " million." << std::endl;

    return 0;
}

