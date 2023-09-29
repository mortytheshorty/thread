#include <stdio.h>
#include <stdlib.h>

#include <private.h>
#include <thread_debug.h>

_Atomic static int task_id = 0;

Task *_Task(task_func_t function, void *arg)
{
    Task *task = calloc(1, sizeof *task);
    if (task == NULL) {
        return NULL;
    }

    task->function = function;
    task->argument = arg;
    task->status = TaskCreated;
    task->id = task_id++;
    
    return task;
}

void task_destroy(Task *task)
{
    // thread can check if master is null, if master is null thread removes it from queue
    task->master = NULL;
    free(task);
}

void task_set(Task *task, task_func_t function, void *arg)
{
    task->function = function;
    task->argument = arg;
    task->retval = NULL;
}

void task_execute(Task *task, Thread *thread)
{
    task->master = thread;
    thread->current_task = task;
    task->status = TaskRunning;
    task->retval = task->function(task->argument);
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
   task->retval = task->function(task->argument);
}
void task_async(Task *task)
{

}