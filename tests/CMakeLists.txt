
add_executable(TestSolvingSpeed SolvingSpeed.cpp)
target_link_libraries(TestSolvingSpeed PRIVATE RubiksSolverLibrary)
target_compile_definitions(TestSolvingSpeed PRIVATE DATA_PATH="${CMAKE_SOURCE_DIR}/DATA")