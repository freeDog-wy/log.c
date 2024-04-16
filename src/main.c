#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "log.h"

#define THREAD_NUM 12

pthread_mutex_t log_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_t thread_handles[THREAD_NUM];

void log_lock_func(bool lock, void *log_lock)
{
    if(lock) {
        pthread_mutex_lock(log_lock);
    }
    else {
        pthread_mutex_unlock(log_lock);
    }
}

void * log_func(void * args) {
    for(int i = 0; i < 4; ++i)
    {
        log_info("%ld", (long)args);
        log_trace("%ld", (long)args);
        log_fatal("%ld", (long)args);
        log_debug("%ld", (long)args);
    }

    return NULL;
}

int main() {

    FILE *fp = fopen("log.txt", "w");
    long thread_index;

    log_add_fp(fp, LOG_TRACE);
    log_set_lock(log_lock_func, (void *)&log_lock);
    log_set_quiet(false);

    for(thread_index = 0; thread_index < THREAD_NUM; thread_index++) {
        pthread_create(&thread_handles[thread_index], NULL, log_func, (void *)thread_index);
    }

    for(thread_index = 0; thread_index < THREAD_NUM; thread_index++) {
        pthread_join(thread_handles[thread_index], NULL);
    }
    return 0;
}