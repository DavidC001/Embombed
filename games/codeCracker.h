#ifndef __CODECRACKER_H__
#define __CODECRACKER_H__

//#include <keypad.h>
//#include <control.h>
#include "../libs/emulators/keypad.h"
#include "../libs/emulators/control.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <constants.h>
#include <string.h>


char* CCnumpadCode;
int* CCTime;
uint8_t* currGameCC;

volatile char CCNumPadChar;
char CCInputLength;
char CCStringLength;

void setupCC(char* code, int* time, uint8_t* currGame){
    CCnumpadCode = code;
    CCTime = time;
    currGameCC = currGame;
    CCStringLength = strlen(CCnumpadCode);

    uint_fast8_t rowPorts[NUMROWS] = {GPIO_PORT_P4, GPIO_PORT_P2, GPIO_PORT_P2, GPIO_PORT_P5};
    uint_fast16_t rowPins[NUMROWS] = {GPIO_PIN5,GPIO_PIN6,GPIO_PIN4,GPIO_PIN6};
    uint_fast8_t colPorts[NUMCOLS] = {GPIO_PORT_P6, GPIO_PORT_P6, GPIO_PORT_P2, GPIO_PORT_P5};
    uint_fast16_t colPins[NUMCOLS] = {GPIO_PIN6,GPIO_PIN7,GPIO_PIN3,GPIO_PIN1};
    setUpKeyPad(rowPorts, rowPins, colPorts, colPins, &CCNumPadChar);
}

void gameCC(){
    //inizializzazione
    CCInputLength = 0;
    CCNumPadChar = 0;
    enableInterruptNumPad();

    //loop
    while(*CCTime>0 && *currGameCC==GAME_CODECRACKER){
        _sleep();
        
        if(CCNumPadChar!=0){

            evaluateKeyPad();

            if(CCNumPadChar!='E'){
                if(CCInputLength<CCStringLength){

                    if(CCNumPadChar==CCnumpadCode[CCInputLength]){

                        CCInputLength++;
                        correctFeedBackControl();

                        if(CCInputLength==CCStringLength){
                            victoryFeedBackControl();
                        }

                    }else{
                        addMistakeControl();
                        CCInputLength = 0;
                    }
                    
                }else{
                    victoryFeedBackControl();
                }
            }

            CCNumPadChar = 0;
        }
    }

    //disabilitazione
    disableInterruptNumPad();
}

#endif