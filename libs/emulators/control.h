#ifndef __CONTROL_H__
#define __CONTROL_H__

#include "LCD.h"
#include <string.h>
#include "mutex.h"
#include "timer_subscription.h"

volatile uint8_t numMistekesControl;
int* timeControl;
int enableControl=0;
volatile uint8_t timeStepperControl;

/*
 * aggiunge un errore velocizzando il timer
 */
void addMistakeControl(){
    
    numMistekesControl++;
    if(numMistekesControl==3){
        enableControl=0;
        *timeControl = 0;
    }

    pthread_mutex_lock(&mutex);
    printf("aggiunto un errore, numero totale: %d", numMistekesControl);
    pthread_mutex_unlock(&mutex);
}


void timeUpdateControl(){
        if(enableControl && *timeControl>0){
            timeStepperControl++;
            if(timeStepperControl==4-numMistekesControl){
                timeStepperControl=0;
                *timeControl = *timeControl - 1;
                int minutes = *timeControl / 60;
                int seconds = *timeControl % 60;
                char timeS[5];
                timeS[0] = minutes / 10 + '0';
                timeS[1] = minutes % 10 + '0';
                timeS[2] = ':';
                timeS[3] = seconds / 10 + '0';
                timeS[4] = seconds % 10 + '0';
                displayLCD(ROW_2, timeS, 5);
            }
        }
}

/*
 * inizia il countdown
 */
void startTimerControl(){
    enableControl=1;
}

/*
 * inizializza l'LCD (essendo l'unico modulo a contatto)
 * inizializza il timer e lo mette in pausa
 */
void setupControl(int* time, char* SN, lcdConfig* LCD, uint8_t* ledPorts, uint16_t* ledPins){
    timeControl = time;
    numMistekesControl = 0;
    timeStepperControl = 0;

    /*
     * inizializzo LCD
     */
    setupLCD(*LCD);

    //send SN to LCD
    displayLCD(ROW_1, SN, strlen(SN));
    //send time to LCD
    int minutes = *timeControl / 60;
    int seconds = *timeControl % 60;
    char timeS[5];
    timeS[0] = minutes / 10 + '0';
    timeS[1] = minutes % 10 + '0';
    timeS[2] = ':';
    timeS[3] = seconds / 10 + '0';
    timeS[4] = seconds % 10 + '0';
    displayLCD(ROW_2, timeS, 5);

    /*
     * inizializzo timer
     */
    enableControl=0;
    registerTimer(timeUpdateControl);
}

#endif
