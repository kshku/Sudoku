set(SRCS
    stext.h
    stext.c
    sbutton.h
    sbutton.c
    timer.h
    timer.c
    sthread.h
    sthread.c
)

target_sources(sudoku PRIVATE ${SRCS})
