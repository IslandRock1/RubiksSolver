//
// Created by Øystein Bringsli.
//

#include <iostream>
#include <random>
#include <bitset>

#include "RubiksCube.hpp"

unsigned short RubiksCube::convertBase5ToBin(int a, int b, int c) {
    return a * 36 + b * 6 + c;
}

std::array<unsigned int, 4> RubiksCube::hashCrossAnd2Corners() {
    std::array<unsigned int, 4> vals = {0, 0, 0, 0};

    for (int i = 0; i < 4; i++) {
        unsigned int val = 0;
        for (int k = 0; k < 4; k++) {

            auto ix0 = 12 * i + 3 * k + 0;
            auto ix1 = ix0 + 1;
            auto ix2 = ix0 + 2;

            auto face0 = cube[ix0];
            auto face1 = cube[ix1];
            auto face2 = cube[ix2];

            std::array<int, 3> indices = {ix0, ix1, ix2};
            std::array<short, 3> faces = {face0, face1, face2};
            std::array<int, 3> output = {0, 0, 0};

            for (int newIx = 0; newIx < 3; newIx++) {
                auto piece0 = RubiksConst::physicalPieces[indices[newIx]];
                auto currentFace = faces[newIx];
                output[newIx] = faces[newIx];

                if (piece0.size() > 1) {

                    if ((cube[piece0[0]] == 5) || (cube[piece0[1]] == 5) || (currentFace == 5)) {
                        output[newIx] = 5;
                    } else {
                        auto white = (currentFace == 0) + (cube[piece0[0]] == 0) + (cube[piece0[1]] == 0);
                        auto orange = (currentFace == 4) + (cube[piece0[0]] == 4) + (cube[piece0[1]] == 4);
                        auto green = (currentFace == 3) + (cube[piece0[0]] == 3) + (cube[piece0[1]] == 3);
                        auto blue = (currentFace == 2) + (cube[piece0[0]] == 2) + (cube[piece0[1]] == 2);

                        if ((white + orange + green) == 3) {
                            output[newIx] = 5;
                        } else if ((white + orange + blue) == 3) {
                            output[newIx] = 5;
                        }
                    }

                } else {

                    if ((cube[piece0[0]] == 5) || (currentFace == 5)) {
                        output[newIx] = 5;
                    } else {
                        auto orange = (currentFace == 4) + (cube[piece0[0]] == 4);
                        auto green = (currentFace == 3) + (cube[piece0[0]] == 3);
                        auto blue = (currentFace == 2) + (cube[piece0[0]] == 2);

                        if (orange && green) {
                            output[newIx] = 5;
                        } else if (orange && blue) {
                            output[newIx] = 5;
                        }
                    }
                }
            }

            auto convert = convertBase5ToBin(output[0], output[1], output[2]);
            val = val << 8;
            val |= convert;

        }
        vals[i] = val;
    }

    return vals;
}

std::array<unsigned int, 4> RubiksCube::hashCrossAnd3Corners() {
    std::array<unsigned int, 4> vals = {0, 0, 0, 0};

    for (int i = 0; i < 4; i++) {
        unsigned int val = 0;
        for (int k = 0; k < 4; k++) {

            auto ix0 = 12 * i + 3 * k + 0;
            auto ix1 = ix0 + 1;
            auto ix2 = ix0 + 2;

            auto face0 = cube[ix0];
            auto face1 = cube[ix1];
            auto face2 = cube[ix2];

            std::array<int, 3> indices = {ix0, ix1, ix2};
            std::array<short, 3> faces = {face0, face1, face2};
            std::array<int, 3> output = {0, 0, 0};

            for (int newIx = 0; newIx < 3; newIx++) {
                auto piece0 = RubiksConst::physicalPieces[indices[newIx]];
                auto currentFace = faces[newIx];
                output[newIx] = faces[newIx];

                if (piece0.size() > 1) {

                    if ((cube[piece0[0]] == 5) || (cube[piece0[1]] == 5) || (currentFace == 5)) {
                        output[newIx] = 5;
                    } else {
                        auto white = (currentFace == 0) + (cube[piece0[0]] == 0) + (cube[piece0[1]] == 0);
                        auto orange = (currentFace == 4) + (cube[piece0[0]] == 4) + (cube[piece0[1]] == 4);
                        auto green = (currentFace == 3) + (cube[piece0[0]] == 3) + (cube[piece0[1]] == 3);

                        if ((white + orange + green) == 3) {
                            output[newIx] = 5;
                        }
                    }

                } else {

                    if ((cube[piece0[0]] == 5) || (currentFace == 5)) {
                        output[newIx] = 5;
                    } else {
                        auto orange = (currentFace == 4) + (cube[piece0[0]] == 4);
                        auto green = (currentFace == 3) + (cube[piece0[0]] == 3);

                        if (orange && green) {
                            output[newIx] = 5;
                        }
                    }
                }
            }

            auto convert = convertBase5ToBin(output[0], output[1], output[2]);
            val = val << 8;
            val |= convert;

        }
        vals[i] = val;
    }

    return vals;
}

