//
// Created by Øystein Bringsli.
//

#include <iostream>
#include <chrono>
#include <fstream>

#include "Move.hpp"
#include "Lookup.hpp"

#include <ranges>

bool Lookup::prune(Move &currentMove, Move &prevMove, Move &doublePrevMove) {
    if (currentMove.face == prevMove.face) { return true;}

    if ((currentMove.face == doublePrevMove.face) && (RubiksConst::oppositeFaceAll[currentMove.face] == prevMove.face)) {
        return true;
    }

    if (currentMove.face < 3) {
        if (prevMove.face == RubiksConst::oppositeFace[currentMove.face]) {
            return true;
        }
    }

    return false;
}

void printStruct(const Position& s) {
    std::cout << "Array elements: ";
    for (int i : s.currPos) {
        std::cout << i << " ";
    }
    std::cout << "." << std::endl;
}

void generateLookupWholeCube(
        std::map<std::array<unsigned int, 4>, std::vector<char>> &map,
        std::vector<char> &moves,
        RubiksCube &cube,
        int depth,
        Position &currPos
        ) {
    if (depth == 0) { return;}

    auto vals = cube.hashFullCube();

    if (map.find(vals) != map.end()) {
        if (moves.size() < map[vals].size()) {
            map[vals] = moves;
        }
    } else {
        map[vals] = moves;
    }

    auto size = moves.size();
    Move prevMove {7, 7};
    Move doublePrevMove {7, 7};

    if (size > 1) {
        prevMove = Move(moves[size - 1]);
        doublePrevMove = Move(moves[size - 2]);
    } else if (size > 0) {
        prevMove = Move(moves[size - 1]);
    }

    for (auto m : RubiksConst::everyMove) {
        if (depth == 1) {
            if (m.move >= RubiksConst::everyMove[0].move) {
                break;
            }
        }

        if (Lookup::prune(m, prevMove, doublePrevMove)) { continue;}

        if (depth > 7) {
            currPos.currPos[depth] = m.move - 'A';
            printStruct(currPos);
        }


        moves.push_back(m.move);
        cube.turn(m);
        generateLookupWholeCube(map, moves, cube, depth - 1, currPos);
        cube.turn(m.face, 4 - m.rotations);
        moves.pop_back();
    }
}

void generateLookupFirstTwoLayers(
        std::map<std::array<unsigned int, 4>, std::vector<char>> &map,
        std::vector<char> &moves,
        RubiksCube &cube,
        int depth
        )
{
    if (depth == 0) { return;}

    auto vals = cube.hashFirstTwoLayers();

    if (map.find(vals) != map.end()) {
        if (moves.size() < map[vals].size()) {
            map[vals] = moves;
        }
    } else {
        map[vals] = moves;
    }

    auto size = moves.size();
    Move prevMove {7, 7};
    Move doublePrevMove {7, 7};

    if (size > 1) {
        prevMove = Move(moves[size - 1]);
        doublePrevMove = Move(moves[size - 2]);
    } else if (size > 0) {
        prevMove = Move(moves[size - 1]);
    }

    for (auto m : RubiksConst::everyMove) {
        if (Lookup::prune(m, prevMove, doublePrevMove)) { continue;}

        moves.push_back(m.move);
        cube.turn(m);
        generateLookupFirstTwoLayers(map, moves, cube, depth - 1);
        cube.turn(m.face, 4 - m.rotations);
        moves.pop_back();
    }
}

void generateLookupCrossAnd2Corners(
        std::map<std::array<unsigned int, 4>, std::vector<char>> &map,
        std::vector<char> &moves,
        RubiksCube &cube,
        int depth
        )
{
    if (depth == 0) { return;}

    auto vals = cube.hashCrossAnd2Corners();

    if (map.find(vals) != map.end()) {
        if (moves.size() < map[vals].size()) {
            map[vals] = moves;
        }
    } else {
        map[vals] = moves;
    }

    auto size = moves.size();
    Move prevMove {7, 7};
    Move doublePrevMove {7, 7};

    if (size > 1) {
        prevMove = Move(moves[size - 1]);
        doublePrevMove = Move(moves[size - 2]);
    } else if (size > 0) {
        prevMove = Move(moves[size - 1]);
    }

    for (auto m : RubiksConst::everyMove) {
        if (Lookup::prune(m, prevMove, doublePrevMove)) { continue;}

        moves.push_back(m.move);
        cube.turn(m);
        generateLookupCrossAnd2Corners(map, moves, cube, depth - 1);
        cube.turn(m.face, 4 - m.rotations);
        moves.pop_back();
    }
}

