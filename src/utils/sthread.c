#include "sthread.h"

#if defined(SLINUX)

void *thread_wrapper(void *data) {
    SThread *thread = (SThread *)data;

    return thread->func(thread->data, thread);
}

// Create a new thread.
bool sthread_create(SThread *thread, sthread_func func, void *data) {
    thread->func = func;
    thread->data = data;
    return !pthread_create(&thread->thread, NULL, thread_wrapper, thread);
}

// Wait till the given thread is completed.
bool sthread_join(SThread *thread, void **ret) {
    return !pthread_join(thread->thread, ret);
}

// Exit from the calling thread.
void sthread_exit(SThread *thread, void *ret) {
    (void)(thread);
    pthread_exit(ret);
}
#else

static DWORD WINAPI thread_wrapper(LPVOID data) {
    SThread *thread = (SThread *)data;

    thread->ret = thread->func(thread->data, thread);

    return 0;
}

// Create a new thread.
bool sthread_create(SThread *thread, sthread_func func, void *data) {
    thread->data = data;
    thread->func = func;
    thread->ret = NULL;

    thread->handle = CreateThread(NULL, 0, thread_wrapper, thread, 0, NULL);

    return !thread->handle ? false : true;
}

// Wait till the given thread is completed.
bool sthread_join(SThread *thread, void **ret) {
    if (ret) *ret = NULL;

    if (!WaitForSingleObject(thread->handle, INFINITE)) return false;

    if (ret) *ret = thread->ret;

    if (!CloseHandle(thread->handle)) return false;

    return true;
}

// Exit from the calling thread.
void sthread_exit(SThread *thread, void *ret) {
    thread->ret = ret;
    ExitThread(0);
}

#endif
