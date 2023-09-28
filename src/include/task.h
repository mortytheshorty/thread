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

typedef struct execution {
    uint8_t* (*function)(void*);
    _Atomic uint8_t* argument;
    _Atomic uint8_t* retval;
} execution_t;

typedef struct Task {
    execution_t exec;
    Thread *master;
    TaskStatus status;
    struct Task *prev;
    struct Task *next;
} Task;

void task_set_rcb();

Task *_Task(void* (*function)(void*), void *arg);
void task_destroy(Task *task);
void task_set(Task *task, void* (*function)(void*), void *arg);
void task_execute(Task *task, Thread *thread);
void task_abort(Task *task);
void task_sync(Task *task);
void task_async(Task *task);

#endif