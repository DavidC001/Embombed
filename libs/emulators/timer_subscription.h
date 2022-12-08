#ifndef __TIMERSUB_H__
#define __TIMERSUB_H__

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "../constants.h"
#include <pthread.h>

void(*TimerIntHandlers[MAX_FUNC])(void);
uint8_t TimerEnabled[MAX_FUNC];
int TimerFuncIndex;

void disableTimer(int index){
    TimerEnabled[index] = 0;
}

void enableTimer(int index){
    TimerEnabled[index] = 1;
}

/*
 * register interrupt
 * ritorna l'indice dove viene salvato
 * non gestisce la mappatura della memoria che va fatta esternamente
 */
int registerTimer(void (*intHandler)(void)){

    TimerIntHandlers[TimerFuncIndex] = intHandler;
    TimerEnabled[TimerFuncIndex] = 1;
    TimerFuncIndex++;

    return (TimerFuncIndex-1);
}



void TA0_0_IRQHandler(void)
{
    while(1){
        int i;
        for(i=0;i<TimerFuncIndex;i++){
            if(TimerEnabled[i]){
                (*TimerIntHandlers[i])();
            }
        }

        //sleep for 250 ms
        usleep(250000);
    }
}

void setupTimer(){
    TimerFuncIndex = 0;

    pthread_t timerThread;
    pthread_create(&timerThread, NULL, (void*)TA0_0_IRQHandler, NULL);
}

#endif
