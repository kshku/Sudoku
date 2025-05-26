set(SRCS
    main.c
    game.h
    game.c
    board.h
    board.c
    options.h
    options.c
    timer.h
    timer.c
    gamestate.h
)

target_sources(sudoku PRIVATE ${SRCS})
