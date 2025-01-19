
#include <iostream>
#include <chrono>
#include <bitset>
#include <fstream>
#include <map>
#include <stdexcept>

#include "Stopwatch.hpp"
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
        outMoves.emplace_back(m.face, 4 - m.rotations);
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

std::vector<Solution> findCrossAnd2Corners(RubiksCube &cube, Lookup &lookup, int depth = 3) {
    if ((cube.numCornerSolved() == 2) && cube.solvedWhiteCross()) {return {};}

    std::vector<Move> moves;
    std::vector<Solution> solutions;

    SearchConditions searchConditions = {cube, lookup.crossAnd2Corners, moves, solutions, TwoCorners};
    searchMoves(searchConditions, depth);

    if (solutions.empty()) {
        std::cout << "Had to increase depth" << "\n";
        return findCrossAnd2Corners(cube, lookup, depth + 1);
        // throw std::runtime_error("No solution found for this depth-limit and lookup combo.");
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

Lookup loadAllMaps() {
    Lookup lookup;

    std::string titleDesktopOld = "J:/Programmering (Lokalt Minne)/RubiksCubeHashTables";
    std::string titleLaptop = "C:/Users/oyste/Programering Lokal Laptop/RubiksCubeHashTables";
    std::string titleDesktop = "C:/LokalProg/RubiksCubeHashTables";
    std::string title = titleDesktop;

    std::string crossTitle = title + "/crossAnd2Corners7D.txt";
    Lookup::load(lookup.crossAnd2Corners, crossTitle);

    std::string twoTitle = title + "/twoLayer.txt";
    Lookup::load(lookup.solveTwoLayer, twoTitle);

    std::string lastTitle = title + "/lastLayer.txt";
    Lookup::load(lookup.solveLastLayer, lastTitle);

    return lookup;
}

void writeVectorToFile(const std::vector<unsigned long long>& vec, const std::string& filename) {
    // Open the file in write mode
    std::ofstream outFile(filename);

    // Check if the file opened successfully
    if (!outFile) {
        std::cerr << "Error: Could not open the file " << filename << " for writing." << std::endl;
        return;
    }

    // Write each entry of the vector to the file, one per line
    for (const auto& value : vec) {
        outFile << value << std::endl;
    }

    // Close the file
    outFile.close();

    std::cout << "Vector contents written to " << filename << " successfully." << std::endl;
}

void printMapStatistics(const std::map<std::array<unsigned int, 4>, int>& numCubes) {

    static std::vector<unsigned long long> tableSizes;
    static unsigned long long prevPrintIX = 0;

    // Check if the map is empty
    if (numCubes.empty()) {
        std::cout << "The map is empty." << "\n";
        return;
    }

    // Calculate the number of elements and the sum of counts
    unsigned long long totalElements = numCubes.size();
    unsigned long long totalCount = 0;

    for (const auto& pair : numCubes) {
        totalCount += pair.second;
    }

    // Calculate the average count
    double averageCount = static_cast<double>(totalCount) / static_cast<double>(totalElements);

    // Print the results

    tableSizes.push_back(totalElements);

    if (totalElements > prevPrintIX + 100000) {
        writeVectorToFile(tableSizes, std::to_string(totalElements) + ".txt");
        prevPrintIX = totalElements;
    }
    std::cout << "Number of elements in the map: " << totalElements << "\n";
    std::cout << "Average count: " << averageCount << "\n";
}

std::vector<Move> solveFullCube(RubiksCube &cube, Lookup &lookup) {
    static std::map<std::array<unsigned int, 4>, int> numCubes;
    static unsigned long long functionCalls = 0;
    functionCalls++;

    std::array<short, 48> shuffleCubeCopy;
    for (int i = 0; i < 48; i++) {
        shuffleCubeCopy[i] = cube.cube[i];
    }

    auto solutions = findCrossAnd2Corners(cube, lookup, 4);

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

        auto hash = cubeSolutions.hashFirstTwoLayers();
        if (numCubes.find(hash) != numCubes.end()) {
            // Increment the counter if the value is found
            numCubes[hash]++;
        } else {
            // Set the counter to 1 if the value is not found
            numCubes[hash] = 1;
        }
    }

    if (functionCalls % 1000 == 0) {
        printMapStatistics(numCubes);
    }

    return {};

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

void testSolveLenght() {

    Stopwatch stopwatch = Stopwatch();
    Lookup lookup = loadAllMaps();
    auto duration = stopwatch.GetElapsedTime();
    std::cout << "Loaded all maps in " << duration / 1000 << " ms." << "\n";

    RubiksCube cube;

    stopwatch.Restart();
    std::cout << "Finished initializing." << "\n";
    long long sumTimeUS = 0;

    int sumMoves = 0;
    int minMoves = 100;
    int maxMoves = 0;

    int numSolves = 1000000;
    std::array<int, 60> solvesForMovelenght = {0};
    for (int i = 0; i < numSolves; i++) {

        auto time = stopwatch.Restart();
        sumTimeUS += time;
        std::cout << "Solved " << i << " cubes. This one in " << time / 1000 << " ms.\n";

        auto shuffleMoves = cube.shuffle(100, false, i);
        auto moves = solveFullCube(cube, lookup);

        int numMoves = moves.size();

        sumMoves += numMoves;
        if (numMoves > maxMoves) {
            maxMoves = numMoves;
        } else if (numMoves < minMoves) {
            minMoves = numMoves;
        }


    }

    std::cout << "Average moves: " << static_cast<double>(sumMoves) / static_cast<double>(numSolves) << ". Max = " << maxMoves << " | Min = " << minMoves << ".\n";
    std::cout << "Average solving time: " << sumTimeUS / 1000 / numSolves << " ms.\n";
}

SerialPort *esp32;
int main() {

    testSolveLenght();
    return 69;

    std::cout << "Starting loading.\n";
    RubiksCube cube;
    Lookup lookup = loadAllMaps();
    // lookup.crossAnd2Corners[cube.hashCrossAnd2Corners()] = {};

    std::cout << "Finished loading maps\n";

    const char *com_port = "\\\\.\\COM7";
    esp32 = new SerialPort(com_port);

    while (!esp32->isConnected()) {

    }

    std::cout << "Connected to ESP32.";

    // Get shuffle moves
    auto shuffleChars = esp32->getMoves();
    // std::vector<char> shuffleChars = {'P', 'A', 'M', 'D', 'P', 'A', 'G', 'D', 'M', 'A', 'P', 'D', 'G', 'J', 'M'};
    auto shuffleMoves = Move::convertVectorCharToMove(shuffleChars);

    std::cout << "Shuffle moves: \n";
    Move::printMoves(shuffleMoves);

    // Shuffle cube
    for (auto m : shuffleMoves) {
        cube.turn(m);
    }

    auto solvingMoves = solveFullCube(cube, lookup);

    std::cout << "Chars of solving moves: \n";
    for (auto &m : solvingMoves) {
        std::cout << m.move;
    }
    std::cout << "\n";

    auto solvingChars = Move::convertVectorMoveToChar(solvingMoves);

    std::cout << "Solving moves: \n";
    Move::printMoves(solvingMoves);
    Stopwatch watch;
    //Return solving moves
    while (true) {
        esp32->sendMoves(solvingChars);

        while (watch.GetElapsedTime() < 1000000) {}

        watch.Restart();
    }
}




















