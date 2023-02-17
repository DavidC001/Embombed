#ifndef TESTING
#include <dotMatrix.h>
#include <sonda.h>
#include <control.h>
#else
#include "../libs/emulators/dotMatrix.h"
#include "../libs/emulators/sonda.h"
#include "../libs/emulators/control.h"
#endif

#include <constants.h>
#include <game_parser.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>


simonSays* SSgame; // The notNot game info
int* SSTime; // The time left in the game
uint8_t* currGameSS; // The current game

volatile uint8_t sondaRisSS; // The sonda result 
uint8_t turnSS; // The turn, 8 cause max is 10
uint8_t stepSolSS; // The step in the solution

void showSequence(){
    int i;
    //show sequence
    for(i=0;i<SSgame->mossePerTurno[turnSS];i++)
    {
        if(*currGameSS!=GAME_SIMONSAYS) return; // If the current game is not simon says, return
        if(*SSTime<=0) return; // If the time is up, return
        sendMat(SSgame->matrix[turnSS][i]);
        int time = *SSTime;
        while(time<=*SSTime+1) //wait at least 2 second
        {
            if(!(*SSTime>0 && *currGameSS==GAME_SIMONSAYS)) return;
            #ifndef TESTING
            __sleep();
            #endif
        }
        clearMat();
    }
}

void setupSS(simonSays* game, int* time, uint8_t* currGame){
    SSgame = game; // Set the game info
    SSTime = time; // Set the time
    currGameSS = currGame; // Set the current game
    turnSS=0;

    //setup dot matrix
    initMatrix(GPIO_PORT_P5, GPIO_PIN4);
    //setup sonda
    setupSonda(GPIO_PIN5,GPIO_PORT_P5,ADC_INPUT_A0,ADC_MEM0,&sondaRisSS);
}

void gameSS(){
    //initialization
    int time;
    if(turnSS<SSgame->numTurni) turnSS=0;
    stepSolSS=0;
    clearMat();
    turnOnMat();
    enableInterruptSonda();

    //send first sequence
    if(turnSS<SSgame->numTurni) showSequence();
    clearMat();
    time = *SSTime;

    //game loop
    while(*SSTime>0 && *currGameSS==GAME_SIMONSAYS){ // While there is time left and the current game is still simon says
        #ifndef TESTING
        __sleep();
        #endif

        //when an input is received
        if(sondaRisSS!=0){
            //check if the game is still running
            if(turnSS < SSgame->numTurni){
                //check if the sonda result is correct
                if(sondaRisSS==SSgame->mosseCorrette[turnSS][stepSolSS]){
                    stepSolSS++;
                    correctFeedBackControl();
                    if(stepSolSS==SSgame->mossePerTurno[turnSS]){
                        //turn completed
                        turnSS++;
                        stepSolSS=0;
                        if(turnSS==SSgame->numTurni){
                            //game completed
                            victoryFeedBackControl();
                            break;
                        }else{
                            //send next sequence
                            showSequence();
                            clearMat();
                            time = *SSTime;
                        }
                    }
                }else{
                    //wrong
                    turnSS=0;
                    stepSolSS=0;
                    addMistakeControl();
                    showSequence();
                    clearMat();
                    time = *SSTime;
                }
            }else{
                //game completed
                victoryFeedBackControl();
            }

            //reset sonda
            sondaRisSS=0;
        }

        //every 10 seconds, replay the sequence
        if(time-*SSTime>10 && turnSS<SSgame->numTurni){
            showSequence();
            clearMat();
            time = *SSTime;
        }
    }

    //disabling
    disableInterruptSonda();
    shutdownMat();
}
