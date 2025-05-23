set(SRCS
    main.c
    game.h
    game.c
    board.h
    board.c
)

target_sources(sudoku PRIVATE ${SRCS})
