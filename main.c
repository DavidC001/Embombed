#include "msp.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#include <constants.h>

//config
#include <uart_d.h>
#include <game_parser.h>

//hw
#include <ADC_subscription.h>
#include <accelerometer.h>
#include <control.h>

//games
#include <cableGame.h>
#include <codeCracker.h>
#include <notNotGame.h>
#include <simonSaysGame.h>

game game_info;
uint8_t currGame = GAME_NONE;

char message[MAX_MSG_SIZE];
int size;

void set_DCO(){
    /* Set the core voltage level to VCORE1 */
    PCM_setCoreVoltageLevel(PCM_VCORE1);

    /* Set 2 flash wait states for Flash bank 0 and 1*/
    FlashCtl_setWaitState(FLASH_BANK0, 2);
    FlashCtl_setWaitState(FLASH_BANK1, 2);

    /* Initializes Clock System */
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48);
    CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);

    CS_setReferenceOscillatorFrequency(CS_REFO_32KHZ);
    CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_32);
}

void config(){
    //setup
    size=0;
    setupUART(message, &size, 2);

    //ask for game
    sendUART("S", 1);

    //wait message
    while(size==0){
        __sleep();
    }

    //consume message
    game_info = gameParser(message, size);

    //delete message
    size=0;
}

void hw_init(){
    setupADC();
    setupControl(&(game_info.time), game_info.SerialNumber);
    setupAccelerometer(&currGame);
}

void games_init(){
    setupCC(game_info.numPadCode, &(game_info.time), &currGame);
    setupCG(game_info.FiliDaTagliare, &(game_info.time), &currGame);
    setupSS(&(game_info.simonSaysGame), &(game_info.time), &currGame);
    setupNN(&(game_info.notNotGame), &(game_info.time), &currGame);
}

int main(void)
{
    /* Halting WDT  */
    MAP_WDT_A_holdTimer();

    set_DCO();
    config();
    //printGame(game_info);

    hw_init();
    games_init();

    enableConvADC();
    startTimerControl();

    void (*gameLoops[])(void) = {
        gameCC,
        gameNN,
        gameSS,
        gameCG
    };

    while(game_info.time>0){
        __sleep();

        if(currGame!=GAME_NONE){
            gameLoops[currGame]();
        }

    }

    explosionSound();
}
