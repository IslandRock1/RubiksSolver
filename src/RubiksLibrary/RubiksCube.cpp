
#include <iostream>
#include <random>
#include <bitset>
#include <cstdint>

#include "RubiksLibrary/RubiksCube.hpp"

#include <set>
#include <unordered_map>

std::array<Move, 18> RubiksConst::everyMove = {
        Move('A'), Move('B'), Move('C'), Move('D'), Move('E'), Move('F'),
        Move('G'), Move('H'), Move('I'), Move('J'), Move('K'), Move('L'),
        Move('M'), Move('N'), Move('O'), Move('P'), Move('Q'), Move('R')
};

RubiksCube::RubiksCube() {
    std::array positions = {1, 3, 4, 6, 9, 14, 20, 33, 25, 27, 28, 30, 0, 2, 5, 7, 24, 26, 29, 31};

    hash = 0;
    for (const auto p : positions) {
        hash <<= 6;
        hash += p;
    }
}

std::array<short, 48> RubiksCube::getCubeFromHash() {
    auto physical = RubiksConst::physicalPieces;
    auto colors = RubiksConst::colors;

    std::array<short, 48> cubeLoc = {9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9};

    auto hashCopy = hash;
    for (int ix = 0; ix < 20; ix++) {
        auto Yog = static_cast<int>(hashCopy & 0b111111);
        hashCopy >>= 6;
        const auto& piece = physical[Yog];
        if (piece.size() > 1) {
            std::array<int, 3> indicises = {Yog, piece[0], piece[1]};
            const auto& col = colors[ix];

            for (int i = 0; i < 3; i++) {
                cubeLoc[indicises[i]] = col[i];
            }
        } else if (piece.size() == 1) {
            std::array<int, 3> indicises = {Yog, piece[0]};
            const auto& col = colors[ix];

            for (int i = 0; i < 2; i++) {
                cubeLoc[indicises[i]] = col[i];
            }
        }
    }

    return cubeLoc;
}

std::string bits_to_string_loc(__int128 x, int bits = 128, bool group_by_bytes = false) {
    if (bits <= 0) return {};
    if (bits > 128) bits = 128;

    uint64_t low  = static_cast<uint64_t>(x);
    uint64_t high = static_cast<uint64_t>(x >> 64);

    // bitset produces "MSB..LSB" for each 64-bit chunk
    std::string full = std::bitset<64>(high).to_string() + std::bitset<64>(low).to_string();
    // We want the top 'bits' starting from the MSB side:
    std::string sub = full.substr(128 - bits);

    if (!group_by_bytes) return sub;

    // Insert spaces every 6 bits for readability
    std::string grouped;
    for (size_t i = 0; i < sub.size(); ++i) {
        grouped.push_back(sub[i]);
        if ((i + 5) % 6 == 0 && (i + 5) != sub.size()) grouped.push_back(' ');
    }
    return grouped;
}

void print_bits_loc(__int128 x, int bits = 128, bool group_by_bytes = true) {
    std::cout << bits_to_string_loc(x, bits, group_by_bytes) << '\n';
}

inline void set_bits(__int128 &x, const unsigned pos, const unsigned width, const unsigned value) {
    const __int128 mask = ((static_cast<__int128>(1) << width) - 1) << pos;
    x = (x & ~mask) | ((static_cast<__int128>(value) << pos) & mask);
}

