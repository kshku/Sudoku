set(SRCS
    main.c
    game.h
    game.c
    global.h
    main_menu.h
    main_menu.c
    board.h
    board.c
    loading.h
    loading.c
    end.h
    end.c
)

target_include_directories(sudoku PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

target_sources(sudoku PRIVATE ${SRCS})

add_subdirectory(utils)
