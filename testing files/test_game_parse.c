#include <stdio.h>
#include <stdlib.h>
#include "../libs/emulators/mutex.h"
#include <game_parser.h>

int main(){
    initMutex();

    FILE *fp = fopen("../python/genGame/game/game.json", "r");
    if(fp == NULL){
        printf("Error opening file\n");
        return 1;
    }

    char json[1000000];
    int i = 0;
    while(!feof(fp)){
        char c = fgetc(fp);
        json[i] = c;
        i++;
    }
    json[i-1] = '\0';
    fclose(fp);

    //parse json
    game g = gameParser(json,strlen(json));
    printGame(g);
}