std::array<unsigned int, 4> RubiksCube::hashFirstTwoLayers() {
    std::array<unsigned int, 4> vals = {0, 0, 0, 0};

    for (int i = 0; i < 4; i++) {
        unsigned int val = 0;
        for (int k = 0; k < 4; k++) {

            auto ix0 = 12 * i + 3 * k + 0;
            auto ix1 = ix0 + 1;
            auto ix2 = ix0 + 2;

            auto face0 = cube[ix0];
            auto face1 = cube[ix1];
            auto face2 = cube[ix2];

            auto piece0 = RubiksConst::physicalPieces[ix0];
            short output0 = face0;
            for (auto tmp : piece0) {
                if (cube[tmp] == 5) {
                    output0 = 5;
                }
            }

            auto piece1 = RubiksConst::physicalPieces[ix1];
            short output1 = face1;
            for (auto tmp : piece1) {
                if (cube[tmp] == 5) {
                    output1 = 5;
                }
            }

            auto piece2 = RubiksConst::physicalPieces[ix2];
            short output2 = face2;
            for (auto tmp : piece2) {
                if (cube[tmp] == 5) {
                    output2 = 5;
                }
            }

            auto convert = convertBase5ToBin(output0, output1, output2);
            val = val << 8;
            val |= convert;
        }
        vals[i] = val;
    }

    return vals;
}

std::array<unsigned int, 4> RubiksCube::hashFullCube() {
    std::array<unsigned int, 4> vals = {0, 0, 0, 0};

    for (int i = 0; i < 4; i++) {
        unsigned int val = 0;
        for (int k = 0; k < 4; k++) {

            auto ix0 = 12 * i + 3 * k + 0;
            auto ix1 = ix0 + 1;
            auto ix2 = ix0 + 2;

            auto face0 = cube[ix0];
            auto face1 = cube[ix1];
            auto face2 = cube[ix2];

            auto convert = convertBase5ToBin(face0, face1, face2);
            val = val << 8;
            val |= convert;
        }
        vals[i] = val;
    }

    return vals;
}

void RubiksCube::print() {
    int space = 0;

    for (auto val : cube) {
        std::cout << std::to_string(val) << " ";

        if (space == 7) {
            std::cout << " | ";
            space = -1;
        }

        space++;
    }

    std::cout << "\n";
}

bool RubiksCube::solvedWhiteCross() {
    return (
            cube[1] == 0 &&
            cube[3] == 0 &&
            cube[4] == 0 &&
            cube[6] == 0 &&
            cube[46] == 3 &&
            cube[12] == 1 &&
            cube[35] == 4 &&
            cube[17] == 2);
}

int RubiksCube::numCornerSolved() {
    return solvedBOCorner() + solvedGRCorner() + solvedOGCorner() + solvedRBCorner();
}

bool RubiksCube::solvedRBCorner() {
    return (
            cube[5] == 0 &&
            cube[16] == 2 &&
            cube[15] == 1 &&
            cube[14] == 1 &&
            cube[19] == 2
            );
}

