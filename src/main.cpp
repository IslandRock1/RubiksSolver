#include <iostream>
#include <chrono>

#include "RubiksCube.hpp"

int globalStates = 0;
int maxLevel = 0;
std::chrono::time_point<std::chrono::system_clock, std::chrono::duration<long long, std::ratio<1, 1000000000>>> globalStartTime;

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
        if (m == prevMove) { continue;}

        if ((m.face == doublePrevMove.face) && (cube.oppositeFace[m.face] == prevMove.face)) {
            continue;
        }

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
    testMoveSpeed();
    return 69;

    RubiksCube myCube;
    myCube.shuffle(100);

    Move m0 = {7, 0};
    Move m1 = {7, 0};

    auto start = std::chrono::high_resolution_clock::now();
    globalStartTime = start;

    execEveryMove(myCube, 7, m0, m1);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Time taken by function: " << duration.count() / 1000 << " milliseconds" << std::endl;
    std::cout << "States visited: " << globalStates / 1000 / 1000 << " million." << std::endl;

    return 0;
}

