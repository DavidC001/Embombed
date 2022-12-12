#include "../libs/emulators/keypad.h"
#include "../libs/emulators/dotMatrix.h"
#include "../libs/emulators/LCD.h"
#include "../libs/emulators/control.h"
#include "../libs/emulators/sonda.h"
#include <stdio.h>

int main(){
    initMutex();

    // Initialize the keypad
    uint_fast8_t ports[4] = {0x00, 0x00, 0x00, 0x00};
    uint_fast16_t pins[4] = {0x0000, 0x0000, 0x0000, 0x0000};
    char read=0;
    setUpKeyPad(ports, pins, ports, pins, &read);
    enableInterruptNumPad();

    //conf dot matrix
    char mat[8]={0xff, 0x81, 0x81, 0x81, 0xff, 0x81, 0x81, 0x81};
    initMatrix(0,0);
    turnOnMat();
    //write on the dot matrix
    sendMat(mat);

    //conf control
    int time = 200;
    lcdConfig lcd;
    setupTimer();
    setupControl(&time,"ABCDET\0", &lcd, 0, 0);
    startTimerControl();

    //conf sonda
    uint8_t sonda=0;
    setupSonda(0,0,0,0,0,&sonda);
    enableInterruptSonda();

    while(1){
        if(read!=0){
            evaluateKeyPad();
            printf("Key found: %c\n", read);
            read=0;
        }

        if(sonda!=0){
            printf("Sonda: %d\n", sonda);
            sonda=0;
        }
    }
}