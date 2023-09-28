#include <stdio.h>
#include <stdlib.h>

#include <private.h>

taskqueue_t* taskqueue_create()
{
    taskqueue_t *queue = calloc(1, sizeof *queue);
    if (NULL == queue) {
        return NULL;
    }

    return queue;
}

void taskqueue_destroy(taskqueue_t *queue)
{   
    taskqueue_clear(queue);
    free(queue);
}

void insertAfter(taskqueue_t *queue, Task *fromTask, Task *newTask)
{
    newTask->prev = fromTask;
    if (fromTask->next == NULL) {
        newTask->next = NULL;
        queue->end = newTask;
    }
    else {
        newTask->next = fromTask->next;
        fromTask->next->prev = newTask;
    }

    fromTask->next = newTask;
}

void insertBefore(taskqueue_t *queue, Task *fromTask, Task *newTask)
{
    newTask->next = fromTask;
    if (fromTask->prev == NULL) {
        newTask->prev = NULL;
        queue->begin = newTask;
    }
    else {
        newTask->prev = fromTask->prev;
        fromTask->prev->next = newTask;
    }

    fromTask->prev = newTask;
}

void taskqueue_append(taskqueue_t *queue, Task *task)
{
    if (queue->begin == NULL) {
        queue->begin = task;
        queue->end = task;
        task->prev = NULL;
        task->next = NULL;
    }
    else {
        insertBefore(queue, queue->begin, task);
    }

    queue->count++;
}

void taskqueue_append_last(taskqueue_t *queue, Task *task)
{
    if (queue->end == NULL) {
        taskqueue_append(queue, task);
    }
    else {
        insertAfter(queue, queue->end, task);
    }

    queue->count++;
}

Task* taskqueue_remove(taskqueue_t *queue, Task *task)
{
    if (task->prev == NULL) {
        queue->begin = task->next;
    }
    else {
        task->prev->next = task->next;
    }

    if (task->next == NULL) {
        queue->end = task->prev;
    }
    else {
        task->next->prev = task->prev;
    }

    queue->count--;
    return task;
}

void taskqueue_clear(taskqueue_t *queue)
{
    queue->begin = queue->end = NULL;
    queue->count = 0;
}
