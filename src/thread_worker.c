#include <signal.h>
#include <time.h>

#include "include/thread.h"
#include "include/thread_debug.h"

#define THREAD_SLEEP_TIME 250000000

void thread_worker_pause_action_handler(int sig_id)
{
    debug("thread pause action handler called");
    (void)sig_id;

    struct timespec timespec = { 0 };
    timespec.tv_nsec = THREAD_SLEEP_TIME;

    ThreadStatus old_state = g_thread->status;
    g_thread->status = ThreadPaused;
    
    while( g_thread->status != ThreadResumed ) {
        debug("still sleeping");
        nanosleep(&timespec, NULL);
    }

    g_thread->status = old_state;
}

int thread_worker_enable_pause(void)
{
    struct sigaction act_pause;
    sigemptyset(&act_pause.sa_mask);
    act_pause.sa_flags = 0;
    act_pause.sa_handler = thread_worker_pause_action_handler;

    if(sigaction(SIGUSR1, &act_pause, NULL) == -1) {
        debug_error("failed to register sigaction handler for pausing the thread");
        //thread_on_hold_available = 0;
        return -1;
    }

    debug("registering sigaction handler for puasing the thread");

    return 0;
}

ThreadWorker thread_worker(void *arg) {

    Thread *thread = arg;
    g_thread = thread;

    printf("ThreadWorker init\n");

    printf("Waiting for queue\n");
    while(thread->queue == NULL) {
        nsleep(0, 100000);
        printf("queue is null\n");
    }

    for (;;) {

        if (thread->status == ThreadShutdown) {
            break;
        }

        thread->status = ThreadIdle;
        printf("Thread %d: Waiting for tasks\n", thread->local_id);
        while(thread->queue->begin == NULL && thread->queue->count == 0) {
            nsleep(0, 1000);
        }

        thread->current_task = taskqueue_remove(thread->queue, thread->queue->begin);

        thread->status = ThreadRunning;
        task_execute(thread->current_task, thread);
    }

    return NULL;
}