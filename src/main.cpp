#include <iostream>
#include <chrono>

#include "RubiksCube.hpp"

long long int globalStates = 0;
int maxLevel = 0;
std::chrono::time_point<std::chrono::system_clock, std::chrono::duration<long long, std::ratio<1, 1000000000>>> globalStartTime;

bool prune(Move &currentMove, Move &prevMove, Move &doublePrevMove) {
    if (currentMove.face == prevMove.face) { return true;}

    if ((currentMove.face == doublePrevMove.face) && (RubiksCube::oppositeFace[currentMove.face] == prevMove.face)) {
        return true;
    }

    if (currentMove.face < 3) {
        if (prevMove.face == RubiksCube::oppositeFace[currentMove.face]) {
            return true;
        }
    }

    return false;
}

void execEveryMove(RubiksCube &cube, int depth, Move &prevMove, Move &doublePrevMove)
{
    int level = cube.solvedWhiteCross();

    if (level) {
        level += cube.numCornerSolved();

        if (level > maxLevel) {
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - globalStartTime);
            std::cout << "Reached level: " << level << " after " << duration.count() / 1000 << " milliseconds" << "\n";
            maxLevel = level;
        }
    }

    if (depth == 0) {
        globalStates++;
        return;
    }

    for (Move m : cube.everyMove)
    {
        if (prune(m, prevMove, doublePrevMove)) { continue;}

        cube.turn(m.face, m.rotations);
        execEveryMove(cube, depth - 1, m, prevMove);
        cube.turn(m.face, 4 - m.rotations);
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

int main() {
    RubiksCube myCube;
    myCube.shuffle(100);

    Move m0 = {7, 0}; //Invalid to ensure no collision
    Move m1 = {7, 0}; //Invalid to ensure no collision

    auto start = std::chrono::high_resolution_clock::now();
    globalStartTime = start;

    execEveryMove(myCube, 7, m0, m1);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Time taken by function: " << duration.count() / 1000 << " milliseconds" << std::endl;
    std::cout << "States visited: " << globalStates / 1000 / 1000 << " million." << std::endl;

    return 0;
}

