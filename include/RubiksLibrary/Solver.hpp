
#ifndef SOLVER_HPP
#define SOLVER_HPP

#include <map>

#include "RubiksLibrary/Move.hpp"
#include "RubiksLibrary/Lookup.hpp"
#include "RubiksLibrary/solution.hpp"
#include "RubiksLibrary/RubiksCube.hpp"

struct SearchConditions {
	RubiksCube &cube;
	std::map<std::array<unsigned int, 4>, std::vector<char>> &lookup;
	std::vector<Move> &moves;
	std::vector<Solution> &solutions;
	Hash hash;
};

struct SearchConditionsNewHash {
	RubiksCube &cube;
	std::unordered_map<__int128, std::vector<char>> &lookup;
	std::vector<Move> &moves;
	std::vector<Solution> &solutions;
	Hash hash;
};

struct SearchConditionsUnordered {
	RubiksCube &cube;
	std::unordered_map<uint64_t, std::vector<char>> &lookup;
	std::vector<Move> &moves;
	std::vector<Solution> &solutions;
	Hash hash;
};

class Solver {
public:
	// TODO: refactor most of solving code
	std::vector<Move> solveFullCube(RubiksCube &cube, Lookup &lookup, int depth = 4, bool twoCorner = true);
	std::vector<Move> solveFullCubeUsingUnordered(RubiksCube &cube, Lookup &lookup, int depth = 4);

	std::vector<Move> solveUpTo3Corners(RubiksCube &cube, Lookup &lookup, int depth = 4);
	static std::vector<Move> solveUpTo2CornersUsingNewHash(RubiksCube &cube, Lookup &lookup, int depth = 4);
	static std::vector<Solution> findCrossAnd2CornersUsingNewHash(RubiksCube &cube, Lookup &lookup, int depth = 3);
	static void searchMovesNewHash(SearchConditionsNewHash &searchConditions, int depth);

private:
	std::vector<Solution> findCrossAnd2Corners(RubiksCube &cube, Lookup &lookup, int depth = 3);
	std::vector<Solution> findCrossAnd2CornersUnordered(RubiksCube &cube, Lookup &lookup, int depth = 3);
	std::vector<Solution> findCrossAnd3Corners(RubiksCube &cube, Lookup &lookup, int depth = 3);
	void findAndTestSolutionsFirstTwoLayers(std::array<short, 48> &shuffled, Lookup &lookup, std::vector<Solution> &solutions);
	void findAndTestSolutionsLastLayer(std::array<short, 48> &shuffled, Lookup &lookup, std::vector<Solution> &solutions);
	void searchMoves(SearchConditions &searchConditions, int depth);
	void searchMovesUnordered(SearchConditionsUnordered &searchConditions, int depth);
};


#endif //SOLVER_HPP
