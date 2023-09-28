#include <stdio.h>
#include <unistd.h>
#include "src/include/task.h"
#include "src/include/thread.h"
#include "src/include/threadpool.h"
#include "src/include/thread_debug.h"

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
        error("task null\n");
        return 0;
    }

    if (!task2) {
        error("task2 null\n");
        return 0;
    }
    ThreadPool *tp = _ThreadPool();

    threadpool_append(tp, task);
    threadpool_append(tp, task2);

    sleep(5);

    //task_abort(task2);
    task_pause(task);

    printf("Main: Waiting\n");
    sleep(2);

    task_resume(task);

    printf("ThreadPool->running: %d\n", tp->n_running);
    printf("ThreadPool->idle: %d\n", tp->n_threads - tp->n_running);

    sleep(10);



    task_destroy(task);
    task_destroy(task2);
    return 0;
}