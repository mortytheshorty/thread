#ifndef TASK_H
#define TASK_H

#include <inttypes.h>
//#include <thread.h>


typedef struct Thread Thread;

extern __thread Thread *g_thread;

#define RETURN_ADDRESS g_thread->tcb = __builtin_extract_return_addr(__builtin_return_address (0));


typedef enum {
    TaskCreated,
    TaskRunning,
    TaskStopped,
    TaskCancled,
} TaskStatus;

typedef void* (*task_func_t)(void*);
typedef void* argument_t;
typedef void* result_t;

typedef struct Task {
    _Atomic task_func_t function;
    _Atomic argument_t argument;
    _Atomic result_t retval;
    Thread *master;
    _Atomic int id;
    _Atomic const char *name;
    TaskStatus status;
    struct Task *prev;
    struct Task *next;
} Task;

void task_set_rcb();

Task *_Task(void* (*function)(void*), void *arg);
void task_destroy(Task *task);
void task_set(Task *task, void* (*function)(void*), void *arg);
void task_name(Task *task, const char *name);
void task_execute(Task *task, Thread *thread);
void task_abort(Task *task);
void task_pause(Task *task);
void task_resume(Task *task);
void task_sync(Task *task);
void task_async(Task *task);

#endif