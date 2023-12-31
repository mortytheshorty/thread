#include <private.h>
#include <stdlib.h>

#include <thread_debug.h>

ThreadPool* _ThreadPool()
{
    ThreadPool *tp = calloc(1, sizeof *tp);
    if (NULL == tp) {
        return NULL;
    }

    tp->n_threads = DEFAULT_N_THREADS;
    tp->threads = calloc(DEFAULT_N_THREADS, sizeof *tp->threads);
    if (NULL == tp->threads) {
        error("out of memory");
        free(tp);
        return NULL;
    }
    
    tp->queue = taskqueue_create();
    if (NULL == tp->queue) {
        free(tp->threads);
        free(tp);
        return NULL;
    }

    debug("threadpool_create\n");

    for (size_t i = 0; i < tp->n_threads; i++) {
        debug("Creating new Thread %zu\n", i);
        tp->threads[i] = _Thread();
        tp->threads[i]->tp = tp;
        tp->threads[i]->queue = tp->queue;
    }

    tp->n_threads = DEFAULT_N_THREADS;
    return tp;
}

void threadpool_destroy(ThreadPool *tp)
{
    for (size_t i = 0; i < tp->n_threads; i++) {
        tp->threads[i]->queue = NULL;
        pthread_join(tp->threads[i]->thread_id, NULL);
        debug("waiting on thread to finish");
        free(tp->threads[i]);
    }

    taskqueue_destroy(tp->queue);
    free(tp->threads);
    free(tp);
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