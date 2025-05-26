set(SRCS
    stext.h
    stext.c
    sbutton.h
    sbutton.c
    timer.h
    timer.c
)

target_sources(sudoku PRIVATE ${SRCS})
