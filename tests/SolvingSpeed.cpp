
#include <iostream>
#include <chrono>
#include <bitset>

#include "RubiksLibrary/Solver.hpp"
#include "RubiksLibrary/Lookup.hpp"
#include "RubiksLibrary/RubiksCube.hpp"

#define MILLION 1000000

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

std::string bits_to_string_loc_solvingspeed(__int128 x, int bits = 128, bool group_by_bytes = false) {
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

void print_bits_loc_solvingspeed(__int128 x, int bits = 128, bool group_by_bytes = true) {
	std::cout << bits_to_string_loc_solvingspeed(x, bits, group_by_bytes) << '\n';
}

void confirmSameResult() {
	RubiksCube cube;

	int numHashes = 1 * MILLION;
	for (int i = 0; i < numHashes; i++) {
		cube.shuffle(50);
		auto h0 = cube.hashCrossAnd2CornersV1();
		auto h1 = cube.hashCrossAnd2CornersV2();

		if (h0 != h1) {
			std::cout << "Invalid hash..?" << "\n";
			return;
		}
	}

	std::cout << "For " << numHashes << " hashes, the functions are equal." << "\n";
}

void confirmSameResultNew() {
	RubiksCube cube;

	int numHashes = 1 * MILLION;
	for (int i = 0; i < numHashes; i++) {
		cube.shuffle(50);
		auto h0 = cube.hashNewV0();
		auto h1 = cube.hashNewV1();

		if (h0 != h1) {
			std::cout << "Invalid hash..?" << "\n";
			print_bits_loc_solvingspeed(h0);
			print_bits_loc_solvingspeed(h1);
			return;
		}
	}

	std::cout << "For " << numHashes << " hashes, the functions are equal." << "\n";
}

void testHashingSpeed() {
	RubiksCube cube;
	cube.shuffle(50);
	int numHashes = 1 * MILLION;

	auto now = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < numHashes; i++) {
		cube.hashCrossAnd2CornersV2();
	}

	auto after = std::chrono::high_resolution_clock::now();
	auto totTime = std::chrono::duration_cast<std::chrono::microseconds>(after - now).count();
	std::cout << "Total time for " << numHashes << " hashes: " << totTime / 1000 << "ms | Avg time: " << static_cast<double>(totTime) / static_cast<double>(numHashes)
	<< "us\n";

	// v0: Total time for 1000000 hashes: 2094ms | Avg time: 2.09411us
	// v1: Total time for 1000000 hashes:  216ms | Avg time: 0.216571us
	// v2: Total time for 1000000 hashes:  181ms | Avg time: 0.181339us (Old hash)
}

void testNewHashingSpeed() {
	RubiksCube cube;
	cube.shuffle(50);
	int numHashes = 1 * MILLION;

	auto now = std::chrono::high_resolution_clock::now();

	for (int i = 0; i < numHashes; i++) {
		cube.hashNewV1();
	}

	auto after = std::chrono::high_resolution_clock::now();
	auto totTime = std::chrono::duration_cast<std::chrono::microseconds>(after - now).count();
	std::cout << "Total time for " << numHashes << " new hashes: " << totTime / 1000 << "ms | Avg time: " << static_cast<double>(totTime) / static_cast<double>(numHashes)
	<< "us\n";

	// v0: Total time for 1000000 new hashes: 5669ms | Avg time: 5.66993us
	// v1: Total time for 1000000 new hashes: 2058ms | Avg time: 2.05838us
}