void RubiksCube::hashNew() {
    // Hash is posision of the white edges, connected to green, red, orange, blue.
    // Then the edges corrosponding to RED and green, RED and blue, BLUE and orange, ORANGE and green.
    // Then yellow and BLUE, yellow and RED, YELLOW and orange, YELLOW and green

    // Then corners WHITE, red, green | WHITE, green, orange | WHITE, blue, red | WHITE, orange, blue
    // yellow, RED, blue | yellow, BLUE, orange | yellow, green, RED | YELLOW, orange, green

    //Indicies: 1, 3, 4, 6, 9, 14, 20, 33, 25, 27, 28, 30, 0, 2, 5, 7, 24, 26, 29, 31


    // TODO: hash is not working, so fix it!

    std::unordered_map<int, int> colorComboLookupEdges {
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

    std::unordered_map<int, int> colorComboLookupCorners {
        {137,  0}, // YELLOW, orange, green => green * 36 + orange * 6 + yellow (smallest value first) | BitPosition 0
        { 59,  6}, // yellow, green, RED | BitPosition 6, aka 0bHEREXXXXXX (x is irrelevant)
        {101, 12}, // yellow, BLUE, orange
        { 53, 18}, // yellow, RED, blue

        { 16, 24}, // WHITE, orange, blue
        {  8, 30}, // WHITE, blue, red
        { 22, 36}, // WHITE, green, orange
        {  9, 42}, // WHITE, red, green
    };

    auto physicalPieces = RubiksConst::physicalPieces;

    auto prevHash = hash;
    hash = 0;
    for (int i = 0; i < 48; i++) {
        auto piece = physicalPieces[i];
        if (piece.size() == 1) {
            int ix = piece[0];

            if (cube[i] > cube[ix]) {continue;}

            // Indicies is i and ix
            auto col0 = std::min(cube[i], cube[ix]);
            auto col1 = std::max(cube[i], cube[ix]);
            // col0 and col1 are the colors of the edge
            // For example 0 and 1, aka white and red.

            // The colors are between 0 and 5 (including), so if I use
            // min * 6 + max, I can index into an array, that says where in the
            // hash this position should be saved.

            // Position given in how many bits to the right it is maybe?
            // Then i can do like hash and = 0b000000 << position
            // Then do hash or = cubePosition << position

            auto colorCombo = col0 * 6 + col1;
            auto position = colorComboLookupEdges[colorCombo];

            set_bits(hash, position, 6, i);

            std::cout << "--EDGE-- Position: " << position << " ----- Value: " << i << " --------------\n";
            print_bits_loc(prevHash);
            print_bits_loc(hash);

        } else {

            int ix0 = piece[0];
            int ix1 = piece[1];

            if (cube[0] > cube[ix0]) {continue;}
            if (cube[0] > cube[ix1]) {continue;}
            if (cube[ix0] > cube[ix1]) {continue;}

            // Indicies is i and ix
            auto col0 = std::min(cube[i], std::min(cube[ix0], cube[ix1]));
            auto col2 = std::max(cube[i], std::max(cube[ix0], cube[ix1]));

            short col1;
            if ((col0 < cube[i]) and (col2 > cube[i])) {
                col1 = cube[i];
            } else if ((col0 < cube[ix0]) and (col2 > cube[ix0])) {
                col1 = cube[ix0];
            } else {
                col1 = cube[ix1];
            }

            // col0 and col1 are the colors of the edge
            // For example 0 and 1, aka white and red.

            // The colors are between 0 and 5 (including), so if I use
            // min * 6 + max, I can index into an array, that says where in the
            // hash this position should be saved.

            // Position given in how many bits to the right it is maybe?
            // Then i can do like hash and = 0b000000 << position
            // Then do hash or = cubePosition << position

            auto colorCombo = col0 * 36 + col1 * 6 + col2;
            auto position = colorComboLookupCorners[colorCombo];

            set_bits(hash, position, 6, i);

            std::cout << "--CORNER-- Position: " << position << " ----- Value: " << i << " --------------\n";
            print_bits_loc(prevHash);
            print_bits_loc(hash);
        }
    }
}


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

std::array<unsigned int, 4> RubiksCube::hashCrossAnd2CornersV1() const {
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
                const auto& piece0 = RubiksConst::physicalPieces[indices[newIx]];
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

                    const auto orange = (currentFace == 4) + (cube[piece0[0]] == 4);
                    const auto green = (currentFace == 3) + (cube[piece0[0]] == 3);
                    const auto blue = (currentFace == 2) + (cube[piece0[0]] == 2);

                    const bool c0 = (cube[piece0[0]] == 5) || (currentFace == 5);
                    const bool c1 = (orange && green);
                    const bool c2 = (orange && blue);
                    const bool c = (c0 || c1 || c2);

                    output[newIx] =
                        5 * (c) + output[newIx] * (!c);
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

std::array<unsigned int, 4> RubiksCube::hashCrossAnd2CornersV2() {
    std::array<unsigned int, 4> vals = {0, 0, 0, 0};

    for (int i = 0; i < 4; i++) {
        unsigned int val = 0;
        for (int k = 0; k < 4; k++) {

            const auto ix0 = 12 * i + 3 * k + 0;
            const auto ix1 = ix0 + 1;
            const auto ix2 = ix0 + 2;

            const auto face0 = cube[ix0];
            const auto face1 = cube[ix1];
            const auto face2 = cube[ix2];

            const std::array<int, 3> indices = {ix0, ix1, ix2};
            const std::array<short, 3> faces = {face0, face1, face2};
            std::array<int, 3> output = {0, 0, 0};

            for (int newIx = 0; newIx < 3; newIx++) {
                const auto& piece0 = RubiksConst::physicalPieces[indices[newIx]];
                const auto currentFace = faces[newIx];
                output[newIx] = faces[newIx];

                if (piece0.size() > 1) {
                    auto white = (currentFace == 0) + (cube[piece0[0]] == 0) + (cube[piece0[1]] == 0);
                    auto orange = (currentFace == 4) + (cube[piece0[0]] == 4) + (cube[piece0[1]] == 4);
                    auto green = (currentFace == 3) + (cube[piece0[0]] == 3) + (cube[piece0[1]] == 3);
                    auto blue = (currentFace == 2) + (cube[piece0[0]] == 2) + (cube[piece0[1]] == 2);

                    bool c0 = (cube[piece0[0]] == 5) || (cube[piece0[1]] == 5) || (currentFace == 5);
                    bool c1 = ((white + orange + green) == 3);
                    bool c2 = ((white + orange + blue) == 3);
                    bool c = (c0 || c1 || c2);

                    output[newIx] =
                        5 * (c) + output[newIx] * (!c);

                } else {

                    const auto orange = (currentFace == 4) + (cube[piece0[0]] == 4);
                    const auto green = (currentFace == 3) + (cube[piece0[0]] == 3);
                    const auto blue = (currentFace == 2) + (cube[piece0[0]] == 2);

                    const bool c0 = (cube[piece0[0]] == 5) || (currentFace == 5);
                    const bool c1 = (orange && green);
                    const bool c2 = (orange && blue);
                    const bool c = (c0 || c1 || c2);

                    output[newIx] =
                        5 * (c) + output[newIx] * (!c);
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
                const auto& piece0 = RubiksConst::physicalPieces[indices[newIx]];
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

std::array<unsigned int, 4> RubiksCube::getFromHash(Hash hash) {
    switch (hash) {
        case TwoCorners:
        {
            return hashCrossAnd2CornersV1();
        } break;

        case ThreeCorners:
        {
            return hashCrossAnd3Corners();
        } break;

        case FirstTwoLayers:
        {
            return hashFirstTwoLayers();
        } break;

        case WholeCube:
        {
            return hashFullCube();
        } break;
    }

    throw std::runtime_error("Wrong hash or something. RubiksCube.cpp, getFromHash(Hash)");
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

void RubiksCube::print(const std::array<short, 48>& cube) {
    int space = 0;

    for (const auto val : cube) {
        std::cout << std::to_string(val) << " ";

        if (space == 7) {
            std::cout << " | ";
            space = -1;
        }

        space++;
    }

    std::cout << "\n";
}


bool RubiksCube::solved() const {
    std::array<short, 48> tmp = {0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,5,5,5,5,5,5,5,5,4,4,4,4,4,4,4,4,3,3,3,3,3,3,3,3};
    return (cube == tmp);
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
        shuffleMoves.emplace_back(static_cast<short>(face), static_cast<short>(num));

        if (print) {
            std::cout << "f" << face << "n" << num << " | ";
        }
    }

    if (print) {std::cout << "\n";}
    return shuffleMoves;
}

void RubiksCube::raiseCross() {
    if (!solvedWhiteCross()) {
        throw std::runtime_error("Cross not solved.");
    }
}

void RubiksCube::raiseTwoCorners() {
    if (numCornerSolved() < 2) {
        throw std::runtime_error("Not solved two corners.");
    }
}

void RubiksCube::raiseThreeCorners() {
    if (numCornerSolved() < 3) {
        throw std::runtime_error("Not solved three corners.");
    }
}

void RubiksCube::raiseTwoLayer() {
    if (numCornerSolved() < 4) {
        throw std::runtime_error("Not solved 4 corners");
    }
}

void RubiksCube::raiseSolved() {
    if (!solved()) {
        throw std::runtime_error("Cube not solved.");
    }
}

void RubiksCube::turn(Move m) {
    switch (m.move) {
        case 'A':
        {
            turnWhite1();
        } break;

        case 'B':
        {
            turnWhite2();
        } break;

        case 'C':
        {
            turnWhite3();
        } break;

        case 'D':
        {
            turnRed1();
        } break;

        case 'E':
        {
            turnRed2();
        } break;

        case 'F':
        {
            turnRed3();
        } break;

        case 'G':
        {
            turnBlue1();
        } break;

        case 'H':
        {
            turnBlue2();
        } break;

        case 'I':
        {
            turnBlue3();
        } break;

        case 'J':
        {
            turnGreen1();
        } break;

        case 'K':
        {
            turnGreen2();
        } break;

        case 'L':
        {
            turnGreen3();
        } break;

        case 'M':
        {
            turnOrange1();
        } break;

        case 'N':
        {
            turnOrange2();
        } break;

        case 'O':
        {
            turnOrange3();
        } break;

        case 'P':
        {
            turnYellow1();
        } break;

        case 'Q':
        {
            turnYellow2();
        } break;

        case 'R':
        {
            turnYellow3();
        } break;
    }
}

void RubiksCube::turn(char m) {
    Move move = Move(m);
    turn(move);
}

void RubiksCube::turn(int face, int rotations) {
    turn(Move(face, rotations));
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