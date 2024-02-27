#include "../games/cableGame.h"
#include "../games/cableGame.c"
#include "../libs/emulators/control.h"
#include <stdio.h>

void setup(){

}

int main(){
    int time = 200;
    uint8_t currGame = GAME_CABLES;

    lcdConfig lcd;
    printf("setupControl start\n");
    setupTimer();
    setupControl(&time, "CABLES\0", &lcd, 0,0);
    startTimerControl();

    printf("setupCG start\n");
    uint8_t cables[NUMCAVI] = {0,1,0,0};
    setupCG(cables, &time, &currGame);

    printf("gameCG start\n");
    gameCG();
    return 0;
}