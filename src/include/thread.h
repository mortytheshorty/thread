#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>
#include "taskqueue.h"

typedef void* (*RETURN_CALLBACK)();

typedef enum {
    ThreadIdle,
    ThreadRunning,
    ThreadShutdown,
    ThreadPaused,
    ThreadResumed,
} ThreadStatus;

typedef struct Thread {
    _Atomic int local_id;
    pthread_t thread_id;
    _Atomic const char *name;
    _Atomic ThreadStatus status;
    taskqueue_t *queue;
    Task *current_task;
    _Atomic RETURN_CALLBACK tcb;
} Thread;

// ThreadWorker Interface
typedef void* (*ThreadWorker)(void*);

// Default
ThreadWorker thread_worker(void* arg);

Thread* _Thread();
void thread_destroy(Thread *thread);
void thread_append(Thread *thread, Task *task);


#endif