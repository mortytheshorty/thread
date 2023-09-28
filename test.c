#include <stdio.h>
#include <unistd.h>
#include "src/include/task.h"
#include "src/include/thread.h"
#include "src/include/threadpool.h"

void* func(void *arg) {
    RETURN_ADDRESS;
    int i = 10;
    while(i--) {
        printf("%s\n", arg);
        sleep(1);
    }

    return NULL;
}


typedef int (*test)();

int main(void)
{
    Task *task = _Task(func, "Task: 1");
    Task *task2 = _Task(func, "Task: 2");
    if (!task) {
        printf("task null\n");
        return 0;
    }

    if (!task2) {
        printf("task2 null\n");
        return 0;
    }
    ThreadPool *tp = _ThreadPool();
    printf("Test\n");
    threadpool_append(tp, task);
    threadpool_append(tp, task2);

    sleep(5);

    task_abort(task2);

    printf("Main: Waiting\n");
    sleep(25);



    task_destroy(task);
    task_destroy(task2);
    return 0;
}