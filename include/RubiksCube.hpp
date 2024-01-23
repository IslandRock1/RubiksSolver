//
// Created by Øystein Bringsli.
//

#ifndef RUBIKSSOLVER_RUBIKSCUBE_HPP
#define RUBIKSSOLVER_RUBIKSCUBE_HPP

#include <cstdint>
#include <array>
#include <string>
#include <unordered_map>

struct Move{
    int face;
    int rotations;

    bool operator==(const Move &other) const {
        return (face == other.face) && (rotations == other.rotations);
    }
};


class RubiksCube {
public:
    std::array<std::uint_fast8_t, 54> cube = {0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5};
    void turn(int face, int rotations);
    std::array<Move, 18> everyMove = {{{0, 1}, {0, 2}, {0, 3}, {1, 1}, {1, 2}, {1, 3}, {2, 1}, {2, 2}, {2, 3}, {3, 1}, {3, 2}, {3, 3}, {4, 1}, {4, 2}, {4, 3}, {5, 1}, {5, 2}, {5, 3}}};
    std::array<int, 6> oppositeFace = {5, 3, 4, 1, 2, 0};

    bool handleMap(int depth);
    std::string hash();
    std::unordered_map<std::string, int> cubeDict;

    int cacheHits = 0;
    std::array<int, 6> prevSymmetryLeftRight = {0, 1, 2, 1, 2, 0};


private:
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
