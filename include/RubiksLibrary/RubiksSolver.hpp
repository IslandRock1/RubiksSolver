
#ifndef RUBIKSSOLVER_HPP
#define RUBIKSSOLVER_HPP

#include <vector>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "RubiksLibrary/Lookup.hpp"

class RubiksSolver {
public:
	RubiksSolver();
	~RubiksSolver();

	std::vector<char> solve(const std::vector<int> &input);

private:
	Lookup lookup;
};

PYBIND11_MODULE(RubiksSolver, m) {
	pybind11::class_<RubiksSolver>(m, "RubiksSolver")
		.def(pybind11::init<>())
		.def("solve", &RubiksSolver::solve);
}


#endif //RUBIKSSOLVER_HPP
