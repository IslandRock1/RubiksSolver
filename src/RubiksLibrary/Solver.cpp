
#include <iostream>

#include "RubiksLibrary/Solver.hpp"
#include "RubiksLibrary/Move.hpp"

std::vector<Move> Solver::solveFullCube(RubiksCube &cube, Lookup &lookup, const int depth, const bool twoCorner) {

	std::array<short, 48> shuffleCubeCopy;
	for (int i = 0; i < 48; i++) {
		shuffleCubeCopy[i] = cube.cube[i];
	}

	std::vector<Solution> solutions;
	if (twoCorner) {
		solutions = findCrossAnd2Corners(cube, lookup, depth);
	} else {
		solutions = findCrossAnd3Corners(cube, lookup, depth);
	}

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

	std::vector<Move> out;
	int fewestMoves = 100;
	for (const auto &sol : solutions) {
		std::vector allMoves = {sol.crossMoves, sol.twoLayerMoves, sol.lastLayerMoves};
		auto combinedMoves = Move::combineMoves(allMoves);

		const int num = combinedMoves.size();
		if (num < fewestMoves) {
			fewestMoves = num;
			out = combinedMoves;
		}
	}

	return out;
}

std::vector<Move> Solver::solveFullCubeUsingUnordered(RubiksCube& cube, Lookup& lookup, int depth) {
	std::array<short, 48> shuffleCubeCopy;
	for (int i = 0; i < 48; i++) {
		shuffleCubeCopy[i] = cube.cube[i];
	}

	auto solutions = findCrossAnd2CornersUnordered(cube, lookup, depth);

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

	std::vector<Move> out;
	int fewestMoves = 100;
	for (const auto &sol : solutions) {
		std::vector allMoves = {sol.crossMoves, sol.twoLayerMoves, sol.lastLayerMoves};
		auto combinedMoves = Move::combineMoves(allMoves);

		const int num = combinedMoves.size();
		if (num < fewestMoves) {
			fewestMoves = num;
			out = combinedMoves;
		}
	}

	return out;
}


std::vector<Solution> Solver::findCrossAnd2Corners(RubiksCube &cube, Lookup &lookup, int depth) {
	if ((cube.numCornerSolved() == 2) && cube.solvedWhiteCross()) {return {};}

	std::vector<Move> moves;
	std::vector<Solution> solutions;

	SearchConditions searchConditions = {cube, lookup.crossAnd2Corners, moves, solutions, TwoCorners};
	searchMoves(searchConditions, depth);

	if (solutions.empty()) {
		// std::cout << "Had to increase depth" << "\n";
		return findCrossAnd2Corners(cube, lookup, depth + 1);
		// throw std::runtime_error("No solution found for this depth-limit and lookup combo.");
	} else {
		return solutions;
	}
}

std::vector<Solution> Solver::findCrossAnd2CornersUnordered(RubiksCube& cube, Lookup& lookup, int depth) {
	if ((cube.numCornerSolved() == 2) && cube.solvedWhiteCross()) {return {};}

	std::vector<Move> moves;
	std::vector<Solution> solutions;

	SearchConditionsUnordered searchConditions = {cube, lookup.smallerUnorderedCrossAnd2Corners, moves, solutions, TwoCorners};
	searchMovesUnordered(searchConditions, depth);

	if (solutions.empty()) {
		// std::cout << "Had to increase depth" << "\n";
		return findCrossAnd2Corners(cube, lookup, depth + 1);
		// throw std::runtime_error("No solution found for this depth-limit and lookup combo.");
	} else {
		return solutions;
	}
}


std::vector<Solution> Solver::findCrossAnd3Corners(RubiksCube &cube, Lookup &lookup, int depth) {
	if ((cube.numCornerSolved() == 3) && cube.solvedWhiteCross()) {return {};}

	std::vector<Move> moves;
	std::vector<Solution> solutions;

	SearchConditions searchConditions = {cube, lookup.crossAnd3Corners, moves, solutions, ThreeCorners};
	searchMoves(searchConditions, depth);

	if (solutions.empty()) {
		std::cout << "\n";
		std::cout << "Had to increase depth" << "\n";
		return findCrossAnd2Corners(cube, lookup, depth + 1);
		// throw std::runtime_error("No solution found for this depth-limit and lookup combo.");
	} else {
		return solutions;
	}
}

void Solver::findAndTestSolutionsFirstTwoLayers(std::array<short, 48> &shuffled, Lookup &lookup, std::vector<Solution> &solutions) {

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

void Solver::findAndTestSolutionsLastLayer(std::array<short, 48> &shuffled, Lookup &lookup, std::vector<Solution> &solutions) {

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

void Solver::searchMoves(SearchConditions &searchConditions, int depth) {
	auto &cube = searchConditions.cube;
	auto &lookup = searchConditions.lookup;

	auto lookupIterator = lookup.find(cube.getFromHash(searchConditions.hash));
	if (lookupIterator != lookup.end()) {
		auto lookupChars = lookup[cube.getFromHash(searchConditions.hash)];
		auto lookupMoves = Move::convertVectorCharToMove(lookupChars);
		auto solution = Move::combineMovesWithLookupMoves(searchConditions.moves, lookupMoves);

		Solution newSol;
		newSol.crossMoves = solution;
		searchConditions.solutions.push_back(newSol);

		// std::cout << "\n";
		// std::cout << "Found a solution!" << "\n";
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

std::vector<char> decodeMoves(uint64_t num, size_t length) {
	std::vector<char> moves(length);

	// Decode from last to first
	for (size_t i = 0; i < length; ++i) {
		moves[length - 1 - i] = 'A' + (num % 18); // remainder gives current move
		num /= 18;                               // shift right in base 18
	}

	return moves;
}

void Solver::searchMovesUnordered(SearchConditionsUnordered& searchConditions, int depth) {
	auto &cube = searchConditions.cube;
	auto &lookup = searchConditions.lookup;

	auto hash = Lookup::hashF(cube.getFromHash(searchConditions.hash));
	auto lookupIterator = lookup.find(hash);
	if (lookupIterator != lookup.end()) {
		auto lookupChars = lookup[hash];
		auto lookupMoves = Move::convertVectorCharToMove(lookupChars);
		auto solution = Move::combineMovesWithLookupMoves(searchConditions.moves, lookupMoves);

		Solution newSol;
		newSol.crossMoves = solution;
		searchConditions.solutions.push_back(newSol);

		// std::cout << "\n";
		// std::cout << "Found a solution!" << "\n";
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

		searchMovesUnordered(searchConditions, depth - 1);

		cube.turn(m.face, 4 - m.rotations);
		moves.pop_back();
	}
}
