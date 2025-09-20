
set(SHARED_SOURCES
        RubiksLibrary/RubiksCube.cpp
        RubiksLibrary/Lookup.cpp
        RubiksLibrary/Move.cpp
        RubiksLibrary/Solver.cpp
        RubiksLibrary/InfoLogger.cpp
)

add_library(RubiksSolverLibrary ${SHARED_SOURCES})
target_include_directories(RubiksSolverLibrary PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_compile_definitions(RubiksSolverLibrary PRIVATE DATA_PATH="${CMAKE_SOURCE_DIR}/DATA")

# Build python module
add_library(RubiksSolver RubiksLibrary/RubiksSolver.cpp)
target_link_libraries(RubiksSolver PRIVATE RubiksSolverLibrary)
target_link_libraries(RubiksSolver PRIVATE pybind11::module Python::Python)

add_library(RubiksCubeWrapper RubiksLibrary/RubiksCubeWrapper.cpp)
target_link_libraries(RubiksCubeWrapper PRIVATE RubiksSolverLibrary)
target_link_libraries(RubiksCubeWrapper PRIVATE pybind11::module Python::Python)

if (UNIX)
    set_target_properties(RubiksSolver PROPERTIES SUFFIX ".so")
    set_target_properties(RubiksCubeWrapper PROPERTIES SUFFIX ".so")
elseif (WIN32)
    set_target_properties(RubiksSolver PROPERTIES SUFFIX ".pyd")
    set_target_properties(RubiksCubeWrapper PROPERTIES SUFFIX ".pyd")
endif ()