void generateLookupCrossAnd3Corners(
        std::map<std::array<unsigned int, 4>, std::vector<char>> &map,
        std::vector<char> &moves,
        RubiksCube &cube,
        int depth
)
{
    if (depth == 0) { return;}

    auto vals = cube.hashCrossAnd3Corners();

    if (map.find(vals) != map.end()) {
        if (moves.size() < map[vals].size()) {
            map[vals] = moves;
        }
    } else {
        map[vals] = moves;
    }

    auto size = moves.size();
    Move prevMove {7, 7};
    Move doublePrevMove {7, 7};

    if (size > 1) {
        prevMove = Move(moves[size - 1]);
        doublePrevMove = Move(moves[size - 2]);
    } else if (size > 0) {
        prevMove = Move(moves[size - 1]);
    }

    for (auto m : RubiksConst::everyMove) {
        if (Lookup::prune(m, prevMove, doublePrevMove)) { continue;}

        moves.push_back(m.move);
        cube.turn(m);
        generateLookupCrossAnd3Corners(map, moves, cube, depth - 1);
        cube.turn(m.face, 4 - m.rotations);
        moves.pop_back();
    }
}

void Lookup::makeWholeCube(int depth) {
    auto start = std::chrono::high_resolution_clock::now();

//    RubiksCube cube;
//    std::vector<char> moves;
//    generateLookupWholeCube(wholeCube, moves, cube, depth + 1);

    std::string title;
    switch (depth) {
        case 8:
        {
            throw std::runtime_error("Sorry not yet");
            title = "J:/Programmering (Lokalt Minne)/RubiksCubeHashTables/wholeCube8D.txt";
        } break;

        case 7:
        {
            title = "J:/Programmering (Lokalt Minne)/RubiksCubeHashTables/wholeCube7D.txt";
        } break;

        case 6:
        {
            title = "J:/Programmering (Lokalt Minne)/RubiksCubeHashTables/wholeCube6D.txt";
        } break;

        default:
        {
            throw std::runtime_error("What? Need better depth for makeWholeCube");
        }
    }

    load(wholeCube, title);

//    RubiksCube cube;
//    std::vector<char> moves;
//
//    Position currPos;
//    generateLookupWholeCube(wholeCube, moves, cube, depth + 2, currPos);

    auto end = std::chrono::high_resolution_clock::now();
    auto durLookup = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "Size of wholeCube table is " << wholeCube.size() << " in " << durLookup.count() / 1000 / 1000 << " seconds." << "\n";
}

void Lookup::makeFirstTwoLayers(int depth) {
    auto start = std::chrono::high_resolution_clock::now();
//    RubiksCube cube;
//    std::vector<char> moves;
//    generateLookupFirstTwoLayers(firstTwoLayers, moves, cube, depth + 1);

    std::string title;
    if (depth == 7) {
        title = "J:/Programmering (Lokalt Minne)/RubiksCubeHashTables/firstTwoLayers7D.txt";
    } else if (depth == 6) {
        title = "J:/Programmering (Lokalt Minne)/RubiksCubeHashTables/firstTwoLayers6D.txt";
    } else {
        throw std::runtime_error("Only depth 6 and 7 made.");
    }

    load(firstTwoLayers, title);

    auto end = std::chrono::high_resolution_clock::now();
    auto durLookup = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "Size of first 2 layers table is " << firstTwoLayers.size() << " in " << durLookup.count() / 1000 / 1000 << " seconds." << "\n";
}

void Lookup::makeCrossAnd2Corners(int depth) {
    auto start = std::chrono::high_resolution_clock::now();
//    RubiksCube cube;
//    std::vector<char> moves;
//    generateLookupCrossAnd2Corners(crossAnd2Corners, moves, cube, depth + 1);

    std::string title;
    if (depth == 7) {
        title = "J:/Programmering (Lokalt Minne)/RubiksCubeHashTables/crossAnd2Corners7D.txt";
    } else if (depth == 6) {
        title = "J:/Programmering (Lokalt Minne)/RubiksCubeHashTables/crossAnd2Corners6D.txt";
    } else {
        throw std::runtime_error("Only depth 6 and 7 made.");
    }

    load(crossAnd2Corners, title);

    auto end = std::chrono::high_resolution_clock::now();
    auto durLookup = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "Size of 2 corner table is " << crossAnd2Corners.size() << " in " << durLookup.count() / 1000 / 1000 << " seconds." << "\n";
}

void Lookup::makeCrossAnd3Corners(int depth) {
    auto start = std::chrono::high_resolution_clock::now();
    RubiksCube cube;
    std::vector<char> moves;
    generateLookupCrossAnd3Corners(crossAnd3Corners, moves, cube, depth + 1);

    auto end = std::chrono::high_resolution_clock::now();
    auto durLookup = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "Size of 3 corner table is " << crossAnd3Corners.size() << " in " << durLookup.count() / 1000 / 1000 << " seconds." << "\n";
}

void Lookup::save(std::map<std::array<unsigned int, 4>, std::vector<char>> &map, std::string &title) {
    std::ofstream file(title);

    for (const auto& entry : map) {
        auto key = entry.first;
        auto moves = entry.second;

        for (auto val : key) {

            auto str = std::to_string(val);

            for (auto i = 0; i < 12 - str.length(); i++) {
                file << "0";
            }

            file << str;
        }

        for (auto m : moves) {
            file << m;
        }

        file << "\n";
    }

    file.close();
}