bool RubiksCube::solvedGRCorner() {
    return (
            cube[0] == 0 &&
            cube[45] == 3 &&
            cube[10] == 1 &&
            cube[9] == 1 &&
            cube[43] == 3
    );
}

bool RubiksCube::solvedOGCorner() {
    return (
            cube[2] == 0 &&
            cube[47] == 3 &&
            cube[44] == 3 &&
            cube[32] == 4 &&
            cube[33] == 4
    );
}

bool RubiksCube::solvedBOCorner() {
    return (
            cube[7] == 0 &&
            cube[18] == 2 &&
            cube[37] == 4 &&
            cube[38] == 4 &&
            cube[20] == 2
    );
}

std::vector<Move> RubiksCube::shuffle(int numMoves, bool print, unsigned int seed) {
    // std::random_device dev;

    std::mt19937 rng;
    rng.seed(seed);

    std::uniform_int_distribution<std::mt19937::result_type> dist6(0,5);
    std::uniform_int_distribution<std::mt19937::result_type> distNum(1, 3);

    std::vector<Move> shuffleMoves;
    int prevFace = -1;

    for (int i = 0; i < numMoves; i++)
    {
        auto face = static_cast<int>(dist6(rng));
        auto num = static_cast<int>(distNum(rng));

        if (face == prevFace) {
            numMoves++;
            continue;
        } else {
            prevFace = face;
        }

        turn(face, num);
        shuffleMoves.push_back({face, num});

        if (print) {
            std::cout << "f" << face << "n" << num << " | ";
        }
    }

    if (print) {std::cout << "\n";}
    return shuffleMoves;
}

void RubiksCube::turn(Move m) {
    turn(m.face, m.rotations);
}

void RubiksCube::turn(int face, int rotations) {
    switch (face) {
        case 0: {
            turnWhite(rotations);
        } break;

        case 1: {
            turnRed(rotations);
        } break;

        case 2: {
            turnBlue(rotations);
        } break;

        case 3: {
            turnGreen(rotations);
        } break;

        case 4: {
            turnOrange(rotations);
        } break;

        case 5: {
            turnYellow(rotations);
        } break;
    }
}

void RubiksCube::turnWhite(int rotations) {
    switch (rotations) {
        case 1: {
            RubiksCube::turnWhite1();
        } break;
        case 2: {
            RubiksCube::turnWhite2();
        } break;
        case 3: {
            RubiksCube::turnWhite3();
        } break;
    }
}

void RubiksCube::turnBlue(int rotations) {
    switch (rotations) {
        case 1: {
            RubiksCube::turnBlue1();
        } break;
        case 2: {
            RubiksCube::turnBlue2();
        } break;
        case 3: {
            RubiksCube::turnBlue3();
        } break;
    }
}

void RubiksCube::turnRed(int rotations) {
    switch (rotations) {
        case 1: {
            RubiksCube::turnRed1();
        } break;
        case 2: {
            RubiksCube::turnRed2();
        } break;
        case 3: {
            RubiksCube::turnRed3();
        } break;
    }
}

void RubiksCube::turnGreen(int rotations) {
    switch (rotations) {
        case 1: {
            RubiksCube::turnGreen1();
        } break;
        case 2: {
            RubiksCube::turnGreen2();
        } break;
        case 3: {
            RubiksCube::turnGreen3();
        } break;
    }
}

void RubiksCube::turnOrange(int rotations) {
    switch (rotations) {
        case 1: {
            RubiksCube::turnOrange1();
        } break;
        case 2: {
            RubiksCube::turnOrange2();
        } break;
        case 3: {
            RubiksCube::turnOrange3();
        } break;
    }
}

void RubiksCube::turnYellow(int rotations) {
    switch (rotations) {
        case 1: {
            RubiksCube::turnYellow1();
        } break;
        case 2: {
            RubiksCube::turnYellow2();
        } break;
        case 3: {
            RubiksCube::turnYellow3();
        } break;
    }
}

