#include <iostream>
#include <chrono>
#include <bitset>
#include <map>
#include <stdexcept>

#include "RubiksCube.hpp"
#include "Lookup.hpp"

std::vector<Move> addMovesWithLookup(const std::vector<Move>& moves, std::vector<Move> lookupMoves, bool reverse = true) {
    std::vector<Move> outMoves;

    for (auto m : moves) {
        outMoves.emplace_back(m);
    }

    if (reverse) {
        std::reverse(lookupMoves.begin(), lookupMoves.end());
    }

    for (auto m : lookupMoves) {
        outMoves.push_back({m.face, 4 - m.rotations});
    }

    return outMoves;
}

enum Hash {
    Two,
    Three,
    Full,
    Whole
};

std::vector<char> convertVectorMovesToChar(const std::vector<Move>& moves) {
    std::vector<char> out;

    for (auto m : moves) {
        out.push_back(Lookup::moveToChar(m));
    }

    return out;
}

std::vector<Move> convertVectorCharToMove(const std::vector<char>& moves) {
    std::vector<Move> out;

    for (auto m : moves) {
        out.push_back(Lookup::charToMove(m));
    }

    return out;
}

std::vector<Move> execEveryMove(
        RubiksCube &cube,
        int depth,
        std::vector<Move> &moves,
        std::map<std::array<unsigned int, 4>, std::vector<char>> &lookup,
        Hash hash
        )
{

    std::_Rb_tree_iterator<std::pair<const std::array<unsigned int, 4>, std::vector<char>>> lookupSolution;
    switch (hash) {
        case Two:
        {
            lookupSolution = lookup.find(cube.hashCrossAnd2Corners());
            if (lookupSolution != lookup.end()) {
                return addMovesWithLookup(moves, convertVectorCharToMove(lookup[cube.hashCrossAnd2Corners()]));
            }
        } break;

        case Three:
        {
            lookupSolution = lookup.find(cube.hashCrossAnd3Corners());
            if (lookupSolution != lookup.end()) {
                return addMovesWithLookup(moves, convertVectorCharToMove(lookup[cube.hashCrossAnd3Corners()]));
            }
        } break;

        case Full:
        {
            lookupSolution = lookup.find(cube.hashFirstTwoLayers());
            if (lookupSolution != lookup.end()) {
                return addMovesWithLookup(moves, convertVectorCharToMove(lookup[cube.hashFirstTwoLayers()]));
            }
        } break;

        case Whole:
        {
            lookupSolution = lookup.find(cube.hashFullCube());
            if (lookupSolution != lookup.end()) {
                return addMovesWithLookup(moves, convertVectorCharToMove(lookup[cube.hashFullCube()]));
            }
        } break;
    }

    if (depth == 0) {
        return {};
    }

    auto size = moves.size();

    Move prevMove {7, 7};
    Move doublePrevMove {7, 7};

    if (size > 1) {
        prevMove = moves[size - 1];
        doublePrevMove = moves[size - 2];
    } else if (size > 0) {
        prevMove = moves[size - 1];
    }

    for (Move m : RubiksConst::everyMove)
    {
        if (Lookup::prune(m, prevMove, doublePrevMove)) { continue;}

        moves.push_back(m);
        cube.turn(m.face, m.rotations);

        auto fromRecursed = execEveryMove(cube, depth - 1, moves, lookup, hash);

        cube.turn(m.face, 4 - m.rotations);
        moves.pop_back();

        if (!fromRecursed.empty()) {
            return fromRecursed;
        }
    }

//    std::cout << "Returned from latest" << "\n";
    return {};
}

