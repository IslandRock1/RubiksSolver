//
// Created by Øystein Bringsli.
//

#ifndef RUBIKSSOLVER_RUBIKSCUBE_HPP
#define RUBIKSSOLVER_RUBIKSCUBE_HPP

#include <array>
#include <string>

struct Move{
    int face;
    int rotations;

    bool operator==(const Move &other) const {
        return (face == other.face) && (rotations == other.rotations);
    }
};

class RubiksCube {
public:
    std::array<short, 48> cube = {0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,5,5,5,5,5,5,5,5,4,4,4,4,4,4,4,4,3,3,3,3,3,3,3,3};
    void turn(int face, int rotations);
    std::array<Move, 18> everyMove = {{{0, 1}, {0, 2}, {0, 3}, {1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}, {4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}}};

    void print();
    static std::array<int, 3> oppositeFace;
    static std::array<int, 6> oppositeFaceAll;
    static std::array<std::vector<int>, 48> physicalPieces;

    bool solvedWhiteCross();
    int numCornerSolved();
    void shuffle(int numMoves);

    std::array<unsigned int, 4> hashFirstTwoLayers();
    std::array<unsigned int, 4> hashCrossAnd3Corners();


    static long convertBase5ToBin(int a, int b, int c);

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
