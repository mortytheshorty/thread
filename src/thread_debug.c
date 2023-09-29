// (c) 2023 Florian Giest
// This code is licensed under MIT license (see LICENSE.txt for details)

/*********************************************************************************************************/
/*                                              DEPENDANICIES                                            */
/*********************************************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <pthread.h>

#include <thread.h>

#define DEBUG

/*********************************************************************************************************/
/*                                               DEFINITIONS                                             */
/*********************************************************************************************************/
#ifdef DEBUG
pthread_mutex_t debug_lock = PTHREAD_MUTEX_INITIALIZER;

extern __thread FILE *g_thread_log;
extern __thread Thread *g_thread;

extern const char* thread_strerror(int err);

/*********************************************************************************************************/
/*                                           FUNCTION DEFINITIONS                                        */
/*********************************************************************************************************/

/*  void xfree(void *ptr, const char *file, int line)
 {
   printf("call of free in file %s:%d\n", file, line);
   free(ptr);
 } */

void xdebug(pthread_t threadid, const char *function, const char *fmt, ...)
{
    FILE *fp = g_thread_log == NULL ? stdout : g_thread_log;
    char name[16] = { 0 };
    pthread_getname_np(threadid, name, sizeof name);

    pthread_mutex_lock(&debug_lock);
    va_list args;
    va_start(args, fmt);

    fprintf(fp, "DEBUG: %s[%ld] %32s(): ", name, g_thread == NULL ? 0UL : g_thread->local_id, function);
    vfprintf(fp, fmt, args);
    fprintf(fp, "\n");
    fflush(fp);
    
    va_end(args);
    pthread_mutex_unlock(&debug_lock);
}


void xerror(pthread_t threadid, const char *function, const char *filename, int line, const char *fmt, ...)
{
    FILE *fp = g_thread_log == NULL ? stderr : g_thread_log;
    char name[16] = { 0 };
    pthread_getname_np(threadid, name, sizeof name);

    pthread_mutex_lock(&debug_lock);
    int err = errno;
    va_list args;
    va_start(args, fmt);

    fprintf(fp, "ERROR: %s[%ld] %s(): ", name, g_thread == NULL ? 0UL : g_thread->local_id, function);
    vfprintf(fp, fmt, args);
    if(err) {
        fprintf(fp, " (%s)", strerror(errno));
        errno = 0;
    }
    fprintf(fp, " in file %s at line %d\n", filename, line);
    fflush(fp);

    va_end(args);
    pthread_mutex_unlock(&debug_lock);
}
#endif