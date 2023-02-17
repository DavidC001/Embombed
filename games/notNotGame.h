#ifndef __NOTNOTGAME_H__
#define __NOTNOTGAME_H__

#include <game_parser.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/**
 * @brief Setup the notNot game
 * 
 * @param game notNot game info
 * @param time pointer to the time left in the game
 * @param currGame pointer to the current game
 * 
 * @return None
 * 
 * @note sets up game variables and joystick and LCD drivers
 */
void setupNN(notNot* game, int* time, uint8_t* currGame);

/**
 * @brief notNot game loop
 * 
 * @return None
 * 
 * @note the game loop
 * checks the joystick direction
 * if the direction is correct the turn is incremented
 * if the direction is wrong the turn is reset
 * ramining time for each turn is rendered as a bar on the LCD
 * if all the turns are done the game is won
 * if the time runs out the player loses
 * if the game is changed the loop ends
 */
void gameNN();

#endif