void RubiksCube::turnWhite3()  {
    unsigned char tmp;
    tmp = cube[47];
    cube[47] = cube[37];
    cube[37] = cube[16];
    cube[16] = cube[10];
    cube[10] = tmp;

    tmp = cube[46];
    cube[46] = cube[35];
    cube[35] = cube[17];
    cube[17] = cube[12];
    cube[12] = tmp;

    tmp = cube[45];
    cube[45] = cube[32];
    cube[32] = cube[18];
    cube[18] = cube[15];
    cube[15] = tmp;

    tmp = cube[7];
    cube[7] = cube[5];
    cube[5] = cube[0];
    cube[0] = cube[2];
    cube[2] = tmp;

    tmp = cube[4];
    cube[4] = cube[6];
    cube[6] = cube[3];
    cube[3] = cube[1];
    cube[1] = tmp;

}

void RubiksCube::turnWhite2()  {
    unsigned char tmp;

    tmp = cube[47];
    cube[47] = cube[16];
    cube[16] = tmp;

    tmp = cube[46];
    cube[46] = cube[17];
    cube[17] = tmp;

    tmp = cube[45];
    cube[45] = cube[18];
    cube[18] = tmp;

    tmp = cube[37];
    cube[37] = cube[10];
    cube[10] = tmp;

    tmp = cube[35];
    cube[35] = cube[12];
    cube[12] = tmp;

    tmp = cube[32];
    cube[32] = cube[15];
    cube[15] = tmp;

    tmp = cube[7];
    cube[7] = cube[0];
    cube[0] = tmp;

    tmp = cube[4];
    cube[4] = cube[3];
    cube[3] = tmp;

    tmp = cube[2];
    cube[2] = cube[5];
    cube[5] = tmp;

    tmp = cube[6];
    cube[6] = cube[1];
    cube[1] = tmp;
}

void RubiksCube::turnWhite1()  {
    unsigned char tmp;

    tmp = cube[47];
    cube[47] = cube[10];
    cube[10] = cube[16];
    cube[16] = cube[37];
    cube[37] = tmp;

    tmp = cube[46];
    cube[46] = cube[12];
    cube[12] = cube[17];
    cube[17] = cube[35];
    cube[35] = tmp;

    tmp = cube[45];
    cube[45] = cube[15];
    cube[15] = cube[18];
    cube[18] = cube[32];
    cube[32] = tmp;

    tmp = cube[7];
    cube[7] = cube[2];
    cube[2] = cube[0];
    cube[0] = cube[5];
    cube[5] = tmp;

    tmp = cube[4];
    cube[4] = cube[1];
    cube[1] = cube[3];
    cube[3] = cube[6];
    cube[6] = tmp;
}

void RubiksCube::turnRed3()  {
    unsigned char tmp;

    tmp = cube[45];
    cube[45] = cube[5];
    cube[5] = cube[21];
    cube[21] = cube[29];
    cube[29] = tmp;

    tmp = cube[43];
    cube[43] = cube[3];
    cube[3] = cube[19];
    cube[19] = cube[27];
    cube[27] = tmp;

    tmp = cube[40];
    cube[40] = cube[0];
    cube[0] = cube[16];
    cube[16] = cube[24];
    cube[24] = tmp;

    tmp = cube[15];
    cube[15] = cube[13];
    cube[13] = cube[8];
    cube[8] = cube[10];
    cube[10] = tmp;

    tmp = cube[12];
    cube[12] = cube[14];
    cube[14] = cube[11];
    cube[11] = cube[9];
    cube[9] = tmp;

}

void RubiksCube::turnRed2()  {
    unsigned char tmp;

    tmp = cube[45];
    cube[45] = cube[21];
    cube[21] = tmp;

    tmp = cube[43];
    cube[43] = cube[19];
    cube[19] = tmp;

    tmp = cube[40];
    cube[40] = cube[16];
    cube[16] = tmp;

    tmp = cube[24];
    cube[24] = cube[0];
    cube[0] = tmp;

    tmp = cube[27];
    cube[27] = cube[3];
    cube[3] = tmp;

    tmp = cube[29];
    cube[29] = cube[5];
    cube[5] = tmp;

    tmp = cube[15];
    cube[15] = cube[8];
    cube[8] = tmp;

    tmp = cube[12];
    cube[12] = cube[11];
    cube[11] = tmp;

    tmp = cube[10];
    cube[10] = cube[13];
    cube[13] = tmp;

    tmp = cube[14];
    cube[14] = cube[9];
    cube[9] = tmp;

}

