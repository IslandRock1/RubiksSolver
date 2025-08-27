
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <random>
#include <limits>
#include <chrono>
#include <Lookup.hpp>

/*

#################################

std::string trimmed = num.substr(0, num.size() - 2);
uint64_t hash_value = 0x811C9DC5 ^ seed; // FNV offset basis XOR seed

for (char c : trimmed) {
    hash_value ^= static_cast<uint8_t>(c);
    hash_value *= 0x01000193; // FNV prime
    hash_value &= 0xFFFFFFFFFFFFFF; // Keep it to x bits
}
return hash_value;

Using seed = 588376777
Min: 163424010
Max: 72057593591050776

Delta: 72057593427626766

###############################
std::string trimmed = num.substr(0, num.size() - 2);
uint64_t hash_value = 0x811C9DC5 ^ seed; // FNV offset basis XOR seed

for (char c : trimmed) {
    hash_value ^= static_cast<uint8_t>(c);
    hash_value *= 0x01000193; // FNV prime
    hash_value &= 0xFFFFFFFFFFFF; // Keep it to x bits
}
return hash_value;

Using seed = 217193825
Min: 9288215
Max: 281474962741823
Delta: 281474953453608

With magic number  9635677191019765758 we can reduce by  1 resulting in a max of 9223371938311996177
With magic number  8272628999626919902 we can reduce by  2 resulting in a max of 4611685878853552155
With magic number  6453801953355695072 we can reduce by  3 resulting in a max of 2305842984851628100
With magic number  7221221962654582942 we can reduce by  4 resulting in a max of 1152921466272290257
With magic number 16336438861502074413 we can reduce by  5 resulting in a max of  576460750806585562
With magic number  8933502984661840072 we can reduce by  6 resulting in a max of  288230366881847791
With magic number 12201289853157866913 we can reduce by  7 resulting in a max of  144115187882473563
With magic number  5175146340869780535 we can reduce by  8 resulting in a max of   72057592894135972
With magic number  2535366181321713636 we can reduce by  9 resulting in a max of   36028796792318299
With magic number  6557058883804513686 we can reduce by 10 resulting in a max of   18014398129288844
With magic number    13010701294270543 we can reduce by 11 resulting in a max of    9007198904323779
With magic number  8674217037858742146 we can reduce by 12 resulting in a max of    4503599516570457
With magic number 17213420439901729306 we can reduce by 13 resulting in a max of    2251799751570221
With magic number  8664694160870043275 we can reduce by 14 resulting in a max of    1125899899827503
With magic number 15282091858062745566 we can reduce by 15 resulting in a max of     562949949546608
With magic number  5052632519703722194 we can reduce by 16 resulting in a max of     281474972052757 Woop Woop! Can reduce

32b:                                                                                      4294967296
48b:                                                                                 281474976710656
64b:                                                                            18446744073709551616
Goal:                                                                                       52000000
*/

uint64_t hashMoves(const std::vector<char> &moves, uint32_t seed = 0) {

    /*

    Max moves: 7
    Each move can be A, B, C, ..., Q, R
    18 legal moves.
    18 ** 7 = 612220032
    32b:     4294967296
    30b:     1073741824

    As there are 18 different values, one would need
    5 bits, multiply by 7 we get 35 bits. Too many!

    Therefore, should probably use base 18 number...

    */

    uint64_t num = 0;

    for (auto &m : moves) {
        num *= 18;
        num += (m - 'A');
    }

    return num;
}

uint64_t magicNum(uint64_t num, uint64_t magicNum, uint64_t reduceSize) {
    return (num * magicNum) >> reduceSize;
}

uint64_t hashF(const std::array<unsigned int, 4> &num, uint32_t seed = 0) {
    uint64_t hash_value = 0x811C9DC5 ^ seed; // FNV offset basis XOR seed

    std::string actualString;

    for (auto val : num) {

        auto str = std::to_string(val);

        for (auto i = 0; i < 12 - str.length(); i++) {
            actualString += "0";
        }

        actualString += str;
    }

    for (const char c : actualString) {
        hash_value ^= static_cast<uint8_t>(c);
        hash_value *= 0x01000193; // FNV prime
        hash_value &= 0xFFFFFFFFFFFFFFFF; // Keep it to x bits
    }
    return hash_value;
}

