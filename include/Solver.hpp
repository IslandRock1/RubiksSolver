//
// Created by oyste on 2/23/2025.
//

#ifndef SOLVER_HPP
#define SOLVER_HPP

#include <map>

#include "Lookup.hpp"
#include "solution.hpp"
#include "RubiksCube.hpp"

struct SearchConditions {
	RubiksCube &cube;
	std::map<std::array<unsigned int, 4>, std::vector<char>> &lookup;
	std::vector<Move> &moves;
	std::vector<Solution> &solutions;
	Hash hash;
};

class Solver {
public:
	std::vector<Move> solveFullCube(RubiksCube &cube, Lookup &lookup, int depth = 4, bool twoCorner = true);

private:
	std::vector<Solution> findCrossAnd2Corners(RubiksCube &cube, Lookup &lookup, int depth = 3);
	std::vector<Solution> findCrossAnd3Corners(RubiksCube &cube, Lookup &lookup, int depth = 3);
	void findAndTestSolutionsFirstTwoLayers(std::array<short, 48> &shuffled, Lookup &lookup, std::vector<Solution> &solutions);
	void findAndTestSolutionsLastLayer(std::array<short, 48> &shuffled, Lookup &lookup, std::vector<Solution> &solutions);
	void searchMoves(SearchConditions &searchConditions, int depth);
};



#endif //SOLVER_HPP
