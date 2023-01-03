#ifndef __SONDA_H__
#define __SONDA_H__

/* DriverLib Includes */
#include "../constants.h"
#include "mutex.h"
#include <stdio.h>

uint8_t* puntSonda;
int enableSonda = 0;

void ADCSondaIRQ(void){
    while(1){
        if(enableSonda && *puntSonda==0){
            pthread_mutex_lock(&mutex);
            printf("vuoi simulare la sonda? (y/n) ");
            char c;
            scanf(" %c", &c);
            if(c=='y'){
                do{
                    printf("quale input? (1-9) ");
                    scanf(" %c", &c);
                }while(c<'1' || c>'9');
                *puntSonda = c-'0';
            }
            pthread_mutex_unlock(&mutex);
        }
        sleep(5);
    }
}

void disableInterruptSonda(){
    enableSonda = 0;
}

void enableInterruptSonda(){
    enableSonda = 1;
}

void setupSonda(uint16_t pin, uint8_t port, uint32_t ADCInputMap, uint32_t ADCMemory, uint8_t* punt){
    puntSonda = punt;

    //start thread
    pthread_t thread;
    pthread_create(&thread, NULL, (void*) &ADCSondaIRQ, NULL);
}

#endif
