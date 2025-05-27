#pragma once

#include <stdbool.h>

#if defined(__gnu_linux__) || defined(__linux__)
    #define SLINUX
#elif defined(_WIN32)
    #define SWINDOWS
#else
    #error "Unsupported platform"
#endif

#ifdef __STDC_NO_ATOMICS__
    #error "No automic support"
#endif

#if defined(SLINUX)
    #include <pthread.h>
#elif defined(SWINDOWS)
    #include <Windows.h>
#endif

typedef struct SThread SThread;

typedef void *(*sthread_func)(void *data, SThread *thread);

struct SThread {
#if defined(SWINDOWS)
        HANDLE handle;
        void *ret;
#else
        pthread_t thread;
#endif
        sthread_func func;
        void *data;
};

bool sthread_create(SThread *thread, sthread_func func, void *data);

bool sthread_join(SThread *thread, void **ret);

void sthread_exit(SThread *thread, void *ret);
