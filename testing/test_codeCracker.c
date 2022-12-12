#include "../games/codeCracker.h"
#include <stdio.h>

void setup(){

}

int main(){
    int time = 200;
    uint8_t currGame = GAME_CODECRACKER;
    lcdConfig lcd;
    printf("setupControl start\n");
    setupTimer();
    setupControl(&time, "codeCracker\0", &lcd, 0,0);
    startTimerControl();

    printf("setupCC start\n");
    setupCC("1234\0", &time, &currGame);

    printf("gameCC start\n");
    gameCC();
    return 0;
}