cmake_minimum_required(VERSION 3.26)
project(RubiksSolver)
set(CMAKE_CXX_STANDARD 20)

# Remember to also change Lookup::loadAllMaps()!!!!
option(LAPTOP_PATH "Use laptop path" ON)

if (LAPTOP_PATH)
    set(pybind11_DIR C:/Users/oyste/AppData/Local/Packages/PythonSoftwareFoundation.Python.3.11_qbz5n2kfra8p0/LocalCache/local-packages/Python311/site-packages/pybind11/share/cmake/pybind11)
else()
    set(pybind11_DIR C:/Users/oyste/AppData/Local/Programs/Python/Python312/Lib/site-packages/pybind11/share/cmake/pybind11)
endif()

find_package(Python REQUIRED COMPONENTS Interpreter Development)
find_package(pybind11 REQUIRED)

set(SHARED_SOURCES
        src/RubiksCube.cpp
        src/Lookup.cpp
        src/Move.cpp
        src/Solver.cpp
)

set(HEADERS
        include/RubiksCube.hpp
        include/Lookup.hpp
        include/Move.hpp
        include/solution.hpp
        include/Stopwatch.hpp
        include/Solver.hpp
)

# Build exe
add_executable(RubiksSolverExe src/main.cpp ${SHARED_SOURCES})
target_include_directories(RubiksSolverExe PRIVATE include)

# Build python module
add_library(RubiksSolver MODULE ${SHARED_SOURCES} src/RubiksSolver.cpp)  # Add pybind source file
target_include_directories(RubiksSolver PRIVATE include)
target_link_libraries(RubiksSolver PRIVATE pybind11::module Python::Python)
set_target_properties(RubiksSolver PROPERTIES SUFFIX ".pyd")
