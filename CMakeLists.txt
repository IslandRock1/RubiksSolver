cmake_minimum_required(VERSION 3.26)
project(RubiksSolver)

set(CMAKE_CXX_STANDARD 20)

# Set the paths to the ixwebsocket headers and library
set(IXWEBSOCKET_INCLUDE_DIR "C:/Users/oyste/CLionProjects/NTNU/RubiksSolver/IXWebSocket/ixwebsocket")
set(IXWEBSOCKET_LIB_DIR "C:/Users/oyste/CLionProjects/NTNU/RubiksSolver/IXWebSocket/build/Release")

# Add the include directory
include_directories(${IXWEBSOCKET_INCLUDE_DIR})

# Add the library directory
link_directories(${IXWEBSOCKET_LIB_DIR})

set(SOURCES
        src/main.cpp
        src/RubiksCube.cpp
        src/Lookup.cpp
        src/solution.cpp
        src/Move.cpp
        src/SerialPortOld.cpp
        src/SerialPort.cpp
)

set(HEADERS
        include/RubiksCube.hpp
        include/Lookup.hpp
        include/Move.hpp
        include/solution.hpp
        include/SerialPortOld.hpp
        include/Stopwatch.hpp
        include/SerialPort.hpp
)

# Create the executable
add_executable(RubiksSolver ${SOURCES} ${HEADERS})
target_include_directories(RubiksSolver PRIVATE include)
target_link_libraries(RubiksSolver PRIVATE ixwebsocket)