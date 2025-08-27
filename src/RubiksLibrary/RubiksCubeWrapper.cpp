
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <array>

#include "RubiksLibrary/RubiksCube.hpp"

class RubiksCubeWrapper {
public:
	void move(char m);
	std::array<short, 48> getCube() const;

private:
	RubiksCube cube;
};

void RubiksCubeWrapper::move(char m) {
	cube.turn(m);
}

std::array<short, 48> RubiksCubeWrapper::getCube() const {
	return cube.cube;
}

PYBIND11_MODULE(RubiksCubeWrapper, m) {
	pybind11::class_<RubiksCubeWrapper>(m, "RubiksCubeWrapper")
		.def(pybind11::init<>())
		.def("move", &RubiksCubeWrapper::move)
		.def("getCube", &RubiksCubeWrapper::getCube);
}