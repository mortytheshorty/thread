#ifndef TASKQUEUE_H
#define TASKQUEUE_H

#include <stddef.h>
#include "task.h"

typedef struct taskqueue {
    _Atomic Task *begin;
    _Atomic Task *end;
    _Atomic size_t count;
} taskqueue_t;

taskqueue_t* taskqueue_create();
void taskqueue_destroy(taskqueue_t *queue);
void taskqueue_append(taskqueue_t *queue, Task *task);
Task* taskqueue_remove(taskqueue_t *queue, Task *task);
void taskqueue_clear(taskqueue_t *queue);

#endif