// (c) 2023 Florian Giest
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef _DEBUG_H    // BEGIN INCLUDE GUARD
#define _DEBUG_H

/*********************************************************************************************************/
/*                                              DEPENDANICIES                                            */
/*********************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <pthread.h>

#define DEBUG

/*********************************************************************************************************/
/*                                               DEFINITIONS                                             */
/*********************************************************************************************************/

/* if DEBUG is defines it enables the macro for logging the steps 
   of every function which calls these functions/macros */
#ifdef DEBUG
 /* pretty simple debug function expaned by macro 'debug()'
    write DEBUG: at the first place followed by the calling pthread id
    and a formatstring and its arguments
 */
 void xdebug(pthread_t threadid, const char *function, const char *fmt, ...);
 #define debug(fmt, ...) xdebug(pthread_self(), __func__, fmt, ##__VA_ARGS__)
 /* same as for 'xdebug()' but for error and also prints errno string if
    errno is set.
 */
 void xerror(pthread_t threadid, const char *function, const char *filename, int line, const char *fmt, ...);
 #define error(fmt, ...) xerror(pthread_self(), __func__, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

/*  void xfree(void *ptr, const char *file, int line);
 #define free(x) xfree((x), __FILE__, __LINE__)
 */
#else
 #define debug_error(x, ...)
 #define debug(x, ...)

#endif

/*********************************************************************************************************/
/*                                           FUNCTION DEFINITIONS                                        */
/*********************************************************************************************************/


#endif