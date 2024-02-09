//
// Created by Øystein Bringsli.
//

#include <iostream>
#include <random>

#include "RubiksCube.hpp"
#include <iostream>

bool RubiksCube::handleMap(int depth) {
    auto hash = this->hash();
    // std::cout << hash << "\n";

    auto prevDepth = cubeDict.find(hash);

    if (prevDepth != cubeDict.end()) {
        if (cubeDict[hash] <= depth) {

            cacheHits++;
            return true;
        }
    } else {
        cubeDict[hash] = depth;
    }

    return false;
}

std::string RubiksCube::hash() {
    std::string hash;
    for (int v : cube)
    {
        hash += std::to_string(v);
    }

    return hash;
}

std::string RubiksCube::hash() {
    std::string hashString;
    for (int val : cube)
    {
        hashString += std::to_string(val);
    }

    return hashString;
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

void RubiksCube::shuffle(int numMoves) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(0,5); // distribution in range [1, 6]

    for (int i = 0; i < numMoves; i++)
    {
        turn(dist6(rng), 1);
    }
}

void RubiksCube::turn(int face, int rotations) {
    switch (face) {
        case 0: {
            turnWhite(rotations);
        } break;

        case 1: {
            turnBlue(rotations);
        } break;

        case 2: {
            turnRed(rotations);
        } break;

        case 3: {
            turnYellow(rotations);
        } break;

        case 4: {
            turnOrange(rotations);
        } break;

        case 5: {
            turnGreen(rotations);
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

void RubiksCube::turnWhite1()  {
    unsigned char tmp;

//    tmp = cube[53];
//    cube[53] = cube[42];
//    cube[42] = cube[18];
//    cube[18] = cube[11];
//    cube[11] = tmp;
//
//    tmp = cube[52];
//    cube[52] = cube[39];
//    cube[39] = cube[19];
//    cube[19] = cube[14];
//    cube[14] = tmp;
//
//    tmp = cube[51];
//    cube[51] = cube[36];
//    cube[36] = cube[20];
//    cube[20] = cube[17];
//    cube[17] = tmp;
//
//    tmp = cube[8];
//    cube[8] = cube[6];
//    cube[6] = cube[0];
//    cube[0] = cube[2];
//    cube[2] = tmp;
//
//    tmp = cube[5];
//    cube[5] = cube[7];
//    cube[7] = cube[3];
//    cube[3] = cube[1];
//    cube[1] = tmp;

// ----------------------- //
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

//    tmp = cube[53];
//    cube[53] = cube[18];
//    cube[18] = tmp;
//
//    tmp = cube[52];
//    cube[52] = cube[19];
//    cube[19] = tmp;
//
//    tmp = cube[51];
//    cube[51] = cube[20];
//    cube[20] = tmp;
//
//    tmp = cube[42];
//    cube[42] = cube[11];
//    cube[11] = tmp;
//
//    tmp = cube[39];
//    cube[39] = cube[14];
//    cube[14] = tmp;
//
//    tmp = cube[36];
//    cube[36] = cube[17];
//    cube[17] = tmp;
//
//    tmp = cube[8];
//    cube[8] = cube[0];
//    cube[0] = tmp;
//
//    tmp = cube[5];
//    cube[5] = cube[3];
//    cube[3] = tmp;
//
//    tmp = cube[2];
//    cube[2] = cube[6];
//    cube[6] = tmp;
//
//    tmp = cube[7];
//    cube[7] = cube[1];
//    cube[1] = tmp;

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

void RubiksCube::turnWhite3()  {
    unsigned char tmp;

//    tmp = cube[53];
//    cube[53] = cube[11];
//    cube[11] = cube[18];
//    cube[18] = cube[42];
//    cube[42] = tmp;
//
//    tmp = cube[52];
//    cube[52] = cube[14];
//    cube[14] = cube[19];
//    cube[19] = cube[39];
//    cube[39] = tmp;
//
//    tmp = cube[51];
//    cube[51] = cube[17];
//    cube[17] = cube[20];
//    cube[20] = cube[36];
//    cube[36] = tmp;
//
//    tmp = cube[8];
//    cube[8] = cube[2];
//    cube[2] = cube[0];
//    cube[0] = cube[6];
//    cube[6] = tmp;
//
//    tmp = cube[5];
//    cube[5] = cube[1];
//    cube[1] = cube[3];
//    cube[3] = cube[7];
//    cube[7] = tmp;

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

void RubiksCube::turnBlue1()  {
    unsigned char tmp;

//    tmp = cube[51];
//    cube[51] = cube[6];
//    cube[6] = cube[24];
//    cube[24] = cube[29];
//    cube[29] = tmp;
//
//    tmp = cube[48];
//    cube[48] = cube[3];
//    cube[3] = cube[21];
//    cube[21] = cube[32];
//    cube[32] = tmp;
//
//    tmp = cube[45];
//    cube[45] = cube[0];
//    cube[0] = cube[18];
//    cube[18] = cube[35];
//    cube[35] = tmp;
//
//    tmp = cube[17];
//    cube[17] = cube[15];
//    cube[15] = cube[9];
//    cube[9] = cube[11];
//    cube[11] = tmp;
//
//    tmp = cube[14];
//    cube[14] = cube[16];
//    cube[16] = cube[12];
//    cube[12] = cube[10];
//    cube[10] = tmp;

    tmp = cube[45];
    cube[45] = cube[5];
    cube[5] = cube[21];
    cube[21] = cube[26];
    cube[26] = tmp;

    tmp = cube[43];
    cube[43] = cube[3];
    cube[3] = cube[19];
    cube[19] = cube[28];
    cube[28] = tmp;

    tmp = cube[40];
    cube[40] = cube[0];
    cube[0] = cube[16];
    cube[16] = cube[31];
    cube[31] = tmp;

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

void RubiksCube::turnBlue2()  {
    unsigned char tmp;

//    tmp = cube[51];
//    cube[51] = cube[24];
//    cube[24] = tmp;
//
//    tmp = cube[48];
//    cube[48] = cube[21];
//    cube[21] = tmp;
//
//    tmp = cube[45];
//    cube[45] = cube[18];
//    cube[18] = tmp;
//
//    tmp = cube[35];
//    cube[35] = cube[0];
//    cube[0] = tmp;
//
//    tmp = cube[32];
//    cube[32] = cube[3];
//    cube[3] = tmp;
//
//    tmp = cube[29];
//    cube[29] = cube[6];
//    cube[6] = tmp;
//
//    tmp = cube[17];
//    cube[17] = cube[9];
//    cube[9] = tmp;
//
//    tmp = cube[14];
//    cube[14] = cube[12];
//    cube[12] = tmp;
//
//    tmp = cube[11];
//    cube[11] = cube[15];
//    cube[15] = tmp;
//
//    tmp = cube[16];
//    cube[16] = cube[10];
//    cube[10] = tmp;

    tmp = cube[45];
    cube[45] = cube[21];
    cube[21] = tmp;

    tmp = cube[43];
    cube[43] = cube[19];
    cube[19] = tmp;

    tmp = cube[40];
    cube[40] = cube[16];
    cube[16] = tmp;

    tmp = cube[31];
    cube[31] = cube[0];
    cube[0] = tmp;

    tmp = cube[28];
    cube[28] = cube[3];
    cube[3] = tmp;

    tmp = cube[26];
    cube[26] = cube[5];
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

void RubiksCube::turnBlue3()  {
    unsigned char tmp;

//    tmp = cube[51];
//    cube[51] = cube[29];
//    cube[29] = cube[24];
//    cube[24] = cube[6];
//    cube[6] = tmp;
//
//    tmp = cube[48];
//    cube[48] = cube[32];
//    cube[32] = cube[21];
//    cube[21] = cube[3];
//    cube[3] = tmp;
//
//    tmp = cube[45];
//    cube[45] = cube[35];
//    cube[35] = cube[18];
//    cube[18] = cube[0];
//    cube[0] = tmp;
//
//    tmp = cube[17];
//    cube[17] = cube[11];
//    cube[11] = cube[9];
//    cube[9] = cube[15];
//    cube[15] = tmp;
//
//    tmp = cube[14];
//    cube[14] = cube[10];
//    cube[10] = cube[12];
//    cube[12] = cube[16];
//    cube[16] = tmp;

    tmp = cube[45];
    cube[45] = cube[26];
    cube[26] = cube[21];
    cube[21] = cube[5];
    cube[5] = tmp;

    tmp = cube[43];
    cube[43] = cube[28];
    cube[28] = cube[19];
    cube[19] = cube[3];
    cube[3] = tmp;

    tmp = cube[40];
    cube[40] = cube[31];
    cube[31] = cube[16];
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

void RubiksCube::turnRed1()  {
    unsigned char tmp;

//    tmp = cube[44];
//    cube[44] = cube[35];
//    cube[35] = cube[17];
//    cube[17] = cube[8];
//    cube[8] = tmp;
//
//    tmp = cube[43];
//    cube[43] = cube[34];
//    cube[34] = cube[16];
//    cube[16] = cube[7];
//    cube[7] = tmp;
//
//    tmp = cube[42];
//    cube[42] = cube[33];
//    cube[33] = cube[15];
//    cube[15] = cube[6];
//    cube[6] = tmp;
//
//    tmp = cube[26];
//    cube[26] = cube[24];
//    cube[24] = cube[18];
//    cube[18] = cube[20];
//    cube[20] = tmp;
//
//    tmp = cube[23];
//    cube[23] = cube[25];
//    cube[25] = cube[21];
//    cube[21] = cube[19];
//    cube[19] = tmp;

    tmp = cube[39];
    cube[39] = cube[31];
    cube[31] = cube[15];
    cube[15] = cube[7];
    cube[7] = tmp;

    tmp = cube[38];
    cube[38] = cube[30];
    cube[30] = cube[14];
    cube[14] = cube[6];
    cube[6] = tmp;

    tmp = cube[37];
    cube[37] = cube[29];
    cube[29] = cube[13];
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

void RubiksCube::turnRed2()  {
    unsigned char tmp;

//    tmp = cube[44];
//    cube[44] = cube[17];
//    cube[17] = tmp;
//
//    tmp = cube[43];
//    cube[43] = cube[16];
//    cube[16] = tmp;
//
//    tmp = cube[42];
//    cube[42] = cube[15];
//    cube[15] = tmp;
//
//    tmp = cube[35];
//    cube[35] = cube[8];
//    cube[8] = tmp;
//
//    tmp = cube[34];
//    cube[34] = cube[7];
//    cube[7] = tmp;
//
//    tmp = cube[33];
//    cube[33] = cube[6];
//    cube[6] = tmp;
//
//    tmp = cube[26];
//    cube[26] = cube[18];
//    cube[18] = tmp;
//
//    tmp = cube[23];
//    cube[23] = cube[21];
//    cube[21] = tmp;
//
//    tmp = cube[20];
//    cube[20] = cube[24];
//    cube[24] = tmp;
//
//    tmp = cube[25];
//    cube[25] = cube[19];
//    cube[19] = tmp;

    tmp = cube[39];
    cube[39] = cube[15];
    cube[15] = tmp;

    tmp = cube[38];
    cube[38] = cube[14];
    cube[14] = tmp;

    tmp = cube[37];
    cube[37] = cube[13];
    cube[13] = tmp;

    tmp = cube[31];
    cube[31] = cube[7];
    cube[7] = tmp;

    tmp = cube[30];
    cube[30] = cube[6];
    cube[6] = tmp;

    tmp = cube[29];
    cube[29] = cube[5];
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

void RubiksCube::turnRed3()  {
    unsigned char tmp;

//    tmp = cube[44];
//    cube[44] = cube[8];
//    cube[8] = cube[17];
//    cube[17] = cube[35];
//    cube[35] = tmp;
//
//    tmp = cube[43];
//    cube[43] = cube[7];
//    cube[7] = cube[16];
//    cube[16] = cube[34];
//    cube[34] = tmp;
//
//    tmp = cube[42];
//    cube[42] = cube[6];
//    cube[6] = cube[15];
//    cube[15] = cube[33];
//    cube[33] = tmp;
//
//    tmp = cube[26];
//    cube[26] = cube[20];
//    cube[20] = cube[18];
//    cube[18] = cube[24];
//    cube[24] = tmp;
//
//    tmp = cube[23];
//    cube[23] = cube[19];
//    cube[19] = cube[21];
//    cube[21] = cube[25];
//    cube[25] = tmp;

    tmp = cube[39];
    cube[39] = cube[7];
    cube[7] = cube[15];
    cube[15] = cube[31];
    cube[31] = tmp;

    tmp = cube[38];
    cube[38] = cube[6];
    cube[6] = cube[14];
    cube[14] = cube[30];
    cube[30] = tmp;

    tmp = cube[37];
    cube[37] = cube[5];
    cube[5] = cube[13];
    cube[13] = cube[29];
    cube[29] = tmp;

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

void RubiksCube::turnGreen1()  {
    unsigned char tmp;

//    tmp = cube[47];
//    cube[47] = cube[9];
//    cube[9] = cube[24];
//    cube[24] = cube[44];
//    cube[44] = tmp;
//
//    tmp = cube[46];
//    cube[46] = cube[12];
//    cube[12] = cube[25];
//    cube[25] = cube[41];
//    cube[41] = tmp;
//
//    tmp = cube[45];
//    cube[45] = cube[15];
//    cube[15] = cube[26];
//    cube[26] = cube[38];
//    cube[38] = tmp;
//
//    tmp = cube[35];
//    cube[35] = cube[33];
//    cube[33] = cube[27];
//    cube[27] = cube[29];
//    cube[29] = tmp;
//
//    tmp = cube[32];
//    cube[32] = cube[34];
//    cube[34] = cube[30];
//    cube[30] = cube[28];
//    cube[28] = tmp;

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

void RubiksCube::turnGreen2()  {
    unsigned char tmp;

//    tmp = cube[47];
//    cube[47] = cube[24];
//    cube[24] = tmp;
//
//    tmp = cube[46];
//    cube[46] = cube[25];
//    cube[25] = tmp;
//
//    tmp = cube[45];
//    cube[45] = cube[26];
//    cube[26] = tmp;
//
//    tmp = cube[44];
//    cube[44] = cube[9];
//    cube[9] = tmp;
//
//    tmp = cube[41];
//    cube[41] = cube[12];
//    cube[12] = tmp;
//
//    tmp = cube[38];
//    cube[38] = cube[15];
//    cube[15] = tmp;
//
//    tmp = cube[35];
//    cube[35] = cube[27];
//    cube[27] = tmp;
//
//    tmp = cube[32];
//    cube[32] = cube[30];
//    cube[30] = tmp;
//
//    tmp = cube[29];
//    cube[29] = cube[33];
//    cube[33] = tmp;
//
//    tmp = cube[34];
//    cube[34] = cube[28];
//    cube[28] = tmp;

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

void RubiksCube::turnGreen3()  {
    unsigned char tmp;

//    tmp = cube[47];
//    cube[47] = cube[44];
//    cube[44] = cube[24];
//    cube[24] = cube[9];
//    cube[9] = tmp;
//
//    tmp = cube[46];
//    cube[46] = cube[41];
//    cube[41] = cube[25];
//    cube[25] = cube[12];
//    cube[12] = tmp;
//
//    tmp = cube[45];
//    cube[45] = cube[38];
//    cube[38] = cube[26];
//    cube[26] = cube[15];
//    cube[15] = tmp;
//
//    tmp = cube[35];
//    cube[35] = cube[29];
//    cube[29] = cube[27];
//    cube[27] = cube[33];
//    cube[33] = tmp;
//
//    tmp = cube[32];
//    cube[32] = cube[28];
//    cube[28] = cube[30];
//    cube[30] = cube[34];
//    cube[34] = tmp;

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

void RubiksCube::turnOrange1()  {
    unsigned char tmp;

//    tmp = cube[53];
//    cube[53] = cube[27];
//    cube[27] = cube[26];
//    cube[26] = cube[8];
//    cube[8] = tmp;
//
//    tmp = cube[50];
//    cube[50] = cube[30];
//    cube[30] = cube[23];
//    cube[23] = cube[5];
//    cube[5] = tmp;
//
//    tmp = cube[47];
//    cube[47] = cube[33];
//    cube[33] = cube[20];
//    cube[20] = cube[2];
//    cube[2] = tmp;
//
//    tmp = cube[44];
//    cube[44] = cube[42];
//    cube[42] = cube[36];
//    cube[36] = cube[38];
//    cube[38] = tmp;
//
//    tmp = cube[41];
//    cube[41] = cube[43];
//    cube[43] = cube[39];
//    cube[39] = cube[37];
//    cube[37] = tmp;

    tmp = cube[47];
    cube[47] = cube[24];
    cube[24] = cube[23];
    cube[23] = cube[7];
    cube[7] = tmp;

    tmp = cube[44];
    cube[44] = cube[27];
    cube[27] = cube[20];
    cube[20] = cube[4];
    cube[4] = tmp;

    tmp = cube[42];
    cube[42] = cube[29];
    cube[29] = cube[18];
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

//    tmp = cube[53];
//    cube[53] = cube[26];
//    cube[26] = tmp;
//
//    tmp = cube[50];
//    cube[50] = cube[23];
//    cube[23] = tmp;
//
//    tmp = cube[47];
//    cube[47] = cube[20];
//    cube[20] = tmp;
//
//    tmp = cube[44];
//    cube[44] = cube[36];
//    cube[36] = tmp;
//
//    tmp = cube[41];
//    cube[41] = cube[39];
//    cube[39] = tmp;
//
//    tmp = cube[38];
//    cube[38] = cube[42];
//    cube[42] = tmp;
//
//    tmp = cube[43];
//    cube[43] = cube[37];
//    cube[37] = tmp;
//
//    tmp = cube[33];
//    cube[33] = cube[2];
//    cube[2] = tmp;
//
//    tmp = cube[30];
//    cube[30] = cube[5];
//    cube[5] = tmp;
//
//    tmp = cube[27];
//    cube[27] = cube[8];
//    cube[8] = tmp;

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

    tmp = cube[29];
    cube[29] = cube[2];
    cube[2] = tmp;

    tmp = cube[27];
    cube[27] = cube[4];
    cube[4] = tmp;

    tmp = cube[24];
    cube[24] = cube[7];
    cube[7] = tmp;

}

void RubiksCube::turnOrange3()  {
    unsigned char tmp;

//    tmp = cube[53];
//    cube[53] = cube[8];
//    cube[8] = cube[26];
//    cube[26] = cube[27];
//    cube[27] = tmp;
//
//    tmp = cube[50];
//    cube[50] = cube[5];
//    cube[5] = cube[23];
//    cube[23] = cube[30];
//    cube[30] = tmp;
//
//    tmp = cube[47];
//    cube[47] = cube[2];
//    cube[2] = cube[20];
//    cube[20] = cube[33];
//    cube[33] = tmp;
//
//    tmp = cube[44];
//    cube[44] = cube[38];
//    cube[38] = cube[36];
//    cube[36] = cube[42];
//    cube[42] = tmp;
//
//    tmp = cube[41];
//    cube[41] = cube[37];
//    cube[37] = cube[39];
//    cube[39] = cube[43];
//    cube[43] = tmp;

    tmp = cube[47];
    cube[47] = cube[7];
    cube[7] = cube[23];
    cube[23] = cube[24];
    cube[24] = tmp;

    tmp = cube[44];
    cube[44] = cube[4];
    cube[4] = cube[20];
    cube[20] = cube[27];
    cube[27] = tmp;

    tmp = cube[42];
    cube[42] = cube[2];
    cube[2] = cube[18];
    cube[18] = cube[29];
    cube[29] = tmp;

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

void RubiksCube::turnYellow1()  {
    unsigned char tmp;

//    tmp = cube[53];
//    cube[53] = cube[51];
//    cube[51] = cube[45];
//    cube[45] = cube[47];
//    cube[47] = tmp;
//
//    tmp = cube[50];
//    cube[50] = cube[52];
//    cube[52] = cube[48];
//    cube[48] = cube[46];
//    cube[46] = tmp;
//
//    tmp = cube[38];
//    cube[38] = cube[2];
//    cube[2] = cube[11];
//    cube[11] = cube[29];
//    cube[29] = tmp;
//
//    tmp = cube[37];
//    cube[37] = cube[1];
//    cube[1] = cube[10];
//    cube[10] = cube[28];
//    cube[28] = tmp;
//
//    tmp = cube[36];
//    cube[36] = cube[0];
//    cube[0] = cube[9];
//    cube[9] = cube[27];
//    cube[27] = tmp;

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
    cube[10] = cube[26];
    cube[26] = tmp;

    tmp = cube[33];
    cube[33] = cube[1];
    cube[1] = cube[9];
    cube[9] = cube[25];
    cube[25] = tmp;

    tmp = cube[32];
    cube[32] = cube[0];
    cube[0] = cube[8];
    cube[8] = cube[24];
    cube[24] = tmp;

}

void RubiksCube::turnYellow2()  {
    unsigned char tmp;

//    tmp = cube[53];
//    cube[53] = cube[45];
//    cube[45] = tmp;
//
//    tmp = cube[50];
//    cube[50] = cube[48];
//    cube[48] = tmp;
//
//    tmp = cube[47];
//    cube[47] = cube[51];
//    cube[51] = tmp;
//
//    tmp = cube[52];
//    cube[52] = cube[46];
//    cube[46] = tmp;
//
//    tmp = cube[38];
//    cube[38] = cube[11];
//    cube[11] = tmp;
//
//    tmp = cube[37];
//    cube[37] = cube[10];
//    cube[10] = tmp;
//
//    tmp = cube[36];
//    cube[36] = cube[9];
//    cube[9] = tmp;
//
//    tmp = cube[29];
//    cube[29] = cube[2];
//    cube[2] = tmp;
//
//    tmp = cube[28];
//    cube[28] = cube[1];
//    cube[1] = tmp;
//
//    tmp = cube[27];
//    cube[27] = cube[0];
//    cube[0] = tmp;

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

    tmp = cube[26];
    cube[26] = cube[2];
    cube[2] = tmp;

    tmp = cube[25];
    cube[25] = cube[1];
    cube[1] = tmp;

    tmp = cube[24];
    cube[24] = cube[0];
    cube[0] = tmp;

}

void RubiksCube::turnYellow3()  {
    unsigned char tmp;

//    tmp = cube[53];
//    cube[53] = cube[47];
//    cube[47] = cube[45];
//    cube[45] = cube[51];
//    cube[51] = tmp;
//
//    tmp = cube[50];
//    cube[50] = cube[46];
//    cube[46] = cube[48];
//    cube[48] = cube[52];
//    cube[52] = tmp;
//
//    tmp = cube[38];
//    cube[38] = cube[29];
//    cube[29] = cube[11];
//    cube[11] = cube[2];
//    cube[2] = tmp;
//
//    tmp = cube[37];
//    cube[37] = cube[28];
//    cube[28] = cube[10];
//    cube[10] = cube[1];
//    cube[1] = tmp;
//
//    tmp = cube[36];
//    cube[36] = cube[27];
//    cube[27] = cube[9];
//    cube[9] = cube[0];
//    cube[0] = tmp;

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
    cube[34] = cube[26];
    cube[26] = cube[10];
    cube[10] = cube[2];
    cube[2] = tmp;

    tmp = cube[33];
    cube[33] = cube[25];
    cube[25] = cube[9];
    cube[9] = cube[1];
    cube[1] = tmp;

    tmp = cube[32];
    cube[32] = cube[24];
    cube[24] = cube[8];
    cube[8] = cube[0];
    cube[0] = tmp;

}