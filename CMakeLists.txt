cmake_minimum_required(VERSION 3.26)
project(RubiksSolver)

set(CMAKE_CXX_STANDARD 20)

set(SOURCES
        src/main.cpp
        src/RubiksCube.cpp
        src/Lookup.cpp
)

# Specify the header files
set(HEADERS
        include/RubiksCube.hpp
        include/Lookup.hpp
)

# Create the executable
add_executable(RubiksSolver ${SOURCES} ${HEADERS})
target_include_directories(RubiksSolver PRIVATE include)