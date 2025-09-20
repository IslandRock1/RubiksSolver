
#ifndef RUBIKSSOLVER_LOOKUP_HPP
#define RUBIKSSOLVER_LOOKUP_HPP

#include <unordered_map>
#include <map>
#include <array>
#include <vector>
#include <cstdint>
#include <set>

#include "RubiksLibrary/RubiksCube.hpp"

struct Position {
    std::array<int, 10> currPos = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
};

class Lookup {
public:
    std::map<std::array<unsigned int, 4>, std::vector<char>> firstTwoLayers;
    std::map<std::array<unsigned int, 4>, std::vector<char>> crossAnd2Corners;
    std::set<std::array<unsigned int, 4>> crossAnd2CornersLookupOnly;
    std::unordered_map<uint64_t, std::vector<char>> smallerUnorderedCrossAnd2Corners;
    std::unordered_map<uint64_t, std::vector<char>> smallerUnorderedCrossAnd3Corners;
    std::map<std::array<unsigned int, 4>, std::vector<char>> crossAnd3Corners;
    std::map<std::array<unsigned int, 4>, std::vector<char>> solveLastLayer;
    std::map<std::array<unsigned int, 4>, std::vector<char>> solveTwoLayer;
    std::map<std::array<unsigned int, 4>, std::vector<char>> wholeCube;
    std::map<std::array<unsigned int, 4>, std::vector<char>> combined;
    std::map<std::array<unsigned int, 4>, std::vector<char>> solveFromCrossAnd2Corners;
    std::unordered_map<__int128, std::vector<char>> newHashMap2Corner;

    void makeFirstTwoLayers(int depth);
    void makeCrossAnd2Corners(int depth);
    void makeCrossAnd3Corners(int depth);
    void makeWholeCube(int depth);
    void generateLookupNewHash2Corner(int depth);

    static bool prune(const Move &currentMove, const Move &prevMove, const Move &doublePrevMove);

    static void save(std::unordered_map<__int128, std::vector<char>> &map, const std::string &title);
    static void save(std::set<std::array<unsigned int, 4>> &map, const std::string &title);
    static void save(std::map<std::array<unsigned int, 4>, uint32_t> &map, const std::string &title);
    static void save(std::map<std::pair<uint32_t, uint16_t>, uint32_t>& map, const std::string& title);
    static void save(std::map<std::array<unsigned int, 4>, std::vector<char>> &map, std::string &title);

    static void load(std::unordered_map<__int128, std::vector<char>> &map, std::string &title);
    static void load(std::map<std::array<unsigned int, 4>, std::vector<char>> &map, std::string &title);
    static void load(std::set<std::array<unsigned int, 4>> &map, std::string &title);

    static uint64_t getSize(std::map<std::array<unsigned int, 4>, uint32_t> &map);
    static uint64_t getSize(std::map<std::array<unsigned int, 4>, std::vector<char>> &map);
    static uint64_t getSize(std::map<uint64_t, std::vector<char>> &map);
    static uint64_t getSize(std::map<uint64_t, uint32_t> &map);
    static uint64_t getSize(std::map<std::pair<uint32_t, uint16_t>, uint32_t> &map);
    static Lookup loadAllMaps();

    static void convertAndSave(std::map<std::array<unsigned int, 4>, std::vector<char>> &map, std::string &title);
    static uint64_t hashF(const std::array<unsigned int, 4> &num, uint32_t seed = 321464301);
};


#endif //RUBIKSSOLVER_LOOKUP_HPP
