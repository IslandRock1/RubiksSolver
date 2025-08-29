cmake_minimum_required(VERSION 3.26)
project(RubiksSolver LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

include(CheckLanguage)
check_language(CUDA)
if (CMAKE_CUDA_COMPILER)
    MESSAGE("Cuda activated")
    enable_language(CUDA)
else ()
    MESSAGE("Cuda missing")
endif ()

if (UNIX)
    set(pybind11_DIR ${CMAKE_SOURCE_DIR}/.venv/lib/python3.12/site-packages/pybind11/share/cmake/pybind11)
elseif (WIN32)
    set(pybind11_DIR ${CMAKE_SOURCE_DIR}/.venv/Lib/site-packages/pybind11/share/cmake/pybind11)
endif()

find_package(Python REQUIRED COMPONENTS Interpreter Development)
find_package(pybind11 REQUIRED)

add_subdirectory(examples)
add_subdirectory(src)