#ifndef __CABLES_H__
#define __CABLES_H__

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "../constants.h"
#include "mutex.h"

uint8_t* connectedCables;
uint8_t enableCables = 0;

void IRQ_cables(void){
    while(1){
        if(enableCables){
            //take mutex
            pthread_mutex_lock(&mutex);

            //ask it you want to simulate a key
            printf("vuoi simulare un cavo disconnesso (0-no alt 1-4)? ");
            int c;
            scanf(" %d", &c);
            if(c>0 && c<=4){
                connectedCables[c-1] = 0;
            }


            //release mutex
            pthread_mutex_unlock(&mutex);

        }
        //delay 5 seconds
        sleep(5);
    }
}

/*
 * enable interrupt on pin
 * register IRQ on port
 */
void enableInterruptCables(){
    enableCables = 1;
}

/*
 * remove IRQ handler on port and disable interrupt on pin
 */
void disableInterruptCables(){
    enableCables = 0;
}

/*
 * all vectors must be of size NUMCAVI
 * defined inside constants.h
 * pins and ports can be deleted after calling this function
 * conn vector will be used to communicate with the caller
 * conn will be initialized with ones
 *
 * by default interrupts are disabled
 * will overwrite port interrupt!!!
 *
 * ordine cavi: B Y G R
 */
void setupCables(uint_fast16_t* pins, uint_fast8_t* ports,uint8_t* conn){
    connectedCables = conn;
    disableInterruptCables();

    int i;
    for(i=0;i<NUMCAVI;i++){
        connectedCables[i] = 1;
    }

    pthread_t cablesThread;
    pthread_create(&cablesThread, NULL, (void*)IRQ_cables, NULL);
}

#endif
