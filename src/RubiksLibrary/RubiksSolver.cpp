
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <iostream>
#include <vector>

#include "RubiksLibrary/Solver.hpp"
#include "RubiksLibrary/Lookup.hpp"

class RubiksSolver {
public:
	RubiksSolver();
	~RubiksSolver();

	std::vector<char> solve(const std::vector<int> &input);

private:
	Lookup lookup;
};


RubiksSolver::RubiksSolver() {
	std::cout << "Initializing maps..." << "\n";
	lookup = Lookup::loadAllMaps();
	std::cout << "Maps initialized!" << "\n";
}

RubiksSolver::~RubiksSolver() {
	std::cout << "Destruction complete" << "\n";
}

std::vector<char> RubiksSolver::solve(const std::vector<int>& input) {
	RubiksCube cube;
	for (int i = 0; i < 48; i++) {
	    cube.cube[i] = input[i];
	}

	Solver solver;

	auto solvingMoves = solver.solveFullCube(cube, lookup);
	auto solvingMovesChar = Move::convertVectorMoveToChar(solvingMoves);
	return solvingMovesChar;
}

PYBIND11_MODULE(RubiksSolver, m) {
	pybind11::class_<RubiksSolver>(m, "RubiksSolver")
		.def(pybind11::init<>())
		.def("solve", &RubiksSolver::solve);
}