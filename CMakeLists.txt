cmake_minimum_required(VERSION 3.26)
project(RubiksSolver)

set(CMAKE_CXX_STANDARD 20)

#target_include_directories(include)
#add_executable(RubiksSolver src/main.cpp)

set(SOURCES
        src/main.cpp
)

# Specify the header files
set(HEADERS
        include/RubiksCube.hpp
        # Add other header files as needed
)

# Create the executable
add_executable(RubiksSolver ${SOURCES} ${HEADERS})
target_include_directories(RubiksSolver PRIVATE include)