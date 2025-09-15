
#include <iostream>
#include <chrono>

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

int main() {
	// confirmSameResult();
	// testHashingSpeed();

	// testNumSolvingMoves();
	testSolving3Corners();
}