void printInfo(int ix, int bestIx, int numTries) {
    std::cout << "Ix: " << ix
    << " | Progress: " << (100.0 * static_cast<double>(ix) / 52098113) << "%"
    << " | Max progress: " << (100.0 * static_cast<double>(bestIx) / 52098113) << "%"
    << " | Num tries: " << numTries << ".           \r";
}

void printMagicInfo(uint64_t magicNumber, int reducedSize, uint64_t maxValue) {
    std::cout << "\nWith magic number " << magicNumber << " we can reduce by " << reducedSize << " resulting in a max of " << maxValue << "\n";
}

void findBestMagicNumber(std::map<std::array<unsigned int, 4>, std::vector<char>> &lookup) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint64_t> dist(1, std::numeric_limits<uint64_t>::max());

    int sizeToReduce = 1;
    int bestIx = 0;
    int numTries = 0;

    while (true) {
        std::map<uint64_t, bool> smallerMap;
        uint64_t magicNumber = dist(gen);
        bool success = true;
        uint64_t maxNum = 0;
        numTries++;

        int ix = 0;

        auto t0 = std::chrono::high_resolution_clock::now();
        for (const auto& [key, vec] : lookup) {
            ++ix;
            bestIx = std::max(bestIx, ix);

            if (ix % 100000 == 0) {
                printInfo(ix, bestIx, numTries);
            }

            auto newKey = hashF(key, 321464301);
            auto magicKey = magicNum(newKey, magicNumber, sizeToReduce);

            maxNum = std::max(maxNum, magicKey);

            auto lookupIterator = smallerMap.find(magicKey);
            if (lookupIterator != smallerMap.end()) {
                success = false;
                break;
            }
            smallerMap[magicKey] = true;
        }

        if (success) {
            printMagicInfo(magicNumber, sizeToReduce, maxNum);
            sizeToReduce += 1;
            bestIx = 0;
            numTries = 0;
        }

        auto t1 = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
        std::cout << "Elapsed time: " << duration << " milliseconds\n";
        return;
    }
}

void findBestMagicNumber(const std::vector<uint64_t>& arrayBase) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint64_t> dist(1, std::numeric_limits<uint64_t>::max());

    int sizeToReduce = 17;
    int bestIx = 0;
    int numTries = 0;

    while (true) {
        std::unordered_set<uint64_t> smallerSet;
        uint64_t magicNumber = dist(gen);
        bool success = true;
        uint64_t maxNum = 0;
        numTries++;

        int ix = 0;

        // auto t0 = std::chrono::high_resolution_clock::now();
        for (const auto &key : arrayBase) {
            ++ix;

            if (ix % 100000 == 0) {
                printInfo(ix, bestIx, numTries);
            }

            auto magicKey = magicNum(key, magicNumber, sizeToReduce);
            maxNum = std::max(maxNum, magicKey);

            if (smallerSet.contains(magicKey)) {
                bestIx = std::max(bestIx, ix);
                success = false;
                break;
            }
            smallerSet.insert(magicKey);
        }

        if (success) {
            printMagicInfo(magicNumber, sizeToReduce, maxNum);
            sizeToReduce += 1;
            bestIx = 0;
            numTries = 0;
        }

        // auto t1 = std::chrono::high_resolution_clock::now();
        // auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
        // std::cout << "Elapsed time: " << duration << " milliseconds\n";
        // return;
    }
}

