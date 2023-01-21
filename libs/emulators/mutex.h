#ifndef __MUTEX_H__
#define __MUTEX_H__

//lib to get IRQs to work in mutex
#include <pthread.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

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