void printMoves(std::vector<Move> &moves) {
    std::cout << "Moves: ";

    for (auto m : moves) {
        switch (m.face) {
            case 0:
            {
                std::cout << "U";
            } break;

            case 1:
            {
                std::cout << "L";
            } break;

            case 2:
            {
                std::cout << "F";
            } break;

            case 3:
            {
                std::cout << "B";
            } break;

            case 4:
            {
                std::cout << "R";
            } break;

            case 5:
            {
                std::cout << "D";
            } break;
        }

        std::cout << m.rotations << " ";
    }

    std::cout << "\n";
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

void printHash(std::array<unsigned int, 4> &hash, bool bit = false) {
    if (bit) {
        std::cout << std::bitset<32>(hash[0]) << " " << std::bitset<32>(hash[1]) << " " << std::bitset<32>(hash[2]) << " " << std::bitset<32>(hash[3]) << "\n";
    } else {
        std::cout << hash[0] << " " << hash[1] << " " << hash[2] << " " << hash[3] << "\n";
    }
}

void testHashSpeed() {
    RubiksCube cube;

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000000; i++) {
        cube.hashCrossAnd3Corners();
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Time taken by hash-function: " << duration.count() / 1000 << " milliseconds" << std::endl;
}

void printMapSize(const std::map<std::array<unsigned int, 4>, std::vector<char>>& myMap) {
    auto start = std::chrono::high_resolution_clock::now();
    std::size_t totalSize = 0;

    long long totalMoves = 0;
    long long numEntries = 0;

    for (const auto& entry : myMap) {
        totalSize += sizeof(entry.first); // Size of the key

        for (const auto move : entry.second) {
            totalSize += sizeof(move); // Size of each Move object
            totalMoves++;
        }

        numEntries++;
    }

    // Convert total size to megabytes
    double totalSizeMB = static_cast<double>(totalSize) / 1024.0 / 1024.0;
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "Total moves: " << totalMoves << " | Avg moves: " << static_cast<double>(totalMoves) / static_cast<double>(numEntries) << " | Num entries: " << numEntries << ".\n";

    std::cout << "Total size of map: " << totalSize << " bytes. Size found in " << duration.count() / 1000 << " milliseconds." << std::endl;
    // std::cout << "Total size of map: " << std::fixed << std::setprecision(2) << totalSizeMB << " MB. Size found in " << duration.count() / 1000 << " milliseconds." << std::endl;
}

std::vector<Move> solveCrossAnd2Corners(RubiksCube &cube, Lookup &lookup) {
    if ((cube.numCornerSolved() == 2) && cube.solvedWhiteCross()) {return {};}

    auto lookupSolution = lookup.crossAnd2Corners.find(cube.hashCrossAnd2Corners());
    if (lookupSolution != lookup.crossAnd2Corners.end()) {
        std::vector<Move> outMoves;

        auto hashMoves = lookup.crossAnd2Corners[cube.hashCrossAnd2Corners()];
        std::reverse(hashMoves.begin(), hashMoves.end());

        for (auto m : hashMoves) {

            Move mov = Lookup::charToMove(m);
            outMoves.push_back({mov.face, 4 - mov.rotations});
        }

        std::reverse(hashMoves.begin(), hashMoves.end());
        return outMoves;
    }

    std::vector<Move> moves;
    for (unsigned short depth = 4; depth < 10; depth++) {
        auto solvingMoves = execEveryMove(cube, depth, moves, lookup.crossAnd2Corners, Hash::Two);
        if (!solvingMoves.empty()) {
            return solvingMoves;
        }
    }

    throw std::runtime_error("No solution found for this depth-limit and lookup combo.");
}

std::vector<Move> solveFirstTwoLayers(RubiksCube &cube, Lookup &lookup) {
    if ((cube.numCornerSolved() == 4) && cube.solvedWhiteCross()) {return {};}

    auto lookupSolution = lookup.firstTwoLayers.find(cube.hashFirstTwoLayers());
    if (lookupSolution != lookup.firstTwoLayers.end()) {
        std::vector<Move> outMoves;

        auto hashMoves = lookup.firstTwoLayers[cube.hashFirstTwoLayers()];
        std::reverse(hashMoves.begin(), hashMoves.end());

        for (auto m : hashMoves) {
            Move mov = Lookup::charToMove(m);
            outMoves.push_back({mov.face, 4 - mov.rotations});
        }

        std::reverse(hashMoves.begin(), hashMoves.end());
        return outMoves;
    }

    std::vector<Move> moves;
    for (unsigned short depth = 4; depth < 10; depth++) {
        auto solvingMoves = execEveryMove(cube, depth, moves, lookup.firstTwoLayers, Hash::Full);
        if (!solvingMoves.empty()) {
            return solvingMoves;
        }
    }

    throw std::runtime_error("No solution found for this depth-limit and lookup combo.");
}

std::vector<Move> solveWhole(RubiksCube &cube, Lookup &lookup) {
    if (cube.solved()) {return {};}

    auto lookupSolution = lookup.wholeCube.find(cube.hashFullCube());
    if (lookupSolution != lookup.wholeCube.end()) {
        std::vector<Move> outMoves;

        auto hashMoves = lookup.wholeCube[cube.hashFullCube()];
        std::reverse(hashMoves.begin(), hashMoves.end());

        for (auto m : hashMoves) {
            Move mov = Lookup::charToMove(m);
            outMoves.push_back({mov.face, 4 - mov.rotations});
        }

        std::reverse(hashMoves.begin(), hashMoves.end());
        return outMoves;
    }

    std::vector<Move> moves;
    for (unsigned short depth = 4; depth < 10; depth++) {
        auto solvingMoves = execEveryMove(cube, depth, moves, lookup.wholeCube, Hash::Whole);
        if (!solvingMoves.empty()) {
            return solvingMoves;
        }
    }

    throw std::runtime_error("No solution found for this depth-limit and lookup combo.");
}

void compareMap(std::map<std::array<unsigned int, 4>, std::vector<char>> &map0, std::map<std::array<unsigned int, 4>, std::vector<char>> &map1) {

    for (const auto& entry : map0) {
        auto key = entry.first;
        auto moves = entry.second;

        if (map1[key] != moves) {
            std::cout << "Nuh" << "\n";
            throw std::runtime_error("Huh..");
        }
    }

    std::cout << "Comparison succsesfull!" << "\n";
}

void sizeMap(std::map<std::array<unsigned int, 4>, std::vector<char>> &map) {

    long long totMoves = 0;
    long long numEntries = 0;
    int maxMoves = 0;
    int minMoves = 1000;

    for (const auto &entry : map) {
        auto moves = entry.second;

        totMoves += moves.size();
        numEntries++;

        if (moves.size() > maxMoves) {
            maxMoves = moves.size();
        }

        if (moves.size() < minMoves) {
            minMoves = moves.size();
        }
    }

    std::cout << "Max moves: " << maxMoves << " | Min moves: " << minMoves << " | Total moves: ";
    std::cout << totMoves << " | Avg moves: " << static_cast<double>(totMoves) / static_cast<double>(numEntries) << "\n";

}

int main() {
    // throw std::runtime_error("Remember to implement solveTwoLayer Lookup!");

    Lookup lookup;

    lookup.makeCrossAnd2Corners(7);
    printMapSize(lookup.crossAnd2Corners);

    lookup.makeFirstTwoLayers(7);
    printMapSize(lookup.firstTwoLayers);

    lookup.makeWholeCube(7);
    printMapSize(lookup.wholeCube);

    std::string title = "J:/Programmering (Lokalt Minne)/RubiksCubeHashTables/lastLayer.txt";
    Lookup::load(lookup.solveLastLayer, title);
    printMapSize(lookup.solveLastLayer);

    int numSolves = 10000000;
    auto startGlob = std::chrono::high_resolution_clock::now();

    long long sumTime = 0;
    double maxTime = 0;
    int llSolved = 0;

    for (int i = 0; i < numSolves; i++) {
        auto start = std::chrono::high_resolution_clock::now();

        RubiksCube cube;

        auto shuffleMoves = cube.shuffle(30, false, i);

        auto start2Corners = std::chrono::high_resolution_clock::now();
        auto movesCross2Corners = solveCrossAnd2Corners(cube, lookup);
        auto end2Corners = std::chrono::high_resolution_clock::now();
        auto duration2Corners = std::chrono::duration_cast<std::chrono::microseconds>(end2Corners - start2Corners).count();

        for (auto m : movesCross2Corners) {
            cube.turn(m.face, m.rotations);
        }

        int num2 = cube.numCornerSolved();
        bool crossSolved = cube.solvedWhiteCross();

        if (num2 < 2) {
            std::cout << "Nuh uh!" << "\n";
            return 69;
        }

        if (!crossSolved) {
            throw std::runtime_error("Bruh.. cross not solved");
        }

        auto start4Corners = std::chrono::high_resolution_clock::now();
        auto movesFullLayer = solveFirstTwoLayers(cube, lookup);
        auto end4Corners = std::chrono::high_resolution_clock::now();
        auto duration4Corners = std::chrono::duration_cast<std::chrono::microseconds>(end4Corners - start4Corners).count();

        for (auto m : movesFullLayer) {
            cube.turn(m.face, m.rotations);
        }

        int num4 = cube.numCornerSolved();
        crossSolved = cube.solvedWhiteCross();

        if (num4 != 4) {
            std::cout << "Bruh.." << "\r";
            return 2;
        }

        if (!crossSolved) {
            throw std::runtime_error("Bruh.. cross not solved");
        }

        bool inMap = false;
        for (int t = 0; t < 4; t++) {
            cube.turn(5, 1);
            auto hash = cube.hashFullCube();

            if (lookup.solveLastLayer.find(hash) != lookup.solveLastLayer.end()) {
                inMap = true;
                break;
            }
        }

        if (!inMap) {
            auto hash = cube.hashFullCube();

            auto movesWhole = solveWhole(cube, lookup);

            for (auto m : movesWhole) {
                cube.turn(m.face, m.rotations);
            }

            if (!cube.solved()) {
                std::cout << "Whole cube not solved.." << "\n";
                return 2;
            }

            lookup.solveLastLayer[hash] = convertVectorMovesToChar(movesWhole);
            std::string titleLocal = "J:/Programmering (Lokalt Minne)/RubiksCubeHashTables/lastLayer.txt";
            Lookup::save(lookup.solveLastLayer, titleLocal);

            llSolved++;
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

        if (!inMap) {
            sumTime += duration;
        }

        auto currentTime = static_cast<double>(duration) / 1000.0;

        if (currentTime > maxTime) {
            maxTime = currentTime;
        }

        std::cout << "Unique cubes: ";
        std::cout << lookup.solveLastLayer.size() << "/" << i + 1 << ".";
        std::cout << " This one in " << currentTime << " ms. Sumtime: ";
        std::cout << static_cast<double>(sumTime) / 1000.0 / 1000.0 / 60.0 / 60.0 << " hours. Average time: ";
        std::cout << static_cast<double>(sumTime) / static_cast<double>(llSolved) / 1000.0 / 1000.0 << " seconds." << "\n";
        //std::cout << static_cast<double>(duration2Corners) / 1000.0 << " ms. Time for first two layers: ";
        //std::cout << static_cast<double>(duration4Corners) / 1000.0 << " ms." << "\n";
        //std::cout << static_cast<double>(durationWhole) / 1000000.0 << " seconds." << "\n";

    }

    auto endGlob = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endGlob - startGlob).count();
    std::cout << "Found solutions in " << static_cast<double>(duration) / 1000.0 << " milliseconds." << std::endl;
    std::cout << "Average time: " << static_cast<double>(duration) / 1000.0 / static_cast<double>(numSolves) << " milliseconds." << std::endl;
}

