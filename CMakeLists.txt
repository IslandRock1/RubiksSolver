cmake_minimum_required(VERSION 3.26)
project(RubiksSolver LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

# Remember to also change Lookup::loadAllMaps()!!!!
option(LAPTOP_PATH "Use laptop path" OFF)

if (LAPTOP_PATH)
    set(pybind11_DIR C:/Users/oyste/AppData/Local/Packages/PythonSoftwareFoundation.Python.3.11_qbz5n2kfra8p0/LocalCache/local-packages/Python311/site-packages/pybind11/share/cmake/pybind11)
else()
    set(pybind11_DIR C:/Users/oyste/AppData/Local/Programs/Python/Python312/Lib/site-packages/pybind11/share/cmake/pybind11)
endif()

set(pybind11_DIR ${CMAKE_SOURCE_DIR}/.venv/lib/python3.12/site-packages/pybind11/share/cmake/pybind11)

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
add_executable(RubiksSolverExeExe src/main.cpp ${SHARED_SOURCES})
target_include_directories(RubiksSolverExeExe PRIVATE include)

add_executable(CudaSolver src/main.cu ${SHARED_SOURCES})
target_include_directories(CudaSolver PRIVATE include)

# Build python module
add_library(RubiksSolver MODULE ${SHARED_SOURCES} src/RubiksSolver.cpp)  # Add pybind source file
target_include_directories(RubiksSolver PRIVATE include)
target_link_libraries(RubiksSolver PRIVATE pybind11::module Python::Python)
set_target_properties(RubiksSolver PROPERTIES SUFFIX ".so")

add_library(RubiksCubeWrapper MODULE ${SHARED_SOURCES} src/RubiksCubeWrapper.cpp)  # Add pybind source file
target_include_directories(RubiksCubeWrapper PRIVATE include)
target_link_libraries(RubiksCubeWrapper PRIVATE pybind11::module Python::Python)
set_target_properties(RubiksCubeWrapper PROPERTIES SUFFIX ".so")

target_compile_definitions(RubiksSolver PRIVATE DATA_PATH="${CMAKE_SOURCE_DIR}/data")