void RubiksCube::turnRed1()  {
    unsigned char tmp;

    tmp = cube[45];
    cube[45] = cube[29];
    cube[29] = cube[21];
    cube[21] = cube[5];
    cube[5] = tmp;

    tmp = cube[43];
    cube[43] = cube[27];
    cube[27] = cube[19];
    cube[19] = cube[3];
    cube[3] = tmp;

    tmp = cube[40];
    cube[40] = cube[24];
    cube[24] = cube[16];
    cube[16] = cube[0];
    cube[0] = tmp;

    tmp = cube[15];
    cube[15] = cube[10];
    cube[10] = cube[8];
    cube[8] = cube[13];
    cube[13] = tmp;

    tmp = cube[12];
    cube[12] = cube[9];
    cube[9] = cube[11];
    cube[11] = cube[14];
    cube[14] = tmp;

}

void RubiksCube::turnBlue3()  {
    unsigned char tmp;

    tmp = cube[39];
    cube[39] = cube[24];
    cube[24] = cube[15];
    cube[15] = cube[7];
    cube[7] = tmp;

    tmp = cube[38];
    cube[38] = cube[25];
    cube[25] = cube[14];
    cube[14] = cube[6];
    cube[6] = tmp;

    tmp = cube[37];
    cube[37] = cube[26];
    cube[26] = cube[13];
    cube[13] = cube[5];
    cube[5] = tmp;

    tmp = cube[23];
    cube[23] = cube[21];
    cube[21] = cube[16];
    cube[16] = cube[18];
    cube[18] = tmp;

    tmp = cube[20];
    cube[20] = cube[22];
    cube[22] = cube[19];
    cube[19] = cube[17];
    cube[17] = tmp;

}

void RubiksCube::turnBlue2()  {
    unsigned char tmp;

    tmp = cube[39];
    cube[39] = cube[15];
    cube[15] = tmp;

    tmp = cube[38];
    cube[38] = cube[14];
    cube[14] = tmp;

    tmp = cube[37];
    cube[37] = cube[13];
    cube[13] = tmp;

    tmp = cube[24];
    cube[24] = cube[7];
    cube[7] = tmp;

    tmp = cube[25];
    cube[25] = cube[6];
    cube[6] = tmp;

    tmp = cube[26];
    cube[26] = cube[5];
    cube[5] = tmp;

    tmp = cube[23];
    cube[23] = cube[16];
    cube[16] = tmp;

    tmp = cube[20];
    cube[20] = cube[19];
    cube[19] = tmp;

    tmp = cube[18];
    cube[18] = cube[21];
    cube[21] = tmp;

    tmp = cube[22];
    cube[22] = cube[17];
    cube[17] = tmp;

}

void RubiksCube::turnBlue1()  {
    unsigned char tmp;

    tmp = cube[39];
    cube[39] = cube[7];
    cube[7] = cube[15];
    cube[15] = cube[24];
    cube[24] = tmp;

    tmp = cube[38];
    cube[38] = cube[6];
    cube[6] = cube[14];
    cube[14] = cube[25];
    cube[25] = tmp;

    tmp = cube[37];
    cube[37] = cube[5];
    cube[5] = cube[13];
    cube[13] = cube[26];
    cube[26] = tmp;

    tmp = cube[23];
    cube[23] = cube[18];
    cube[18] = cube[16];
    cube[16] = cube[21];
    cube[21] = tmp;

    tmp = cube[20];
    cube[20] = cube[17];
    cube[17] = cube[19];
    cube[19] = cube[22];
    cube[22] = tmp;

}

