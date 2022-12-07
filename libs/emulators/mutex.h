#ifndef __MUTEX_H__
#define __MUTEX_H__

//lib to get IRQs to work in mutex
#include <pthread.h>

typedef unsigned char uint_fast8_t;
typedef unsigned short uint_fast16_t;

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void initMutex()
{
    pthread_mutexattr_t mymutexattr;
    pthread_attr_t myattr; 

    pthread_mutexattr_init(&mymutexattr);
    pthread_mutex_init(&mutex, &mymutexattr);
    pthread_mutexattr_destroy(&mymutexattr);
}

#endif