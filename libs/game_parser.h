#ifndef __GAMEPARSER_H__
#define __GAMEPARSER_H__

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <constants.h>

typedef struct
{
    char messaggi[NOTNOTMAXTURNS][NOTNOTMAXMSGDIM];    //what to show on screen
    uint8_t mosseCorrette[NOTNOTMAXTURNS][4]; //what is the correct move (1-left 2-up 3-right 4-down), I can have more than one correct moves
    int num_turni;                                   //number of turns in the game
} notNot;

typedef struct
{
    char matrix[SIMONSAYSMAXTURNS][SIMONSAYSMAXSTEPPERTURN][8];         //matrix[1][2][1] -> how to turn on the led in the first turn, second flash, line 1 matrix led
    uint8_t mosseCorrette[SIMONSAYSMAXTURNS][SIMONSAYSMAXSTEPPERTURN];  //index logic is the same, contains the correct sequence of moves for each turn (1-9 for which button is correct)
    int mossePerTurno[SIMONSAYSMAXTURNS];                               //for each turn how many moves there are
    int numTurni;                                                       //how many turns there are
} simonSays;

typedef struct
{
    char SerialNumber[SNMAXDIM];        //serial number bomb
    int time;                           //time before explosion in seconds
    uint8_t FiliDaTagliare[NUMCAVI];    //cable order: P Y G R
    notNot notNotGame;       
    simonSays simonSaysGame; 
    char numPadCode[CODEPADMAXDIM];     //code for the keypad
    uint8_t valid_game;                 //1 if the game is valid, 0 otherwise
} game;

/**
 * @brief print the game info
 * @param gameInfo struct with the game info
 * @return None
 */
void printGame(game gameInfo);

/**
 * @brief parse the game json
 * @param data json string
 * @param size size of the json string
 * @return game struct with the game info
 */
game gameParser(char *data, int size);

#endif