void RubiksCube::turnYellow3()  {
    unsigned char tmp;

    tmp = cube[42];
    cube[42] = cube[8];
    cube[8] = cube[21];
    cube[21] = cube[39];
    cube[39] = tmp;

    tmp = cube[41];
    cube[41] = cube[11];
    cube[11] = cube[22];
    cube[22] = cube[36];
    cube[36] = tmp;

    tmp = cube[40];
    cube[40] = cube[13];
    cube[13] = cube[23];
    cube[23] = cube[34];
    cube[34] = tmp;

    tmp = cube[31];
    cube[31] = cube[29];
    cube[29] = cube[24];
    cube[24] = cube[26];
    cube[26] = tmp;

    tmp = cube[28];
    cube[28] = cube[30];
    cube[30] = cube[27];
    cube[27] = cube[25];
    cube[25] = tmp;

}

void RubiksCube::turnYellow2()  {
    unsigned char tmp;

    tmp = cube[42];
    cube[42] = cube[21];
    cube[21] = tmp;

    tmp = cube[41];
    cube[41] = cube[22];
    cube[22] = tmp;

    tmp = cube[40];
    cube[40] = cube[23];
    cube[23] = tmp;

    tmp = cube[39];
    cube[39] = cube[8];
    cube[8] = tmp;

    tmp = cube[36];
    cube[36] = cube[11];
    cube[11] = tmp;

    tmp = cube[34];
    cube[34] = cube[13];
    cube[13] = tmp;

    tmp = cube[31];
    cube[31] = cube[24];
    cube[24] = tmp;

    tmp = cube[28];
    cube[28] = cube[27];
    cube[27] = tmp;

    tmp = cube[26];
    cube[26] = cube[29];
    cube[29] = tmp;

    tmp = cube[30];
    cube[30] = cube[25];
    cube[25] = tmp;

}

void RubiksCube::turnYellow1()  {
    unsigned char tmp;

    tmp = cube[42];
    cube[42] = cube[39];
    cube[39] = cube[21];
    cube[21] = cube[8];
    cube[8] = tmp;

    tmp = cube[41];
    cube[41] = cube[36];
    cube[36] = cube[22];
    cube[22] = cube[11];
    cube[11] = tmp;

    tmp = cube[40];
    cube[40] = cube[34];
    cube[34] = cube[23];
    cube[23] = cube[13];
    cube[13] = tmp;

    tmp = cube[31];
    cube[31] = cube[26];
    cube[26] = cube[24];
    cube[24] = cube[29];
    cube[29] = tmp;

    tmp = cube[28];
    cube[28] = cube[25];
    cube[25] = cube[27];
    cube[27] = cube[30];
    cube[30] = tmp;

}

void RubiksCube::turnOrange3()  {
    unsigned char tmp;

    tmp = cube[47];
    cube[47] = cube[31];
    cube[31] = cube[23];
    cube[23] = cube[7];
    cube[7] = tmp;

    tmp = cube[44];
    cube[44] = cube[28];
    cube[28] = cube[20];
    cube[20] = cube[4];
    cube[4] = tmp;

    tmp = cube[42];
    cube[42] = cube[26];
    cube[26] = cube[18];
    cube[18] = cube[2];
    cube[2] = tmp;

    tmp = cube[39];
    cube[39] = cube[37];
    cube[37] = cube[32];
    cube[32] = cube[34];
    cube[34] = tmp;

    tmp = cube[36];
    cube[36] = cube[38];
    cube[38] = cube[35];
    cube[35] = cube[33];
    cube[33] = tmp;

}

void RubiksCube::turnOrange2()  {
    unsigned char tmp;

    tmp = cube[47];
    cube[47] = cube[23];
    cube[23] = tmp;

    tmp = cube[44];
    cube[44] = cube[20];
    cube[20] = tmp;

    tmp = cube[42];
    cube[42] = cube[18];
    cube[18] = tmp;

    tmp = cube[39];
    cube[39] = cube[32];
    cube[32] = tmp;

    tmp = cube[36];
    cube[36] = cube[35];
    cube[35] = tmp;

    tmp = cube[34];
    cube[34] = cube[37];
    cube[37] = tmp;

    tmp = cube[38];
    cube[38] = cube[33];
    cube[33] = tmp;

    tmp = cube[26];
    cube[26] = cube[2];
    cube[2] = tmp;

    tmp = cube[28];
    cube[28] = cube[4];
    cube[4] = tmp;

    tmp = cube[31];
    cube[31] = cube[7];
    cube[7] = tmp;

}

