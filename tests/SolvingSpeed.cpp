
#include <iostream>
#include <chrono>

#include "RubiksLibrary/Solver.hpp"
#include "RubiksLibrary/Lookup.hpp"
#include "RubiksLibrary/RubiksCube.hpp"

int main() {
	Solver solver = Solver();
	RubiksCube cube = RubiksCube();

	auto lookup = Lookup::loadAllMaps();
	std::cout << "Finished loading maps." << "\n";

	unsigned long totNumMoves = 0;

	int num_test = 1000;
	auto now = std::chrono::high_resolution_clock::now();
	for (int i = 0; i < num_test; i++) {
		cube.shuffle(50);
		auto solvingMoves = solver.solveFullCube(cube, lookup);
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
