#ifndef __SIMONSAYSGAME_H__
#define __SIMONSAYSGAME_H__

#include <game_parser.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/**
 * @brief setup the simon says game
 * 
 * @param game simon says game info
 * @param time pointer to time left in the game
 * @param currGame pointer to the current game
 * 
 * @return None
 * 
 * @note sets up game variables and dot matrix driver
 */
void setupSS(simonSays* game, int* time, uint8_t* currGame);

/**
 * @brief simon says game loop
 * 
 * @return None
 * 
 * @note the game loop
 * sends the sequence
 * checks the sonda result
 * if the result is correct the turn is incremented
 * if the result is wrong the turn is reset
 * if all the turns are done the game is won
 * if the time runs out the player loses
 * if the game is changed the loop ends
 */
void gameSS();

#endif
