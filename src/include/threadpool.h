#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <stdio.h>
#include "thread.h"
#include "taskqueue.h"


#define DEFAULT_N_THREADS 10

typedef struct ThreadPool {
    Thread **threads;
    taskqueue_t *queue;
    size_t n_threads;
    size_t n_running;
} ThreadPool;

ThreadPool* _ThreadPool();
void threadpool_extend(ThreadPool *tp, Thread *thread);
void threadpool_append(ThreadPool *tp, Task *task);

#endif