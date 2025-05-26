cmake_minimum_required(VERSION 3.15...3.31)

project(Sudoku VERSION 0.1.0 DESCRIPTION "A simple sudoku game")

# Find raylib
find_package(raylib REQUIRED)

add_executable(sudoku)

target_compile_definitions(sudoku PRIVATE APP_NAME="${CMAKE_PROJECT_NAME}" VERSION_MAJOR=${CMAKE_PROJECT_VERSION_MAJOR} VERSION_MINOR=${CMAKE_PROJECT_VERSION_MINOR} VERSION_PATCH=${CMAKE_PROJECT_VERSION_PATCH})

target_link_libraries(sudoku PRIVATE raylib)

set(gcc_clang "$<COMPILE_LANG_AND_ID:C,Clang,GNU>")
set(msvc_comp "$<COMPILE_LANG_AND_ID:C,MSVC>")

target_compile_options(sudoku PRIVATE "$<${gcc_clang}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wformat;-Wuninitialized;-Wpedantic>>")

add_subdirectory(src)

add_custom_target(copy_res ALL COMMENT "Copying resources")
add_custom_command(
    TARGET copy_res
    POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_SOURCE_DIR}/res" "${CMAKE_BINARY_DIR}/res"
    USES_TERMINAL
)
