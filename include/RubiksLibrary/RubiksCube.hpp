
#ifndef RUBIKSSOLVER_RUBIKSCUBE_HPP
#define RUBIKSSOLVER_RUBIKSCUBE_HPP

#include <array>
#include <string>
#include <unordered_map>

#include "RubiksLibrary/Move.hpp"

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
    const std::array<short, 48> solvedCube = {0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,5,5,5,5,5,5,5,5,4,4,4,4,4,4,4,4,3,3,3,3,3,3,3,3};

    const std::array<std::vector<int>, 48> physicalPieces = {{
        {10, 45},        // 0
        {46},        // 1
        {47, 32},// 2
        {12},        // 3
        {35},        // 4
        {16, 15},// 5
        {17},        // 6
        {37, 18},// 7
        {29, 40},// 8
        {43},        // 9
        {45, 0}, // 10
        {27},        // 11
        {3},         // 12
        {21, 24},// 13
        {19},        // 14
        {5, 16}, // 15
        {15, 5}, // 16
        {6},         // 17
        {7, 37}, // 18
        {14},        // 19
        {38},        // 20
        {24, 13},// 21
        {25},        // 22
        {39, 26},// 23
        {13, 21},// 24
        {22},        // 25
        {23, 39},// 26
        {11},        // 27
        {36},        // 28
        {40, 8}, // 29
        {41},        // 30
        {34, 42},// 31
        {2, 47}, // 32
        {44},        // 33
        {42, 31},// 34
        {4},         // 35
        {28},        // 36
        {18, 7}, // 37
        {20},        // 38
        {26, 23},// 39
        {8, 29}, // 40
        {30},        // 41
        {31, 34},// 42
        {9},         // 43
        {33},        // 44
        {0, 10}, // 45
        {1},         // 46
        {32, 2}  // 47
    }};

    const std::array<std::vector<int>, 48> colors = {
        {
            // Begin Corners
            {5, 4, 3},
            {5, 3, 1},
            {5, 2, 4},
            {5, 1, 2},
            {0, 4, 2},
            {0, 2, 1},
            {0, 3, 4},
            {0, 1, 3},
            // End Corners
            // Start Edges
            {5, 3},
            {5, 4},
            {5, 1},
            {5, 2},

            {4, 3},
            {2, 4},
            {1, 2},
            {1, 3},

            {0, 2},
            {0, 4},
            {0, 1},
            {0, 3}
        }
    };

    const std::array<int, 30> colorComboLookupEdgesArray = {0, 108, 96, 114, 102, 0, 0, 0, 84, 90, 0, 60, 0, 0, 0, 0, 78, 66, 0, 0, 0, 0, 72, 48, 0, 0, 0, 0, 0, 54};
    const std::unordered_map<int, int> colorComboLookupEdges {
            {23,  48}, // YELLOW, green
            {29,  54}, // YELLOW, orange
            {11,  60}, // YELLOW, red
            {17,  66}, // YELLOW, blue

            {22,  72}, // ORANGE, green
            {16,  78}, // BLUE, orange
            { 8,  84}, // RED, blue
            { 9,  90}, // RED, green

            { 2,  96},
            { 4, 102},
            { 1, 108},
            { 3, 114}
    };

    const std::array<int, 150> colorComboLookupCornersArray = {
        0, 0, 0, 0, 0, 0, 0, 0, 30, 42,      //  0-  9
        0, 0, 0, 30, 0, 0, 24, 0, 0, 42,     //  10- 19
        0, 0, 36, 0, 0, 0, 24, 36, 0, 0,     //  20- 29
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,        //  30- 39
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,        //  40- 49
        0, 0, 0, 18, 0, 0, 0, 0, 0, 6,        //  50- 59
        0, 0, 0, 0, 0, 0, 0, 0, 18, 6,        //  60- 69
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,        //  70- 79
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,        //  80- 89
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,        //  90- 99
        0, 12, 0, 0, 0, 0, 12, 0, 0, 0,        // 100-109
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,        // 110-119
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,        // 120-129
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,        // 130-139
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,        // 140-149
    };
    const std::unordered_map<int, int> colorComboLookupCorners {
            {137,  0}, // YELLOW, orange, green => green * 36 + orange * 6 + yellow (smallest value first) | BitPosition 0
            { 59,  6}, // yellow, green, RED | BitPosition 6, aka 0bHEREXXXXXX (x is irrelevant)
            {101, 12}, // yellow, BLUE, orange
            { 53, 18}, // yellow, RED, blue

            { 16, 24}, // WHITE, orange, blue
            {  8, 30}, // WHITE, blue, red
            { 22, 36}, // WHITE, green, orange
            {  9, 42}, // WHITE, red, green

                // By adding in col0 * 36 + col2 * 6 + col1, i can drop the max and min functions in the hash
            { 26, 24},
            { 69,  6},
            { 13, 30},
            {106, 12},
            { 19, 42},
            { 68, 18},
            { 27, 36},
            {142,  0}
        };
}

class RubiksCube {
public:
    //                                               10                  20                  30                  40
    //                            0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7
    std::array<short, 48> cube = {0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,5,5,5,5,5,5,5,5,4,4,4,4,4,4,4,4,3,3,3,3,3,3,3,3};
    void turn(int face, int rotations);
    void turn(char m);
    void turn(Move m);

    static std::array<short, 48> getCubeFromHash(__int128 hash);
    __int128 hashNewV0() const;
    __int128 hashNewV1() const;
    __int128 hashNewV2() const;
    __int128 hashNewV3() const;
    __int128 hashNewV4() const;

    void print();
    static void print(const std::array<short, 48>& cube);

    bool solvedWhiteCross();
    int numCornerSolved();
    bool solved() const;
    std::vector<Move> shuffle(int numMoves, bool print = false, unsigned int seed = 0);

    std::array<unsigned int, 4> hashFirstTwoLayers();
    std::array<unsigned int, 4> hashCrossAnd2Corners();
    std::array<unsigned int, 4> hashCrossAnd2CornersV1() const;
    std::array<unsigned int, 4> hashCrossAnd2CornersV2();
    std::array<unsigned int, 4> hashCrossAnd3Corners();
    std::array<unsigned int, 4> hashFullCube();
    std::array<unsigned int, 4> getFromHash(Hash hash);

    inline static unsigned short convertBase5ToBin(int a, int b, int c);

    void raiseCross();
    void raiseTwoCorners();
    void raiseThreeCorners();
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