void RubiksCube::turnOrange1()  {
    unsigned char tmp;

    tmp = cube[47];
    cube[47] = cube[7];
    cube[7] = cube[23];
    cube[23] = cube[31];
    cube[31] = tmp;

    tmp = cube[44];
    cube[44] = cube[4];
    cube[4] = cube[20];
    cube[20] = cube[28];
    cube[28] = tmp;

    tmp = cube[42];
    cube[42] = cube[2];
    cube[2] = cube[18];
    cube[18] = cube[26];
    cube[26] = tmp;

    tmp = cube[39];
    cube[39] = cube[34];
    cube[34] = cube[32];
    cube[32] = cube[37];
    cube[37] = tmp;

    tmp = cube[36];
    cube[36] = cube[33];
    cube[33] = cube[35];
    cube[35] = cube[38];
    cube[38] = tmp;

}

void RubiksCube::turnGreen3()  {
    unsigned char tmp;

    tmp = cube[47];
    cube[47] = cube[45];
    cube[45] = cube[40];
    cube[40] = cube[42];
    cube[42] = tmp;

    tmp = cube[44];
    cube[44] = cube[46];
    cube[46] = cube[43];
    cube[43] = cube[41];
    cube[41] = tmp;

    tmp = cube[34];
    cube[34] = cube[2];
    cube[2] = cube[10];
    cube[10] = cube[29];
    cube[29] = tmp;

    tmp = cube[33];
    cube[33] = cube[1];
    cube[1] = cube[9];
    cube[9] = cube[30];
    cube[30] = tmp;

    tmp = cube[32];
    cube[32] = cube[0];
    cube[0] = cube[8];
    cube[8] = cube[31];
    cube[31] = tmp;

}

void RubiksCube::turnGreen2()  {
    unsigned char tmp;

    tmp = cube[47];
    cube[47] = cube[40];
    cube[40] = tmp;

    tmp = cube[44];
    cube[44] = cube[43];
    cube[43] = tmp;

    tmp = cube[42];
    cube[42] = cube[45];
    cube[45] = tmp;

    tmp = cube[46];
    cube[46] = cube[41];
    cube[41] = tmp;

    tmp = cube[34];
    cube[34] = cube[10];
    cube[10] = tmp;

    tmp = cube[33];
    cube[33] = cube[9];
    cube[9] = tmp;

    tmp = cube[32];
    cube[32] = cube[8];
    cube[8] = tmp;

    tmp = cube[29];
    cube[29] = cube[2];
    cube[2] = tmp;

    tmp = cube[30];
    cube[30] = cube[1];
    cube[1] = tmp;

    tmp = cube[31];
    cube[31] = cube[0];
    cube[0] = tmp;

}

void RubiksCube::turnGreen1()  {
    unsigned char tmp;

    tmp = cube[47];
    cube[47] = cube[42];
    cube[42] = cube[40];
    cube[40] = cube[45];
    cube[45] = tmp;

    tmp = cube[44];
    cube[44] = cube[41];
    cube[41] = cube[43];
    cube[43] = cube[46];
    cube[46] = tmp;

    tmp = cube[34];
    cube[34] = cube[29];
    cube[29] = cube[10];
    cube[10] = cube[2];
    cube[2] = tmp;

    tmp = cube[33];
    cube[33] = cube[30];
    cube[30] = cube[9];
    cube[9] = cube[1];
    cube[1] = tmp;

    tmp = cube[32];
    cube[32] = cube[31];
    cube[31] = cube[8];
    cube[8] = cube[0];
    cube[0] = tmp;

}