
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <random>
#include <limits>
#include <chrono>
#include <ranges>
#include <bitset>

#include "RubiksLibrary/Lookup.hpp"
#include "RubiksLibrary/solution.hpp"

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

inline uint64_t mix64(uint64_t x) {
    x ^= x >> 30;
    x *= 0xbf58476d1ce4e5b9ULL;
    x ^= x >> 27;
    x *= 0x94d049bb133111ebULL;
    x ^= x >> 31;
    return x;
}

uint64_t hashArraySeeded(const std::array<unsigned int, 4>& arr, uint64_t seed) {
    uint64_t h = seed;

    for (const unsigned int x : arr) {
        const auto v = static_cast<uint64_t>(x);
        h ^= mix64(v + seed);   // mix with seed
        h *= 0x9e3779b97f4a7c15ULL; // large odd constant (golden ratio)
        h ^= h >> 32;
    }

    return mix64(h);
}

uint64_t hashF(const std::array<unsigned int, 4> &num, uint64_t seed = 0) {
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

    int sizeToReduce = 32;
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

unsigned int create_int_main(const std::string &str) {
    unsigned int i = 0;
    for (const auto c : str) {
        i *= 10;
        i += c - '0';
    }

    return i;
}

void convertCornerAndThree() {
    Lookup lookup;
    std::string titleBase = DATA_PATH;
    std::string title = titleBase + "/crossAnd3Corners7.txt";
    std::set<uint64_t> setBase;
    Lookup::load(lookup.crossAnd3Corners, title);

    std::ifstream file(title);
    if (!file.is_open()) {
        throw std::runtime_error("Could not find the specified lookuptable.");
    }

    std::string text;
    uint64_t seed = 324598623562517;
    int numLines = 0;
    int maxLines = 0;

    bool succy = true;
    while (true) {
        for (auto &key : lookup.crossAnd3Corners | std::views::keys) {
            auto newKey = hashArraySeeded(key, seed);
            if (setBase.contains(newKey)) {
                // std::cout << "\nNot working.. trying new seed." << "\n";
                std::mt19937_64 rng(12345); // or std::random_device{}() for non-deterministic seed
                seed = rng();
                numLines = 0;
                succy = false;
                setBase = {};
                break;
            }

            setBase.emplace(newKey);
            numLines++;
            maxLines = std::max(maxLines, numLines);

            if (numLines % 10000 == 0) {
                std::cout << "Max lines: " << maxLines << " | Line num: " << numLines << "\r";
            }
        }

        if (succy) {
            std::cout << "\nGot it working with seed: " << seed << ". Yippy!\n";
            break;
        }
    }

    file.close();
}

void testHashHugeMap() {
    std::cout << "Starting.." << "\n";
    Lookup lookup;
    std::string titleBase = DATA_PATH;
    std::string title = titleBase + "/crossAnd2CornersDepth8LookupOnly.txt";
    std::set<uint64_t> setBase;

    std::ifstream file(title);
    if (!file.is_open()) {
        throw std::runtime_error("Could not find the specified lookuptable.");
    }

    std::string text;
    uint64_t seed = 324598623562517;
    bool succsess = true;
    int numLines = 0;
    int maxLines = 0;

    while (true) {
        while (std::getline(file, text)) {
            std::array<unsigned int, 4> key = {0, 0, 0, 0};

            std::string num = text.substr(0, 12);
            key[0] = create_int_main(num);
            num = text.substr(12, 12);
            key[1] = create_int_main(num);
            num = text.substr(24, 12);
            key[2] = create_int_main(num);
            num = text.substr(36, 12);
            key[3] = create_int_main(num);

            auto newKey = hashArraySeeded(key, seed);
            if (setBase.contains(newKey)) {
                // std::cout << "\nNot working.. trying new seed." << "\n";
                std::mt19937_64 rng(12345); // or std::random_device{}() for non-deterministic seed
                seed = rng();
                succsess = false;
                file.clear();
                file.seekg(0, std::ios::beg);
                setBase = {};
                numLines = 0;
                break;
            }

            setBase.emplace(newKey);
            numLines++;
            maxLines = std::max(maxLines, numLines);
            std::cout << "Max lines: " << maxLines << " | Line num: " << numLines << "\r";
        }

        if (succsess) {
            std::cout << "\nGot it working with seed: " << seed << ". Yippy!\n";
            break;
        }
    }

    file.close();
}

void traverseCube(RubiksCube &cube, std::vector<Move> &moves, int depth) {
    if (depth == 0) {return;}
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

        traverseCube(cube, moves, depth - 1);

        cube.turn(m.face, 4 - m.rotations);
        moves.pop_back();
    }
}

