#ifndef __CONTROL_H__
#define __CONTROL_H__

#include <LCD.h>
#include <string.h>
#include <timer_subscription.h>

volatile uint8_t numMistekesControl;
int* timeControl;
int timerIndexControl;
volatile uint8_t timeStepperControl;
uint8_t errLedPortsControl[3] = {GPIO_PORT_P3,GPIO_PORT_P4,GPIO_PORT_P5};
uint16_t errLedPinsControl[3] = {GPIO_PIN0,GPIO_PIN0,GPIO_PIN2};

/*
 * aggiunge un errore velocizzando il timer
 */
void addMistakeControl(){
    GPIO_setOutputHighOnPin(errLedPortsControl[numMistekesControl], errLedPinsControl[numMistekesControl]);

    numMistekesControl++;
    if(numMistekesControl==3){
        disableTimer(timerIndexControl);
        *timeControl = 0;
    }

    //in futuro possiamo far vibrare il buzzer qui
    printf("MISTAKE CONTROL");
}

void correctFeedBackControl(){
    //manca
    printf("CORRECT CONTROL\n");
}

void victoryFeedBackControl(){
    //manca
    printf("VICTORY CONTROL\n");
}

void timeUpdateControl(){
    timeStepperControl++;

    if(timeStepperControl>=4-numMistekesControl){
        (*timeControl)--;
        timeStepperControl = 0;
        //send time to LCD
        int minutes = *timeControl / 60;
        int seconds = *timeControl % 60;
        char time[5];
        time[0] = minutes / 10 + '0';
        time[1] = minutes % 10 + '0';
        time[2] = ':';
        time[3] = seconds / 10 + '0';
        time[4] = seconds % 10 + '0';
        displayLCD(ROW_2, time, 5);

        if(*timeControl == 0){
            disableTimer(timerIndexControl);
            while(numMistekesControl!=3){
                addMistakeControl();
            }
        }
    }
}

/*
 * inizia il countdown
 */
void startTimerControl(){
    enableTimer(timerIndexControl);
}

/*
 * inizializza l'LCD (essendo l'unico modulo a contatto)
 * inizializza il timer e lo mette in pausa
 */
void setupControl(int* time, char* SN){
    timeControl = time;
    numMistekesControl = 0;
    timeStepperControl = 0;

    /*
     * inizializzo LCD
     */
    lcdConfig LCD = {
                           GPIO_PORT_P6, GPIO_PIN5,
                           GPIO_PORT_P6, GPIO_PIN4,
                           GPIO_PORT_P3, GPIO_PIN2,
                           GPIO_PORT_P3, GPIO_PIN3,
                           GPIO_PORT_P4, GPIO_PIN1,
                           GPIO_PORT_P4, GPIO_PIN3
                         };
    setupLCD(LCD);

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
    timerIndexControl=registerTimer(timeUpdateControl);
    disableTimer(timerIndexControl);

    /*
     * inizializzo LEDs
     */
    int i;
    for(i=0;i<3;i++){
        GPIO_setAsOutputPin(errLedPortsControl[i], errLedPinsControl[i]);
        GPIO_setOutputLowOnPin(errLedPortsControl[i], errLedPinsControl[i]);
    }
}

#endif
