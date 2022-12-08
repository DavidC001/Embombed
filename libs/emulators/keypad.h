#ifndef __KEYPAD_H__
#define __KEYPAD_H__

#include "../constants.h"
//lib for thread
#include <pthread.h>
#include <stdio.h>
#include <string.h>
//libe for delay
#include <unistd.h>
#include "mutex.h"

int enabledKeypad = 0;
char *puntNumPad;
char validCharsNumPad[17] = {'1', '2', '3', 'A', '4', '5', '6', 'B', '7', '8', '9', 'C', '*', '0', '#', 'D', '\0' };

void IRQ_col(void);

void disableInterruptNumPad()
{
    enabledKeypad = 0;
}

void enableInterruptNumPad()
{
    enabledKeypad = 1;
}

void evaluateKeyPad()
{
    disableInterruptNumPad();

    //take mutex
    pthread_mutex_lock(&mutex);

    //get input from keybord
    do{
        printf("Inserisci il carattere da simulare per il keypad: (valids: %s)", validCharsNumPad);
        scanf(" %c", puntNumPad);
    }while(!strchr(validCharsNumPad, *puntNumPad));
    //release mutex
    pthread_mutex_unlock(&mutex);
    
    enableInterruptNumPad();
}

void IRQ_col(void)
{
    while(1){
        if(enabledKeypad && *puntNumPad==0){
            //take mutex
            pthread_mutex_lock(&mutex);

            //ask it you want to simulate a key
            printf("vuoi simulare una pressione del keypad? (y/n) ");
            char c;
            scanf(" %c", &c);
            
            if(c == 'y'){
                *puntNumPad = 1;
            }

            //release mutex
            pthread_mutex_unlock(&mutex);

        }
        //delay 5 seconds
        sleep(5);
    }
}


void setUpKeyPad(uint_fast8_t *rowPorts, uint_fast16_t *rowPins,
                 uint_fast8_t *colPorts, uint_fast16_t *colPins,
                 char *res)
{
    puntNumPad = res;
    disableInterruptNumPad();

    //start thread with function IRQ_col
    pthread_t thread;
    pthread_create(&thread, NULL, (void *)IRQ_col, NULL);
}

#endif