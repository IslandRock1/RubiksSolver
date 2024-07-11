#include <iostream>
#include <chrono>
#include <bitset>
#include <map>
#include <stdexcept>

#include "SerialPort.hpp"
#include "solution.hpp"
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
        outMoves.emplace_back(m.face, static_cast<short>(4 - m.rotations));
    }

    return outMoves;
}

struct SearchConditions {
    RubiksCube &cube;
    std::map<std::array<unsigned int, 4>, std::vector<char>> &lookup;
    std::vector<Move> &moves;
    std::vector<Solution> &solutions;
    Hash hash;
};

void searchMoves(SearchConditions &searchConditions, int depth) {
    auto &cube = searchConditions.cube;
    auto &lookup = searchConditions.lookup;

    auto lookupIterator = lookup.find(cube.getFromHash(searchConditions.hash));
    if (lookupIterator != lookup.end()) {
        auto lookupChars = lookup[cube.getFromHash(searchConditions.hash)];
        auto lookupMoves = Move::convertVectorCharToMove(lookupChars);
        auto solution = addMovesWithLookup(searchConditions.moves, lookupMoves);

        Solution newSol;
        newSol.crossMoves = solution;
        searchConditions.solutions.push_back(newSol);
    }

    if (depth == 0) {return;}

    auto &moves = searchConditions.moves;
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
        cube.turn(m);

        searchMoves(searchConditions, depth - 1);

        cube.turn(m.face, 4 - m.rotations);
        moves.pop_back();
    }
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

std::vector<Solution> findCrossAnd2Corners(RubiksCube &cube, Lookup &lookup) {
    if ((cube.numCornerSolved() == 2) && cube.solvedWhiteCross()) {return {};}

    std::vector<Move> moves;
    std::vector<Solution> solutions;

    SearchConditions searchConditions = {cube, lookup.crossAnd2Corners, moves, solutions, TwoCorners};
    searchMoves(searchConditions, 5);

    if (solutions.empty()) {
        throw std::runtime_error("No solution found for this depth-limit and lookup combo.");
    } else {
        return solutions;
    }
}

std::vector<Solution> solveCrossAnd2Corners(RubiksCube &cube, Lookup &lookup) {
    auto solutions = findCrossAnd2Corners(cube, lookup);

    std::array<short, 48> cubeCopy;
    for (int i = 0; i < 48; i++) {
        cubeCopy[i] = cube.cube[i];
    }

    for (auto &solution : solutions) {
        for (int i = 0; i < 48; i++) {
            cube.cube[i] = cubeCopy[i];
        }

        for (auto m : solution.crossMoves) {
            cube.turn(m);
        }

        int num2 = cube.numCornerSolved();
        bool crossSolved = cube.solvedWhiteCross();

        if (num2 < 2) {
            throw std::runtime_error("Bruh.. 2 corners not solved");
        }

        if (!crossSolved) {
            throw std::runtime_error("Bruh.. cross not solved");
        }
    }

    return solutions;
}

void findAndTestSolutionsFirstTwoLayers(std::array<short, 48> &shuffled, Lookup &lookup, std::vector<Solution> &solutions) {

    for (auto &sol : solutions) {

        RubiksCube cube;
        for (int i = 0; i < 48; i++) {
            cube.cube[i] = shuffled[i];
        }

        for (auto &m : sol.crossMoves) {
            cube.turn(m);
        }

        auto hash = cube.hashFirstTwoLayers();
        auto mapIter = lookup.solveTwoLayer.find(hash);

        std::vector<Move> movesFullLayer;
        if (mapIter != lookup.solveTwoLayer.end()) {
            movesFullLayer = Move::convertVectorCharToMove(lookup.solveTwoLayer[hash]);
        } else {
            throw std::runtime_error("Had to save two layer table");
        }

        for (auto m: movesFullLayer) {
            cube.turn(m);
            sol.twoLayerMoves.emplace_back(m);
        }

        cube.raiseCross();
        cube.raiseTwoCorners();
        cube.raiseTwoLayer();
    }
}

void findAndTestSolutionsLastLayer(std::array<short, 48> &shuffled, Lookup &lookup, std::vector<Solution> &solutions) {

    for (auto &sol : solutions) {

        RubiksCube cube;
        for (int i = 0; i < 48; i++) {
            cube.cube[i] = shuffled[i];
        }

        for (auto &m : sol.crossMoves) {
            cube.turn(m);
        }

        for (auto &m : sol.twoLayerMoves) {
            cube.turn(m);
        }

        auto hash = cube.hashFullCube();
        bool inMap = false;
        for (int t = 0; t < 4; t++) {
            cube.turn('P');
            sol.lastLayerMoves.emplace_back('P');
            hash = cube.hashFullCube();

            if (lookup.solveLastLayer.find(hash) != lookup.solveLastLayer.end()) {
                inMap = true;
                break;
            }
        }

        auto restMoves = Move::convertVectorCharToMove(lookup.solveLastLayer[hash]);
        for (auto &m : restMoves) {
            sol.lastLayerMoves.emplace_back(m);
            cube.turn(m);
        }

        cube.raiseSolved();
    }
}

