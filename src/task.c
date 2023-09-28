#include <stdio.h>
#include <stdlib.h>

#include <private.h>
#include <thread_debug.h>

static int task_id = 0;

Task *_Task(void* (*function)(void*), void *arg)
{
    Task *task = calloc(1, sizeof *task);
    if (task == NULL) {
        return NULL;
    }
    task->exec.function = function;
    task->exec.argument = arg;
    task->status = TaskCreated;

    return task;
}

void task_destroy(Task *task)
{
    // thread can check if master is null, if master is null thread removes it from queue
    task->master = NULL;
    free(task);
}

void task_set(Task *task, void* (*function)(void*), void *arg)
{
    task->exec.function = (_Atomic uint8_t *) function;
    task->exec.argument = (_Atomic uint8_t *) arg;
    task->exec.retval = (_Atomic uint8_t *) NULL;
}

void task_execute(Task *task, Thread *thread)
{
    task->master = thread;
    thread->current_task = task;
    task->status = TaskRunning;
    task->exec.retval = task->exec.function(task->exec.argument);
}

void task_abort(Task *task)
{
    if (task->master != NULL) {
        debug("master: %p\n", task->master);
        if (task->master->tcb != NULL) {
            debug("callback: %p\n", task->master->tcb);
            task->master->tcb();
            task->status = TaskStopped;
            printf("aborting\n");
        }
    }
}

void task_pause(Task *task)
{
    thread_pause(task->master);
}

void task_resume(Task *task)
{
    thread_resume(task->master);
}

void task_sync(Task *task)
{
   task->exec.retval = task->exec.function(task->exec.argument);
}
void task_async(Task *task)
{

}