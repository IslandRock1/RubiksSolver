cmake_minimum_required(VERSION 3.26)
project(RubiksSolver)

set(CMAKE_CXX_STANDARD 20)

set(SOURCES
        src/main.cpp
        src/RubiksCube.cpp
        src/Lookup.cpp
        src/solution.cpp
        src/Move.cpp
        src/SerialPort.cpp
)

set(HEADERS
        include/RubiksCube.hpp
        include/Lookup.hpp
        include/Move.hpp
        include/solution.hpp
        include/SerialPort.hpp
        include/Stopwatch.hpp
)

# Create the executable
add_executable(RubiksSolver ${SOURCES} ${HEADERS})
target_include_directories(RubiksSolver PRIVATE include)