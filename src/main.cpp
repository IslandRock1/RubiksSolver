#include <iostream>
#include <chrono>
#include <random>
#include <array>

#include "RubiksCube.hpp"

int getRandomIntInRange(int a, int b) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(a, b);

    return distribution(gen);
}

void execEveryMove(RubiksCube &cube, int depth, Move &prevMove, Move &doublePrevMove)
{
    if (depth == 0) {return;}

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

int main() {
    RubiksCube myCube;

    Move m0 = {0, 0};
    Move m1 = {0, 0};

    auto start = std::chrono::high_resolution_clock::now();

    execEveryMove(myCube, 8, m0, m1);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Time taken by function: " << duration.count() / 1000 / 1000 << " seconds" << std::endl;

    return 0;
}
