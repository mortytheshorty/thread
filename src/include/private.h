#ifndef THREAD_PRIVATE
#define THREAD_PRIVATE

#include <task.h>
#include <taskqueue.h>
#include <thread.h>
#include <threadpool.h>

void nsleep(size_t seconds, size_t milli);

#endif