void testNumSolvingMoves() {
	Solver solver = Solver();
	RubiksCube cube = RubiksCube();

	auto lookup = Lookup::loadAllMaps();
	std::cout << "Finished loading maps." << "\n";

	unsigned long totNumMoves = 0;

	int num_test = 100;
	auto now = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < num_test; i++) {
		cube.shuffle(50);
		auto solvingMoves = solver.solveFullCubeUsingUnordered(cube, lookup, 5);
		totNumMoves += solvingMoves.size();

		auto t1 = std::chrono::high_resolution_clock::now();
		auto currTime = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - now).count();

		std::cout << i << "/" << num_test << " | " << 100 * static_cast<double>(i) / static_cast<double>(num_test) << "%"
		<< " | Elapsed time: " << currTime << " | Estimated remaining time: "
		<< currTime * ((static_cast<double>(num_test) / static_cast<double>(i)) - 1) << "\r";

	}
	std::cout << "\n\n";
	std::cout << "Solved " << num_test << " cubes, avg number of moves: " << static_cast<double>(totNumMoves) / static_cast<double>(num_test) << ".\n";

	auto after = std::chrono::high_resolution_clock::now();
	auto totTime = std::chrono::duration_cast<std::chrono::milliseconds>(after - now).count();
	std::cout << "Total time: " << totTime << " | Avg time: " << static_cast<double>(totTime) / static_cast<double>(num_test)
	<< "\n";

	// Solved 100 cubes, avg number of moves: 28.73. Total time: 18161 | Avg time: 181.61 (original)
	// Solved 100 cubes, avg number of moves: 28.73. Total time:  8484 | Avg time:  84.84 (faster hashing)
	// Solved 100 cubes, avg number of moves: 28.73. Total time:  6388 | Avg time:  63.88 (unordered map)
}

void testSolving3Corners() {
	Solver solver = Solver();
	RubiksCube cube = RubiksCube();

	Lookup lookup;
	std::string titleBase = DATA_PATH;
	std::string title = titleBase + "/crossAnd3Corners7.txt";
	Lookup::load(lookup.crossAnd3Corners, title);

	for (auto &entries : lookup.crossAnd3Corners) {
		auto key = entries.first;
		auto moves = entries.second;

		lookup.smallerUnorderedCrossAnd3Corners[hashArraySeeded(key, 324598623562517)] = moves;
	}
	std::cout << "Finished loading maps\n";

	unsigned long totNumMoves = 0;

	int num_test = 1;
	auto now = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < num_test; i++) {
		cube.shuffle(50);
		auto solvingMoves = solver.solveUpTo3Corners(cube, lookup, 4);
		totNumMoves += solvingMoves.size();

		auto t1 = std::chrono::high_resolution_clock::now();
		auto currTime = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - now).count();

		std::cout << i << "/" << num_test << " | " << 100 * static_cast<double>(i) / static_cast<double>(num_test) << "%"
		<< " | Elapsed time: " << currTime << " | Estimated remaining time: "
		<< currTime * ((static_cast<double>(num_test) / static_cast<double>(i)) - 1) << "\r";

	}
	std::cout << "\n\n";
	std::cout << "Solved " << num_test << " cubes, avg number of moves: " << static_cast<double>(totNumMoves) / static_cast<double>(num_test) << ".\n";

	auto after = std::chrono::high_resolution_clock::now();
	auto totTime = std::chrono::duration_cast<std::chrono::milliseconds>(after - now).count();
	std::cout << "Total time: " << totTime << " | Avg time: " << static_cast<double>(totTime) / static_cast<double>(num_test)
	<< "\n";
}

void testMoveSpeed() {
	RubiksCube cube;
	const unsigned long long numCycles = 1 * MILLION;
	const unsigned long long numMoves = numCycles * RubiksConst::everyMove.size();

	const auto t0 = std::chrono::high_resolution_clock::now();
	for (const auto m : RubiksConst::everyMove) {
		for (int i = 0; i < numCycles; i++) {
			cube.turn(m);
		}
	}
	const auto t1 = std::chrono::high_resolution_clock::now();
	const auto totTime = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();
	std::cout << "Total time for " << numMoves << " moves: " << totTime / 1000 << "ms | Avg time: " << static_cast<double>(totTime) / static_cast<double>(numMoves)
	<< "us\n";

	// Total time for 18000000 moves: 137ms | Avg time: 0.00762217us (Old, without hashing)
}

int main() {
	// confirmSameResult();
	// testHashingSpeed();
	// testMoveSpeed();
	confirmSameResultNew();
	testNewHashingSpeed();
	// testNumSolvingMoves();
	// testSolving3Corners();
}
