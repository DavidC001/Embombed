#ifndef __CODECRACKER_H__
#define __CODECRACKER_H__

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/**
 * @brief setup the code cracker game
 * 
 * @param code code to crack
 * @param time pointer to time left in the game
 * @param currGame pointer to the current game
 * 
 * @return None
 * 
 * @note sets up game variables and keypad driver
 */
void setupCC(char* code, int* time, uint8_t* currGame);

/**
 * @brief code cracker game loop
 * 
 * @return None
 * 
 * @note the game loop
 * checks the pressed keys and gives feedback
 * if the code is cracked the player wins
 * if the time runs out the player loses
 * if the game is changed the loop ends
 */
void gameCC();

#endif
