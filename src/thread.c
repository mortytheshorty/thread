#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <unistd.h>

#include <private.h>
#include <signal.h>

_Atomic static int thread_id = 1;
__thread Thread *g_thread = NULL;
__thread FILE *g_thread_log = NULL;

void* thread_worker(void *arg);

void nsleep(size_t seconds, size_t milli)
{
    struct timespec t = { 0 };
    t.tv_nsec = milli * 1000;
    t.tv_sec = seconds;
    nanosleep(&t, NULL);
}

Thread* ThreadDetached()
{
    Thread *thread = _Thread();
    thread->queue = taskqueue_create();
    if (thread->queue == NULL) {
        thread_destroy(thread);
        return NULL;
    }
    return thread;
}

Thread* _Thread()
{
    Thread *thread =  calloc(1, sizeof *thread);

    thread->local_id = thread_id++;
    thread->status = ThreadIdle;
    
    pthread_create(&thread->thread_id, NULL, thread_worker, thread);

    return thread;
}

void thread_destroy(Thread *thread)
{
    thread->status = ThreadShutdown;
    thread->tcb();
}

void thread_append(Thread *thread, Task *task)
{
    taskqueue_append(thread->queue, task);
}

void thread_pause(Thread *thread)
{
    if (thread)
        pthread_kill(thread->thread_id, SIGUSR1);
}

void thread_resume(Thread *thread)
{
    if (thread)
        thread->status = ThreadResumed;
}