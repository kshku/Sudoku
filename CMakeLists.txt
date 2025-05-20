cmake_minimum_required(VERSION 3.15...3.31)

project(Sudoku VERSION 0.1.0 DESCRIPTION "A simple sudoku game")

# Find raylib
find_package(raylib REQUIRED)

add_executable(sudoku)

target_link_libraries(sudoku PRIVATE raylib)

add_subdirectory(src)

add_custom_target(copy_res ALL COMMENT "Copying resources")
add_custom_command(
    TARGET copy_res
    POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_SOURCE_DIR}/res" "${CMAKE_BINARY_DIR}/res"
    USES_TERMINAL
)
