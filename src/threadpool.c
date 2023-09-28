#include <private.h>
#include <stdlib.h>

ThreadPool* _ThreadPool()
{
    ThreadPool *tp = calloc(1, sizeof *tp);
    if (NULL == tp) {
        return NULL;
    }

    tp->n_threads = DEFAULT_N_THREADS;
    tp->threads = calloc(DEFAULT_N_THREADS, sizeof *tp->threads);
    if (NULL == tp->threads) {
        free(tp);
        return NULL;
    }
    
    tp->queue = taskqueue_create();
    if (NULL == tp->queue) {
        free(tp->threads);
        free(tp);
        return NULL;
    }

    printf("threadpool_create\n");

    for (size_t i = 0; i < tp->n_threads; i++) {
        printf("Creating new Thread %zu\n", i);
        tp->threads[i] = _Thread();
        tp->threads[i]->queue = tp->queue;
    }

    tp->n_threads = DEFAULT_N_THREADS;
    return tp;
}

void threadpool_extend(ThreadPool *tp, Thread *thread)
{
    tp->threads = realloc(tp->threads, (tp->n_threads + 1) * sizeof *tp->threads);
    if (NULL == tp->threads) {
        return;
    }

    thread->queue = tp->queue;
    tp->threads[tp->n_threads++] = thread;
}

void threadpool_append(ThreadPool *tp, Task *task)
{
    taskqueue_append(tp->queue, task);
}