void attempt(int numAttempts, std::map<std::array<unsigned int, 4>, std::vector<char>> &table) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint32_t> dist(1, 999999999);

    bool success = true;
    size_t bestIx = 0;

    uint64_t minNum = std::numeric_limits<uint64_t>::max();
    uint64_t maxNum = 0;

    for (int attemptIx = 0; attemptIx < numAttempts; ++attemptIx) {
        success = true;
        std::map<uint64_t, bool> newMap;

        uint32_t seed = dist(gen);
        std::cout << "Using seed = " << seed << "\n";

        minNum = std::numeric_limits<uint64_t>::max();
        maxNum = 0;

        std::string text;
        size_t ix = 0;
        for (const auto& [key, vec] : table) {
            ++ix;
            bestIx = std::max(bestIx, ix);

            if (ix % 100000 == 0) {
                std::cout << "Ix: " << ix
                          << " | Progress: " << (100.0 * static_cast<double>(ix) / 52098113) << "%"
                          << " | Max progress: " << (100.0 * static_cast<double>(bestIx) / 52098113) << "%"
                          << " | Min: " << minNum
                          << " | Max: " << maxNum << "         \r";
                std::cout.flush();
            }

            auto newKey = hashF(key, seed);
            auto lookupIterator = newMap.find(newKey);
            if (lookupIterator != newMap.end()) {
                success = false;
                break;
            }
            newMap[newKey] = true;
        }

        if (success) break;
    }

    std::cout << std::endl;

    if (success) {
        std::cout << "It worked.." << std::endl;
        std::cout << "Min: " << minNum << std::endl;
        std::cout << "Max: " << maxNum << std::endl;
        std::cout << "Delta: " << (maxNum - minNum) << std::endl;
    }
}

void testSmallerTable(std::map<std::array<unsigned int, 4>, std::vector<char>> &table) {
    auto size = Lookup::getSize(table);
    std::cout << "Original size: " << size << ".\n";

    std::map<std::pair<uint32_t, uint16_t>, uint32_t> smallerMap;

    for (const auto& [key, vec] : table) {
        auto newKey = hashF(key, 321464301);
        newKey = magicNum(newKey, 5052632519703722194, 16);

        uint32_t key0 = (newKey >> 16) & 0xFFFFFFFF;
        uint16_t key1 = newKey & 0xFFFF;
        std::pair<uint32_t, uint16_t> keySmol = {key0, key1};

        auto newValue = hashMoves(vec);

        auto iterator = smallerMap.find(keySmol);
        if (iterator == smallerMap.end()) {
            smallerMap[keySmol] = newValue;
        } else {
            std::cout << "Something wrong with keys...\n";
            break;
        }
    }

    auto sizeNew = Lookup::getSize(smallerMap);
    std::cout << "New size: " << sizeNew << ".\n";

    std::cout << "Size saved: " << static_cast<double>(size - sizeNew) / (1024.0 * 1024.0 * 1024.0) << " GB.\n";

    /*

    With uint64_t keys, and uint32_t values.
    Keys:   416784904
    Values: 208392452

    With std::pair<uint32_t, uint16_t> keys, and uint32_t values.
    Keys:
    Values:

    */

}

void testNumDifferentMoveSquences(std::map<std::array<unsigned int, 4>, std::vector<char>> &table) {
    std::unordered_set<uint32_t> moveNumSet;

    for (const auto &[key, val] : table) {

        auto moveVal = hashMoves(val);
        moveNumSet.insert(moveVal);
    }

    auto numEntries = moveNumSet.size();
    std::cout << "Num entries for number: " << numEntries << ".\n";
}

int main() {

    /*

    ################ README! ########################

    For speedup, you can use a set of the hash values (uint64_t).
    That way, checking if this hash is in the table will be a lot faster,
    then you can use the map only when wanting to get the actual moves.

    The moves have been compressed to an uint32_t, so that's great!
    With some luck the keys may be able to be mapped to an uint32_t,
    but seems unlikely, unfortunately.

     */



    auto lookup = Lookup::loadAllMaps().crossAnd2Corners;
    testSmallerTable(lookup);
    return 0;

    // testNumDifferentMoveSquences(lookup);
    // return 1;

    Lookup::getSize(lookup);

    std::vector<uint64_t> arrayBase;

    for (const auto& [key, vec] : lookup) {
        auto newKey = hashF(key, 321464301);
        arrayBase.emplace_back(newKey);
    }

    // All times in IDE, faster from terminal.
    // V0 time: 105270 ms
    // V1 time:  76356 ms # Using an array for keys
    // V2 time:  19335 ms # Using a set to store seen values

    findBestMagicNumber(arrayBase);
    return 1;
}