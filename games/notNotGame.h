#ifndef __NOTNOTGAME_H__
#define __NOTNOTGAME_H__

#include <joystick.h>
#include <control.h>
//#include "../libs/emulators/joystick.h"
//#include "../libs/emulators/control.h"
#include <constants.h>
#include <game_parser.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

notNot* notNotGameNN; // The notNot game info
int* NNTime; // The time left in the game
uint8_t* currGameNN; // The current game

volatile char directionJoystickNN; // The direction of the joystick
int turnNN; // The turn of the game
int sSecondsNN; // Time at turn start

int min(int i, int j){
    if(i<j) return i;
    else return j;
}

void setupNN(notNot* game, int* time, uint8_t* currGame){
    notNotGameNN = game; // Set the game info
    NNTime = time; // Set the time
    currGameNN = currGame; // Set the current game

    setupJoystick(&directionJoystickNN); // Setup the joystick
}

void gameNN(){
    //initialization
    directionJoystickNN = 0; // Set the direction of the joystick to 0
    if(!(turnNN==notNotGameNN->num_turni)) turnNN = 0; // reset the turn
    sSecondsNN = *NNTime; // Set the time at turn start
    enableInterruptJoystick(); // Enable the interrupts

    //TODO: SEND FIRST MESSAGE ON DISPLAY and set full time left bar
    if(!(turnNN==notNotGameNN->num_turni)){
        printf("Turno %d messaggio %s\n", turnNN, notNotGameNN->messaggi[turnNN]);
        printf("Time left: %d\n", min(10,20-turnNN*2));
    }

    //game loop
    while(*NNTime>0 && *currGameNN==GAME_NOTNOT){ // While there is time left and the current game is notNot
        __sleep(); // waked by timer and ADC interrupts

        if(*NNTime-sSecondsNN>=min(10,20-turnNN*2)){ // If the time is up
            addMistakeControl(); // Add a mistake
            turnNN = 0; // Reset the turn
            sSecondsNN = *NNTime; // Set the time at turn start
        }

        if(directionJoystickNN!=0){

            if(turnNN==notNotGameNN->num_turni){
                victoryFeedBackControl();
            }else{
                uint8_t correct = 0;
                switch (directionJoystickNN){
                    case 'L':
                        if(notNotGameNN->mosseCorrette[turnNN][0]){
                            correct = 1;
                        }
                        break;
                    case 'U':
                        if(notNotGameNN->mosseCorrette[turnNN][1]){
                            correct = 1;
                        }
                        break;
                    case 'R':
                        if(notNotGameNN->mosseCorrette[turnNN][2]){
                            correct = 1;
                        }
                        break;
                    case 'D':
                        if(notNotGameNN->mosseCorrette[turnNN][3]){
                            correct = 1;
                        }
                        break;
                }

                if(correct){
                    correctFeedBackControl();
                    turnNN++;

                    if(turnNN==notNotGameNN->num_turni){
                        victoryFeedBackControl();
                    }else{
                        //TODO: SEND NEXT MESSAGE ON DISPLAY
                        printf("Turno %d messaggio %s\n", turnNN, notNotGameNN->messaggi[turnNN]);
                    }

                }else {
                    addMistakeControl();
                    turnNN = 0;

                    //TODO: SEND FIRST MESSAGE ON DISPLAY
                    printf("Turno %d messaggio %s\n", turnNN, notNotGameNN->messaggi[turnNN]);
                }

                sSecondsNN = *NNTime; // Set the time at turn start
            }

            directionJoystickNN=0;
        }

        //TODO: UPDATE TIME LEFT BAR
        //printf("Time left: %d\n", min(10,20-turnNN*2));
    }

    //disabling
    disableInterruptJoystick(); // Disable the interrupts
}

#endif