void printMapInfo(std::map<std::array<unsigned int, 4>, std::vector<char>> &table, long long time) {
    std::cout << "Loaded map in " << time / 1000000 << " seconds." << "\n";
    // printMapSize(table);
}

Lookup loadAllMaps(bool printInfo = false) {
    Lookup lookup;

    auto start = std::chrono::high_resolution_clock::now();
    std::string titleLaptop = "C:/Users/oyste/Programering Lokal Laptop/RubiksCubeHashTables/crossAnd2Corners7D.txt";
    Lookup::load(lookup.crossAnd2Corners, titleLaptop);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    if (printInfo) {
        printMapInfo(lookup.crossAnd2Corners, duration);
    }

    start = std::chrono::high_resolution_clock::now();
    std::string titleDesktop = "J:/Programmering (Lokalt Minne)/RubiksCubeHashTables/twoLayer.txt";
    titleLaptop = "C:/Users/oyste/Programering Lokal Laptop/RubiksCubeHashTables/twoLayer.txt";
    Lookup::load(lookup.solveTwoLayer, titleLaptop);
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    if (printInfo) {
        printMapInfo(lookup.solveTwoLayer, duration);
    }

    start = std::chrono::high_resolution_clock::now();
    titleDesktop = "J:/Programmering (Lokalt Minne)/RubiksCubeHashTables/lastLayer.txt";
    titleLaptop = "C:/Users/oyste/Programering Lokal Laptop/RubiksCubeHashTables/lastLayer.txt";
    Lookup::load(lookup.solveLastLayer, titleLaptop);
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    if (printInfo) {
        printMapInfo(lookup.solveLastLayer, duration);
    }

    return lookup;
}

void findNumStartHash() {
    std::map<std::array<unsigned int, 4>, bool> map;

    Lookup lookup;

    std::string titleLaptop = "C:/Users/oyste/Programering Lokal Laptop/RubiksCubeHashTables/crossAnd2Corners7D.txt";
    Lookup::load(lookup.crossAnd2Corners, titleLaptop);
    std::cout << "Finished loading map" << "\n";

    RubiksCube cube;

    long long iters = 0;
    long long inMap = 0;
    long long numEntries = 0;
    while (true) {
        cube.shuffle(50, false, iters);
        auto solutions = solveCrossAnd2Corners(cube, lookup);

        auto hash = cube.hashFirstTwoLayers();
        auto mapIter = map.find(hash);

        if (mapIter != map.end()) {
            inMap++;
        } else {
            map[hash] = true;
            numEntries++;
        }

        iters++;

        std::cout << "Solved " << iters << " cubes." << "\n";
        if (iters % 1000 == 0) {
            std::cout << "In map vs total iters: " << inMap << "/" << iters << ". Number of entries: " << numEntries << "\n";
        }
    }
}

std::vector<Move> solveFullCube(RubiksCube &cube, Lookup &lookup) {
    std::array<short, 48> shuffleCubeCopy;
    for (int i = 0; i < 48; i++) {
        shuffleCubeCopy[i] = cube.cube[i];
    }

    auto solutions = findCrossAnd2Corners(cube, lookup);

    for (auto &solution : solutions) {
        RubiksCube cubeSolutions;
        for (int i = 0; i < 48; i++) {
            cubeSolutions.cube[i] = shuffleCubeCopy[i];
        }

        for (auto &m : solution.crossMoves) {
            cubeSolutions.turn(m);
        }

        cubeSolutions.raiseCross();
        cubeSolutions.raiseTwoCorners();
    }

    findAndTestSolutionsFirstTwoLayers(shuffleCubeCopy, lookup, solutions);
    findAndTestSolutionsLastLayer(shuffleCubeCopy, lookup, solutions);

    Solution bestSol;
    std::vector<Move> out;
    int fewestMoves = 100;
    for (auto &sol : solutions) {
        std::vector<std::vector<Move>> allMoves = {sol.crossMoves, sol.twoLayerMoves, sol.lastLayerMoves};
        auto combinedMoves = Move::combineMoves(allMoves);

        int num = combinedMoves.size();

        if (num < fewestMoves) {
            fewestMoves = num;
            bestSol = sol;

            out = combinedMoves;
        }
    }

    return out;
}

SerialPort *esp32;
int main() {
    findNumStartHash();
    return 1;

    RubiksCube cube;
    Lookup lookup = loadAllMaps();


    return 17;
    const char *com_port = "\\\\.\\COM3";
    esp32 = new SerialPort(com_port);

    std::cout << "Is connected: " << esp32->isConnected() << "\n";

    // Get shuffle moves
    auto shuffleChars = esp32->getMoves();
    auto shuffleMoves = Move::convertVectorCharToMove(shuffleChars);

    printMoves(shuffleMoves);

    // Shuffle cube
    for (auto m : shuffleMoves) {
        cube.turn(m);
    }

    auto solvingMoves = solveFullCube(cube, lookup);
    auto solvingChars = Move::convertVectorMoveToChar(solvingMoves);

    printMoves(solvingMoves);
    //Return solving moves
    esp32->sendMoves(solvingChars);
}




















