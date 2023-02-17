int max(int a, int b){
    if(a>b){
        return a;
    }else{
        return b;
    }
}

#include "../games/notNotGame.h"
#include "../libs/game_parser.h"
#include "../games/notNotGame.c"
#include "../libs/game_parser.c"
#include "../libs/emulators/control.h"
#include <stdio.h>
#include <string.h>

void setup(){

}

int main(){
    int time = 200;
    uint8_t currGame = GAME_NOTNOT;
    lcdConfig lcd;
    printf("setupControl start\n");
    setupTimer();
    setupControl(&time, "codeCracker\0", &lcd, 0,0);
    startTimerControl();

    notNot notNotGame;
    notNotGame.num_turni = 4;
    int i,j;
    for(i=0; i<notNotGame.num_turni; i++){
        for(j=0; j<4; j++){
            notNotGame.mosseCorrette[i][j] = 0;
        }
    }
    strcpy(notNotGame.messaggi[0], "up\0");
    notNotGame.mosseCorrette[0][1] = 1;
    strcpy(notNotGame.messaggi[1], "down\0");
    notNotGame.mosseCorrette[1][3] = 1;
    strcpy(notNotGame.messaggi[2], "left\0");
    notNotGame.mosseCorrette[2][0] = 3;
    strcpy(notNotGame.messaggi[3], "right\0");
    notNotGame.mosseCorrette[3][2] = 4;
    
    printf("setupNN start\n");
    setupNN(&notNotGame, &time, &currGame);

    printf("gameNN start\n");
    gameNN();
    return 0;
}