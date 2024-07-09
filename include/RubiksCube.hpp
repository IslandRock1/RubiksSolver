//
// Created by Øystein Bringsli.
//

#ifndef RUBIKSSOLVER_RUBIKSCUBE_HPP
#define RUBIKSSOLVER_RUBIKSCUBE_HPP

#include <array>
#include <string>

#include "Move.hpp"

enum Hash {
    TwoCorners,
    ThreeCorners,
    FirstTwoLayers,
    WholeCube
};

namespace RubiksConst {
    std::array<Move, 18> extern everyMove;

    constexpr std::array<int, 3> oppositeFace = {5, 4, 3};
    constexpr std::array<int, 6> oppositeFaceAll = {5, 4, 3, 2, 1, 0};

    const std::array<std::vector<int>, 48> physicalPieces = {{
        {10, 45},        // 0
        {46},        // 1
        {32, 47},// 2
        {12},        // 3
        {35},        // 4
        {15, 16},// 5
        {17},        // 6
        {18, 37},// 7
        {29, 40},// 8
        {43},        // 9
        {0, 45}, // 10
        {27},        // 11
        {3},         // 12
        {21, 24},// 13
        {19},        // 14
        {5, 16}, // 15
        {5, 15}, // 16
        {6},         // 17
        {7, 37}, // 18
        {14},        // 19
        {38},        // 20
        {13, 24},// 21
        {25},        // 22
        {26, 39},// 23
        {13, 21},// 24
        {22},        // 25
        {23, 39},// 26
        {11},        // 27
        {36},        // 28
        {8, 40}, // 29
        {41},        // 30
        {34, 42},// 31
        {2, 47}, // 32
        {44},        // 33
        {31, 42},// 34
        {4},         // 35
        {28},        // 36
        {7, 18}, // 37
        {20},        // 38
        {23, 26},// 39
        {8, 29}, // 40
        {30},        // 41
        {31, 34},// 42
        {9},         // 43
        {33},        // 44
        {0, 10}, // 45
        {1},         // 46
        {2, 32}  // 47
    }};
}

class RubiksCube {
public:

    std::array<short, 48> cube = {0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,5,5,5,5,5,5,5,5,4,4,4,4,4,4,4,4,3,3,3,3,3,3,3,3};
    void turn(int face, int rotations);
    void turn(char m);
    void turn(Move m);

    void print();

    bool solvedWhiteCross();
    int numCornerSolved();
    bool solved() const;
    std::vector<Move> shuffle(int numMoves, bool print = false, unsigned int seed = 0);

    std::array<unsigned int, 4> hashFirstTwoLayers();
    std::array<unsigned int, 4> hashCrossAnd2Corners();
    std::array<unsigned int, 4> hashCrossAnd3Corners();
    std::array<unsigned int, 4> hashFullCube();
    std::array<unsigned int, 4> getFromHash(Hash hash);

    inline static unsigned short convertBase5ToBin(int a, int b, int c);

    void raiseCross();
    void raiseTwoCorners();
    void raiseTwoLayer();
    void raiseSolved();

private:
    bool solvedRBCorner();
    bool solvedGRCorner();
    bool solvedOGCorner();
    bool solvedBOCorner();

    void turnWhite(int rotations);
    void turnWhite1();
    void turnWhite2();
    void turnWhite3();
    void turnBlue(int rotations);
    void turnBlue1();
    void turnBlue2();
    void turnBlue3();
    void turnRed(int rotations);
    void turnRed1();
    void turnRed2();
    void turnRed3();
    void turnGreen(int rotations);
    void turnGreen1();
    void turnGreen2();
    void turnGreen3();
    void turnOrange(int rotations);
    void turnOrange1();
    void turnOrange2();
    void turnOrange3();
    void turnYellow(int rotations);
    void turnYellow1();
    void turnYellow2();
    void turnYellow3();
};


#endif //RUBIKSSOLVER_RUBIKSCUBE_HPP