unsigned int create_int(std::string &str) {
    unsigned int i = 0;
    for (auto c : str) {
        i *= 10;
        i += c - '0';
    }

    return i;
}

void Lookup::load(std::map<std::array<unsigned int, 4>, std::vector<char>> &map, std::string &title) {
    std::ifstream file(title);
    std::string text;

    while (std::getline(file, text)) {
        std::array<unsigned int, 4> key = {0, 0, 0, 0};

        std::string num = text.substr(0, 12);
        key[0] = create_int(num);
        num = text.substr(12, 12);
        key[1] = create_int(num);
        num = text.substr(24, 12);
        key[2] = create_int(num);
        num = text.substr(36, 12);
        key[3] = create_int(num);

        std::vector<char> moves;
        auto movs = text.substr(48);
        for (auto m : movs) {
            moves.push_back(m);
        }

        map[key] = moves;
    }

    file.close();
}

size_t Lookup::getSize(std::map<std::array<unsigned int, 4>, std::vector<char>>& map) {
    size_t total = 0;
    constexpr size_t mapNodeOverhead = 40; // STL-dependent, this is an approximation
    unsigned long long numEntries = 0;

    size_t sizeKey = 0;
    size_t sizeValue = 0;

    for (const auto& [key, vec] : map) {
        total += mapNodeOverhead;
        total += sizeof(key);
        total += sizeof(vec);
        total += vec.capacity(); // each char is 1 byte

        numEntries += 1;

        sizeKey += sizeof(key);
        sizeValue += sizeof(vec);
        sizeValue += vec.capacity();
    }


    std::cout << "Size keys: " << sizeKey << " | Size values: " << sizeValue << " | Total size: " << total << ".\n";
    std::cout << "Num entries: " << numEntries << "\n";
    return total;
}

uint64_t Lookup::getSize(std::map<uint64_t, std::vector<char>>& map) {
    size_t total = 0;
    constexpr size_t mapNodeOverhead = 40; // STL-dependent, this is an approximation
    unsigned long long numEntries = 0;

    for (const auto& [key, vec] : map) {
        total += mapNodeOverhead;
        total += sizeof(key);
        total += sizeof(vec);
        total += vec.capacity(); // each char is 1 byte

        numEntries += 1;
    }

    std::cout << "Num entries: " << numEntries << "\n";
    return total;
}

uint64_t Lookup::getSize(std::map<uint64_t, uint32_t>& map) {
    size_t total = 0;
    constexpr size_t mapNodeOverhead = 40; // STL-dependent, this is an approximation
    unsigned long long numEntries = 0;

    size_t sizeKey = 0;
    size_t sizeValue = 0;

    for (const auto& [key, vec] : map) {
        total += mapNodeOverhead;
        total += sizeof(key);
        total += sizeof(vec);

        sizeKey += sizeof(key);
        sizeValue += sizeof(vec);

        numEntries += 1;
    }

    std::cout << "Size keys: " << sizeKey << " | Size values: " << sizeValue << " | Total size: " << total << ".\n";
    std::cout << "Num entries: " << numEntries << "\n";
    return total;
}

uint64_t Lookup::getSize(std::map<std::pair<uint32_t, uint16_t>, uint32_t>& map) {
    size_t total = 0;
    constexpr size_t mapNodeOverhead = 40; // STL-dependent, this is an approximation
    unsigned long long numEntries = 0;

    size_t sizeKey = 0;
    size_t sizeValue = 0;

    for (const auto& [key, vec] : map) {
        total += mapNodeOverhead;
        total += sizeof(key);
        total += sizeof(vec);

        sizeKey += sizeof(key);
        sizeValue += sizeof(vec);

        numEntries += 1;
    }

    std::cout << "Size keys: " << sizeKey << " | Size values: " << sizeValue << " | Total size: " << total << ".\n";
    std::cout << "Num entries: " << numEntries << "\n";
    return total;
}


Lookup Lookup::loadAllMaps() {
    Lookup lookup;

    std::string titleDesktopOld = "J:/Programmering (Lokalt Minne)/RubiksCubeHashTables";
    std::string titleLaptop = "C:/Users/oyste/Programering Lokal Laptop/RubiksCubeHashTables";
    std::string titleDesktop = "C:/LokalProg/RubiksCubeHashTables";
    std::string title = titleDesktop;

    std::string crossTitle = title + "/crossAnd2Corners7D.txt";
    load(lookup.crossAnd2Corners, crossTitle);

    std::string twoTitle = title + "/twoLayer.txt";
    load(lookup.solveTwoLayer, twoTitle);

    std::string lastTitle = title + "/lastLayer.txt";
    load(lookup.solveLastLayer, lastTitle);

    return lookup;
}