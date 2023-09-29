#include <signal.h>
#include <time.h>
#include <pthread.h>

#include <private.h>
#include <thread_debug.h>

#define THREAD_SLEEP_TIME 250000000

void thread_worker_pause_action_handler(int sig_id)
{
    debug("thread pause action handler called");
    (void)sig_id;

    if (g_thread->status == ThreadShutdown) {
        g_thread->tcb();
    }

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
        error("failed to register sigaction handler for pausing the thread");
        //thread_on_hold_available = 0;
        return -1;
    }

    debug("registering sigaction handler for pausing the thread");

    return 0;
}

void* thread_worker(void *arg) {

    Thread *thread = arg;
    ThreadPool *tp = NULL;
    taskqueue_t *queue = NULL;

    g_thread = thread;

    thread_worker_enable_pause();

    debug("ThreadWorker init");

    debug("Waiting for queue");
    while(thread->queue == NULL) {
        nsleep(0, 100000);
        debug("queue is null");
    }

    for (;thread->queue;) {

        if (thread->tp != NULL) {
            tp = thread->tp;
            queue = thread->tp->queue;
        } else {
            queue = thread->queue;
        }
        
        thread->status = ThreadIdle;
        debug("Thread %d: Waiting for tasks\n", thread->local_id);

        while(queue->begin == NULL && queue->count == 0) {
            if(thread->queue == NULL) {
                return NULL;
            }
            nsleep(0, 1000);
        }

        // debug("queue->begin: %p", queue->begin);
        // debug("queue->count: %p", queue->count);
        thread->current_task = taskqueue_remove(queue, (Task *) queue->begin);

        thread->status = ThreadRunning;
        debug("executing task '%s'", thread->current_task->name);

        if(tp != NULL) {
            tp->n_running++;
        }
        
        task_execute(thread->current_task, thread);

        if(tp != NULL) {
            tp->n_running--;
        }
    }

    return NULL;
}