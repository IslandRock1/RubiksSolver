#include <iostream>
#include <chrono>
#include <bitset>
#include <map>
#include <stdexcept>
#include <cstdlib>
#include <ctime>

#include "simpleSerial.hpp"
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

enum Hash {
    Two,
    Three,
    Full,
    Whole
};

std::vector<Move> bangMoves(std::vector<Move> &moves0, std::vector<Move> &moves1) {
    std::vector<Move> outMoves;

    for (auto &m : moves0) {
        if (outMoves.empty()) {
            outMoves.emplace_back(m);
            continue;
        }

        if (outMoves.back().face == m.face) {
            outMoves.back().rotations = (outMoves.back().rotations + m.rotations) % 4;
            if (outMoves.back().rotations == 0) {
                outMoves.pop_back();
            }
        } else if ((outMoves.size() > 1) and (outMoves.back().face == RubiksConst::oppositeFaceAll[m.face]) and (outMoves.at(outMoves.size() - 2).face == m.face)) {
            auto ix = outMoves.size() - 2;
            outMoves.at(ix).rotations = (outMoves.at(ix).rotations + m.rotations) % 4;
        } else {
            outMoves.emplace_back(m);
        }
    }

    for (auto &m : moves1) {
        if (outMoves.empty()) {
            outMoves.emplace_back(m);
            continue;
        }

        if (outMoves.back().face == m.face) {
            outMoves.back().rotations = (outMoves.back().rotations + m.rotations) % 4;
            if (outMoves.back().rotations == 0) {
                outMoves.pop_back();
            }
        } else {
            outMoves.emplace_back(m);
        }
    }

    return outMoves;
}

std::vector<Move> bangMoves(std::vector<Move> &moves) {
    std::vector<Move> rest;
    return bangMoves(moves, rest);
}

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

std::vector<Move> execEveryMoveForCrossAndFirstTwo(
        RubiksCube &cube,
        int depth,
        std::vector<Move> &moves,
        std::map<std::array<unsigned int, 4>, std::vector<char>> &lookup,
        std::vector<Solution> &solutions
)
{

    auto lookupSolution = lookup.find(cube.hashCrossAnd2Corners());
    if (lookupSolution != lookup.end()) {
        auto solution = addMovesWithLookup(moves, convertVectorCharToMove(lookup[cube.hashCrossAnd2Corners()]));

        Solution newSol;
        newSol.crossMoves = solution;
        solutions.push_back(newSol);
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

        auto fromRecursed = execEveryMoveForCrossAndFirstTwo(cube, depth - 1, moves, lookup, solutions);

        cube.turn(m.face, 4 - m.rotations);
        moves.pop_back();

        if (!fromRecursed.empty()) {
            return fromRecursed;
        }
    }

//    std::cout << "Returned from latest" << "\n";
    return {};
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

std::vector<Solution> findCrossAnd2Corners(RubiksCube &cube, Lookup &lookup) {
    if ((cube.numCornerSolved() == 2) && cube.solvedWhiteCross()) {return {};}

    std::vector<Move> moves;
    std::vector<Solution> solutions;
    auto solvingMoves = execEveryMoveForCrossAndFirstTwo(cube, 5, moves, lookup.crossAnd2Corners, solutions);

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
            movesFullLayer = convertVectorCharToMove(lookup.solveTwoLayer[hash]);
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

        auto restMoves = convertVectorCharToMove(lookup.solveLastLayer[hash]);
        for (auto &m : restMoves) {
            sol.lastLayerMoves.emplace_back(m);
            cube.turn(m);
        }

        cube.raiseSolved();
    }
}

