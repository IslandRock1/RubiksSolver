
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++")
add_executable(RubiksSolverExeExe main.cpp)
target_link_libraries(RubiksSolverExeExe PRIVATE RubiksSolverLibrary)
target_compile_definitions(RubiksSolverExeExe PRIVATE DATA_PATH="${CMAKE_SOURCE_DIR}/DATA")

add_executable(CudaSolver main.cu)
target_link_libraries(CudaSolver PRIVATE RubiksSolverLibrary)