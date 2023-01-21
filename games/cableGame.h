#ifndef __CABLEGAME_H__
#define __CABLEGAME_H__

#ifndef TESTING
#include <cables.h>
#include <control.h>
#else
#include "../libs/emulators/cables.h"
#include "../libs/emulators/control.h"
#endif
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <constants.h>


uint8_t* CablesToCutCG; // The list of cables to cut
int* CGTime; // The time left in the game
uint8_t* currGameCG; // The current game

volatile uint8_t cableStatusCG[NUMCAVI]; // The status of the cables
uint8_t cableCheckedCG[NUMCAVI]; // list of checked cables

void setupCG(uint8_t* cables, int* time, uint8_t* currGame){
    CablesToCutCG = cables; // Set the cables to cut
    CGTime = time; // Set the time
    currGameCG = currGame; // Set the current game

    int i;
    for (i = 0; i < NUMCAVI; i++)
    {
        cableStatusCG[i] = 1; // Set all cables to active
        cableCheckedCG[i] = 0; // Set all cables to unchecked
    }

    uint_fast8_t Ports[NUMROWS] = {GPIO_PORT_P4, GPIO_PORT_P4, GPIO_PORT_P1, GPIO_PORT_P2}; // The ports of the cables
    uint_fast16_t Pins[NUMROWS] = {GPIO_PIN7,GPIO_PIN6,GPIO_PIN7,GPIO_PIN5}; // The pins of the cables
    setupCables(Pins, Ports, cableStatusCG); // Setup the cables
}

void gameCG(){
    //initialization
    uint8_t won = 0; // Set the won state to false
    enableInterruptCables(); // Enable the interrupts for the cables

    //game loop
    while(*CGTime>0 && *currGameCG==GAME_CABLES){ // While there is time left and the current game is cable cut
        #ifndef TESTING
        __sleep();
        #endif

        if(!won){ // If the player hasn't won yet
            //check the cut cables
            int i;
            for(i=0;i<NUMCAVI;i++){ // Check each cable
                if( !(cableStatusCG[i] || cableCheckedCG[i]) ){ // If the cable is cut and not already checked
                    cableCheckedCG[i] = 1; // Set the cable to checked
                    if(!CablesToCutCG[i]){ // If the cable shouldn't be cut
                        correctFeedBackControl(); // Give the player a correct feedback
                    }else{
                        addMistakeControl(); // Give the player a mistake feedback
                    }
                }
            }
            //if all the cables to cut have been cut
            uint8_t allCut = 1;
            for(i=0;i<NUMCAVI;i++){ // Check each cable
                if(!CablesToCutCG[i] && cableStatusCG[i]){ // If the cable should be cut and is active
                    allCut = 0; // Set allCut to false
                    break;
                }
            }
            if(allCut){ // If allCut is true
                victoryFeedBackControl(); // Give the player a victory feedback
                won = 1; // Set the won state to true
            }

        }

    }

    //disabling
    disableInterruptCables(); // Disable the interrupts for the cables
}

#endif