Lookup loadAllMaps() {
    Lookup lookup;

    lookup.makeCrossAnd2Corners(7);
    printMapSize(lookup.crossAnd2Corners);

//    lookup.makeFirstTwoLayers(7);
//    printMapSize(lookup.firstTwoLayers);

//    lookup.makeWholeCube(7);
//    printMapSize(lookup.wholeCube);

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

//void testSolver() {
//    auto lookup = loadAllMaps();
//
//    RubiksCube cube;
//    auto shuffleMoves = cube.shuffle(30);
//
//    std::cout << "Shuffle ";
//    printMoves(shuffleMoves);
//
//    auto movesCross2Corners = findCrossAnd2Corners(cube, lookup);
//
//    for (auto m : movesCross2Corners) {
//        cube.turn(m);
//    }
//
//    int num2 = cube.numCornerSolved();
//    bool crossSolved = cube.solvedWhiteCross();
//
//    if (num2 < 2) {
//        throw std::runtime_error("Bruh.. 2 corners not solved");
//    }
//
//    if (!crossSolved) {
//        throw std::runtime_error("Bruh.. cross not solved");
//    }
//
//    std::cout << "Cross and 2 ";
//    printMoves(movesCross2Corners);
//
//    auto hash = cube.hashFirstTwoLayers();
//    auto mapIter = lookup.solveTwoLayer.find(hash);
//
//    std::vector<Move> movesFullLayer;
//    if (mapIter != lookup.solveTwoLayer.end()) {
//        movesFullLayer = convertVectorCharToMove(lookup.solveTwoLayer[hash]);
//    } else {
//        movesFullLayer = findFirstTwoLayers(cube, lookup);
//        lookup.solveTwoLayer[hash] = convertVectorMovesToChar(movesFullLayer);
//    }
//
//    for (auto m : movesFullLayer) {
//        cube.turn(m);
//    }
//
//    int num4 = cube.numCornerSolved();
//    crossSolved = cube.solvedWhiteCross();
//
//    if (num4 != 4) {
//        throw std::runtime_error("Bruh.. 4 corners not solved");
//    }
//
//    if (!crossSolved) {
//        throw std::runtime_error("Bruh.. cross not solved");
//    }
//
//    std::cout << "Two layer ";
//    printMoves(movesFullLayer);
//}

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

    int numSolutions = 0;

    int numSolves = 1000;
    std::array<int, 60> solvesForMovelenght = {0};
    for (int i = 0; i < numSolves; i++) {
        std::cout << "Iter: " << i << "\r";

        auto shuffleMoves = cube.shuffle(100, false, i);
        std::array<short, 48> shuffleCubeCopy;
        for (int i = 0; i < 48; i++) {
            shuffleCubeCopy[i] = cube.cube[i];
        }


        //////////////// Cross and 2 //////////////
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
        int fewestMoves = 100;
        for (auto &sol : solutions) {
            auto bang = bangMoves(sol.crossMoves, sol.twoLayerMoves);
            auto banged = bangMoves(bang, sol.lastLayerMoves);

            int num = banged.size();

            if (num < fewestMoves) {
                fewestMoves = num;
                bestSol = sol;
            }
        }

//        printMoves(shuffleMoves);
//
//        auto bang = bangMoves(bestSol.crossMoves, bestSol.twoLayerMoves);
//        auto banged = bangMoves(bang, bestSol.lastLayerMoves);
//
//        printMoves(bestSol.crossMoves);
//        printMoves(bestSol.twoLayerMoves);
//        printMoves(bestSol.lastLayerMoves);
//        printMoves(banged);

        auto numMoves = fewestMoves;
        solvesForMovelenght[numMoves] ++;
        sumMoves += numMoves;

        if (numMoves < minMoves) {
            minMoves = numMoves;
        }

        if (numMoves > maxMoves) {
            maxMoves = numMoves;
        }

        numSolutions += solutions.size();
    }

    std::cout << "Average moves: " << static_cast<double>(sumMoves) / static_cast<double>(numSolves) << ". Max = " << maxMoves << " | Min = " << minMoves << ".\n";

    std::cout << "Average solutions looked through: " << static_cast<double>(numSolutions) / static_cast<double>(numSolves) << "\n";

    std::cout << "Distribution: \n";

    for (auto &num : solvesForMovelenght) {
        std::cout << num << "\n";
    }

}

void testTimeFindCrossAnd2() {
    RubiksCube cube;
    auto lookup = loadAllMaps();

    std::cout << "Starting.." << "\n";
    auto start = std::chrono::high_resolution_clock::now();

    //Insert loop here if needed
    auto solutions = findCrossAnd2Corners(cube, lookup);
    // Loop

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Time taken by find-function: " << duration.count() / 1000 << " milliseconds" << std::endl;
    std::cout << "Num solutions: " << solutions.size() << "\n";
}

int main() {
    RubiksCube cube;
    auto moves = cube.shuffle(10);

    SimpleSerial serial = SimpleSerial("COM3", 115200);
    serial.SendMoves(moves);

    return 17;




    //testTimeFindCrossAnd2();
    testSolveLenght();
    return 17;

    auto lookup = loadAllMaps();


    // RubiksCube cube;
    for (long long i = 0; i < 1; i++) {
        cube.shuffle(50, false, i);
        solveCrossAnd2Corners(cube, lookup);


        auto hash = cube.hashFirstTwoLayers();
        auto movesFullLayer = convertVectorCharToMove(lookup.solveTwoLayer[hash]);

        for (auto m : movesFullLayer) {
            cube.turn(m);
        }

        bool inMap = false;
        for (int t = 0; t < 4; t++) {
            cube.turn('P');
            movesFullLayer.emplace_back('P');
            hash = cube.hashFullCube();

            if (lookup.solveLastLayer.find(hash) != lookup.solveLastLayer.end()) {
                inMap = true;
                break;
            }
        }

        auto movesWhole = lookup.solveLastLayer[hash];
        auto movesLastLayer = convertVectorCharToMove(movesWhole);

        for (auto m : movesWhole) {
            cube.turn(m);
        }

        cube.raiseSolved();

        auto banged = bangMoves(movesFullLayer, movesLastLayer);

        printMoves(movesFullLayer);
        printMoves(movesLastLayer);
        printMoves(banged);

    }
}




















