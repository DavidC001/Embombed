#include "../games/simonSaysGame.h"
#include "../libs/game_parser.h"
#include "../games/simonSaysGame.c"
#include "../libs/game_parser.c"
#include <stdio.h>
#include <string.h>

void setup(){

}

int main(){
    int time = 200;
    uint8_t currGame = GAME_SIMONSAYS;
    lcdConfig lcd;
    printf("setupControl start\n");
    setupTimer();
    setupControl(&time, "codeCracker\0", &lcd, 0,0);
    startTimerControl();

    //firts is + and x
    //second is - and /
    char mat[2][2][8] = {
        {
            {0b00011000,0b00011000,0b00011000,0b11111111,0b11111111,0b00011000,0b00011000,0b00011000},
            {0b10000001,0b01000010,0b00100100,0b00011000,0b00011000,0b00100100,0b01000010,0b10000001}
        },
        {
            {0b00000000,0b00000000,0b00000000,0b11111111,0b11111111,0b00000000,0b00000000,0b00000000},
            {0b11000000,0b01100000,0b00110000,0b00011000,0b00001100,0b00000110,0b00000011,0b00000001}
        }
    };
    simonSays game;
    game.numTurni = 2;
    int i,j,k;
    for(i=0; i<game.numTurni; i++){
        game.mossePerTurno[i] = 2;
        for(j=0; j<game.mossePerTurno[i]; j++){
            for(k=0; k<8; k++){
                game.matrix[i][j][k] = mat[i][j][k];
            }
            game.mosseCorrette[i][j] = j+1+i; // 1,2 - 2,3
        }
    }
    
    printf("setupSS start\n");
    setupSS(&game, &time, &currGame);

    sleep(1);

    printf("gameSS start\n");
    gameSS();
    return 0;
}