std::string bits_to_string(__int128 x, int bits = 128, bool group_by_bytes = false) {
    if (bits <= 0) return {};
    if (bits > 128) bits = 128;

    uint64_t low  = static_cast<uint64_t>(x);
    uint64_t high = static_cast<uint64_t>(x >> 64);

    // bitset produces "MSB..LSB" for each 64-bit chunk
    std::string full = std::bitset<64>(high).to_string() + std::bitset<64>(low).to_string();
    // We want the top 'bits' starting from the MSB side:
    std::string sub = full.substr(128 - bits);

    if (!group_by_bytes) return sub;

    // Insert spaces every 8 bits for readability
    std::string grouped;
    for (size_t i = 0; i < sub.size(); ++i) {
        grouped.push_back(sub[i]);
        if ((i + 1) % 8 == 0 && (i + 1) != sub.size()) grouped.push_back(' ');
    }
    return grouped;
}

void print_bits(__int128 x, int bits = 128, bool group_by_bytes = true) {
    std::cout << bits_to_string(x, bits, group_by_bytes) << '\n';
}

void print_u128(__int128 x) {
    if (x == 0) {
        std::cout << 0;
        return;
    }
    std::string s;
    while (x > 0) {
        s.push_back('0' + (x % 10));
        x /= 10;
    }
    std::reverse(s.begin(), s.end());
    std::cout << s << "\n";
}

int main() {

    /*

    ################ README! ########################

    Going for a new hashing method. Currently a decoder has been made, as well as
    a initializer of some sort. Next step is probably to make it so turns will
    update the hash. Lets goooo!

    Should test both hashing as the moves are done, as well as creating the hash afterwards.
    The afterwards hash can use the cube as the starting point, and then fill in the hash
    rather than using the hash and searching for the pieces.

    Example => Instead of searching for the red and white edge, i pick the
    1/46 edge, find out what colors it is, then place it in the hash.
     */

    RubiksCube cube;
    auto physical = RubiksConst::physicalPieces;

    // Hash is posision of the white edges, connected to green, red, orange, blue.
    // Then the edges corrosponding to RED and green, RED and blue, BLUE and orange, ORANGE and green.
    // Then YELLOW and blue, YELLOW and red, YELLOW and orange, YELLOW and green

    // Then corners WHITE, red, green | WHITE, green, orange | WHITE, blue, red | WHITE, orange, blue
    // YELLOW, red, blue | YELLOW, blue, orange | YELLOW, green, red | YELLOW, orange, green

    //Indicies: 1, 3, 4, 6, 9, 14, 20, 33, 25, 27, 28, 30, 0, 2, 5, 7, 24, 26, 29, 31


    print_u128(cube.hash);
    print_bits(cube.hash);

    // Convert back
    std::array positions = {1, 3, 4, 6, 9, 14, 20, 33, 25, 27, 28, 30, 0, 2, 5, 7, 24, 26, 29, 31};
    std::array reverseds = {31, 29, 26, 24, 7, 5, 2, 0, 30, 28, 27, 25, 33, 20, 14, 9, 6, 4, 3, 1};



    print_bits(cube.hash);




    // auto lookup = Lookup::loadAllMaps().crossAnd2Corners;
    // std::vector<uint64_t> arrayBase;
    // for (const auto& key : lookup | std::views::keys) {
    //     auto newKey = hashF(key, 321464301);
    //     arrayBase.emplace_back(newKey);
    // }
    //
    // // All times in IDE, faster (?) from terminal.
    // // V0 time: 105270 ms
    // // V1 time:  76356 ms # Using an array for keys
    // // V2 time:  19335 ms # Using a set to store seen values
    //
    // findBestMagicNumber(arrayBase);
    // return 1;
}