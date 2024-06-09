#include <iostream>
#include <chrono>
#include <bitset>
#include <map>
#include <stdexcept>
#include <cstdlib>
#include <ctime>

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

enum Hash {
    Two,
    Three,
    Full,
    Whole
};

std::vector<char> convertVectorMovesToChar(const std::vector<Move>& moves) {
    std::vector<char> out;

    for (auto m : moves) {
        out.push_back(m.move);
    }

    return out;
}

std::vector<Move> convertVectorCharToMove(const std::vector<char>& moves) {
    std::vector<Move> out;

    for (auto m : moves) {
        out.emplace_back(m);
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
        cube.turn(m);

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
    Move m('A');

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 100000000; i++) {
        cube.turn(m);
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

std::vector<Move> findCrossAnd2Corners(RubiksCube &cube, Lookup &lookup) {
    if ((cube.numCornerSolved() == 2) && cube.solvedWhiteCross()) {return {};}

    auto lookupSolution = lookup.crossAnd2Corners.find(cube.hashCrossAnd2Corners());
    if (lookupSolution != lookup.crossAnd2Corners.end()) {
        std::vector<Move> outMoves;

        auto hashMoves = lookup.crossAnd2Corners[cube.hashCrossAnd2Corners()];
        std::reverse(hashMoves.begin(), hashMoves.end());

        for (auto m : hashMoves) {

            Move mov = Move(m);
            outMoves.emplace_back(mov.face, static_cast<short>(4 - mov.rotations));
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

void solveCrossAnd2Corners(RubiksCube &cube, Lookup &lookup) {
    auto movesCross2Corners = findCrossAnd2Corners(cube, lookup);

    for (auto m : movesCross2Corners) {
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

std::vector<Move> findFirstTwoLayers(RubiksCube &cube, Lookup &lookup) {
    if ((cube.numCornerSolved() == 4) && cube.solvedWhiteCross()) {return {};}

    auto lookupSolution = lookup.firstTwoLayers.find(cube.hashFirstTwoLayers());
    if (lookupSolution != lookup.firstTwoLayers.end()) {
        std::vector<Move> outMoves;

        auto hashMoves = lookup.firstTwoLayers[cube.hashFirstTwoLayers()];
        std::reverse(hashMoves.begin(), hashMoves.end());

        for (auto m : hashMoves) {
            Move mov = Move(m);
            outMoves.emplace_back(mov.face, static_cast<short>(4 - mov.rotations));
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

void solveFirstTwoLayers(RubiksCube &cube, Lookup &lookup) {

    auto hash = cube.hashFirstTwoLayers();
    auto mapIter = lookup.solveTwoLayer.find(hash);

    std::vector<Move> movesFullLayer;
    if (mapIter != lookup.solveTwoLayer.end()) {
        movesFullLayer = convertVectorCharToMove(lookup.solveTwoLayer[hash]);
    } else {
        movesFullLayer = findFirstTwoLayers(cube, lookup);
        lookup.solveTwoLayer[hash] = convertVectorMovesToChar(movesFullLayer);

        std::string title = "J:/Programmering (Lokalt Minne)/RubiksCubeHashTables/twoLayer.txt";
        Lookup::save(lookup.solveTwoLayer, title);
        std::cout << "Had to save two layer" << "\n";
    }

    for (auto m : movesFullLayer) {
        cube.turn(m);
    }

    int num4 = cube.numCornerSolved();
    auto crossSolved = cube.solvedWhiteCross();

    if (num4 != 4) {
        throw std::runtime_error("Bruh.. 4 corners not solved");
    }

    if (!crossSolved) {
        throw std::runtime_error("Bruh.. cross not solved");
    }
}

std::vector<Move> findWhole(RubiksCube &cube, Lookup &lookup) {
    if (cube.solved()) {return {};}

    auto lookupSolution = lookup.wholeCube.find(cube.hashFullCube());
    if (lookupSolution != lookup.wholeCube.end()) {
        std::vector<Move> outMoves;

        auto hashMoves = lookup.wholeCube[cube.hashFullCube()];
        std::reverse(hashMoves.begin(), hashMoves.end());

        for (auto m : hashMoves) {
            Move mov = Move(m);
            outMoves.emplace_back(mov.face, static_cast<short>(4 - mov.rotations));
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

bool solveLastLayer(RubiksCube &cube, Lookup &lookup) {
    cube.raiseCross();
    cube.raiseTwoLayer();

    bool inMap = false;
    std::array<unsigned int, 4> hash;

    for (int t = 0; t < 4; t++) {
        cube.turn('P');
        hash = cube.hashFullCube();

        if (lookup.solveLastLayer.find(hash) != lookup.solveLastLayer.end()) {
            inMap = true;
            break;
        }
    }

    if (inMap) {
        auto movesWhole = lookup.solveLastLayer[hash];

        for (auto m : movesWhole) {
            cube.turn(m);
        }

        cube.raiseSolved();
    } else {
        hash = cube.hashFullCube();

        auto movesWhole = findWhole(cube, lookup);

        for (auto m : movesWhole) {
            cube.turn(m);
        }

        cube.raiseSolved();

        lookup.solveLastLayer[hash] = convertVectorMovesToChar(movesWhole);
        std::string titleLocal = "J:/Programmering (Lokalt Minne)/RubiksCubeHashTables/lastLayer.txt";
        Lookup::save(lookup.solveLastLayer, titleLocal);
    }

    return inMap;
}

Lookup loadAllMaps() {
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

    title = "J:/Programmering (Lokalt Minne)/RubiksCubeHashTables/twoLayer.txt";
    Lookup::load(lookup.solveTwoLayer, title);
    printMapSize(lookup.solveTwoLayer);

    return lookup;
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

void findNumStartHash() {
    std::map<std::array<unsigned int, 4>, bool> map;

    Lookup lookup;

    lookup.makeCrossAnd2Corners(7);
    printMapSize(lookup.crossAnd2Corners);

    RubiksCube cube;

    long long iters = 1;
    long long inMap = 0;
    long long numEntries = 0;
    while (true) {
        cube.shuffle(50, false, iters);
        solveCrossAnd2Corners(cube, lookup);

        auto hash = cube.hashFirstTwoLayers();
        auto mapIter = map.find(hash);

        if (mapIter != map.end()) {
            inMap++;
        } else {
            map[hash] = true;
            numEntries++;
        }

        iters++;

        if (iters % 1000 == 0) {
            std::cout << "In map vs total iters: " << inMap << "/" << iters << ". Number of entries: " << numEntries << "\n";
        }
    }
}

void testSolver() {
    auto lookup = loadAllMaps();

    RubiksCube cube;
    auto shuffleMoves = cube.shuffle(30);

    std::cout << "Shuffle ";
    printMoves(shuffleMoves);

    auto movesCross2Corners = findCrossAnd2Corners(cube, lookup);

    for (auto m : movesCross2Corners) {
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

    std::cout << "Cross and 2 ";
    printMoves(movesCross2Corners);

    auto hash = cube.hashFirstTwoLayers();
    auto mapIter = lookup.solveTwoLayer.find(hash);

    std::vector<Move> movesFullLayer;
    if (mapIter != lookup.solveTwoLayer.end()) {
        movesFullLayer = convertVectorCharToMove(lookup.solveTwoLayer[hash]);
    } else {
        movesFullLayer = findFirstTwoLayers(cube, lookup);
        lookup.solveTwoLayer[hash] = convertVectorMovesToChar(movesFullLayer);
    }

    for (auto m : movesFullLayer) {
        cube.turn(m);
    }

    int num4 = cube.numCornerSolved();
    crossSolved = cube.solvedWhiteCross();

    if (num4 != 4) {
        throw std::runtime_error("Bruh.. 4 corners not solved");
    }

    if (!crossSolved) {
        throw std::runtime_error("Bruh.. cross not solved");
    }

    std::cout << "Two layer ";
    printMoves(movesFullLayer);
}

void testSolveLenght() {
    Lookup lookup;

    lookup.makeCrossAnd2Corners(7);
    printMapSize(lookup.crossAnd2Corners);

    std::string title = "J:/Programmering (Lokalt Minne)/RubiksCubeHashTables/lastLayer.txt";
    Lookup::load(lookup.solveLastLayer, title);
    printMapSize(lookup.solveLastLayer);

    title = "J:/Programmering (Lokalt Minne)/RubiksCubeHashTables/twoLayer.txt";
    Lookup::load(lookup.solveTwoLayer, title);
    printMapSize(lookup.solveTwoLayer);

    RubiksCube cube;

    int sumMoves = 0;
    int minMoves = 100;
    int maxMoves = 0;

    int numSolves = 100000;
    std::array<int, 60> solvesForMovelenght = {0};
    for (int i = 0; i < numSolves; i++) {
        cube.shuffle(50, false, i);


        //////////////// Cross and 2 //////////////
        auto movesCross2Corners = findCrossAnd2Corners(cube, lookup);

        for (auto m: movesCross2Corners) {
            cube.turn(m);
        }

        ///////////// First two layers //////////////
        auto hash = cube.hashFirstTwoLayers();
        auto mapIter = lookup.solveTwoLayer.find(hash);

        std::vector<Move> movesFullLayer;
        if (mapIter != lookup.solveTwoLayer.end()) {
            movesFullLayer = convertVectorCharToMove(lookup.solveTwoLayer[hash]);
        } else {
            throw std::runtime_error("Had to save two layer table");
        }

        for (auto m: movesFullLayer) {
            cube.turn(m);
        }

        ///////////// Last Layer //////////////
        cube.raiseCross();
        cube.raiseTwoLayer();

        bool inMap = false;

        for (int t = 0; t < 4; t++) {
            cube.turn('P');
            hash = cube.hashFullCube();

            if (lookup.solveLastLayer.find(hash) != lookup.solveLastLayer.end()) {
                inMap = true;
                break;
            }
        }

        std::vector<char> movesWhole;
        if (inMap) {
            movesWhole = lookup.solveLastLayer[hash];

            for (auto m: movesWhole) {
                cube.turn(m);
            }

            cube.raiseSolved();
        } else {
            throw std::runtime_error("Had to save last layer");
        }

        int numMoves = movesCross2Corners.size() + movesFullLayer.size() + movesWhole.size();

        solvesForMovelenght[numMoves] ++;
        sumMoves += numMoves;

        if (numMoves < minMoves) {
            minMoves = numMoves;
        }

        if (numMoves > maxMoves) {
            maxMoves = numMoves;
        }
    }

    std::cout << "Average moves: " << static_cast<double>(sumMoves) / static_cast<double>(numSolves) << ". Max = " << maxMoves << " | Min = " << minMoves << ".\n";

    std::cout << "Distribution: \n";

    for (auto &num : solvesForMovelenght) {
        std::cout << num << "\n";
    }

}

int main() {
    testSolveLenght();
    return 17;

    auto lookup = loadAllMaps();

    auto start = std::chrono::high_resolution_clock::now();
    long long continousInMap = 0;
    long long prevMax = 0;
    long long i = 0;
    while (continousInMap < (1 * 1000 * 1000)) {

        RubiksCube cube;

        auto shuffleMoves = cube.shuffle(30, false, i*48270 + 20000);

        solveCrossAnd2Corners(cube, lookup);
        solveFirstTwoLayers(cube, lookup);
        auto inMap = solveLastLayer(cube, lookup);

        if (inMap) {
            continousInMap++;
        } else {

            if (continousInMap > prevMax) {
                prevMax = continousInMap;
            }

            continousInMap = 0;
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000000.0;

        i++;
        std::cout << "ix: " << i << ". LL: ";
        std::cout << lookup.solveLastLayer.size() << ".";
        std::cout << " TwoL: " << lookup.solveTwoLayer.size() << ". Continous in map: ";
        std::cout << continousInMap << ". Total time: " << duration << " seconds. Current highest streak: ";
        std::cout << prevMax << " cubes.\n";
    }

    std::cout << "\n\n";

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000000.0;

    std::cout << "Found all solutions (probably) in " << duration / 60.0 / 60.0 << " hours.\n";
    std::cout << "Total size: " << lookup.solveLastLayer.size() << "LL, " << lookup.solveTwoLayer.size() << "TL.\n";
    std::cout << "Longest inMap streak: " << prevMax << " solves.\n";

    int numTests = 1000;
    RubiksCube cube;
    srand(static_cast<unsigned int>(time(nullptr)));

    start = std::chrono::high_resolution_clock::now();

    for (int test = 0; test < numTests; test++) {
        cube.shuffle(50, false, rand());

        solveCrossAnd2Corners(cube, lookup);
        solveFirstTwoLayers(cube, lookup);
        solveLastLayer(cube, lookup);

        cube.raiseSolved();
    }

    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000000.0;
    std::cout << "Solved " << numTests << " cubes in " << duration << " seconds.\n";
    std::cout << "Avg time: " << static_cast<double>(duration) * 1000.0 / static_cast<double>(numTests) << " ms.";
}

