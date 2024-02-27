#ifndef __CABLEGAME_H__
#define __CABLEGAME_H__

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/**
 * @brief setup the cable game
 * 
 * @param cables array of correct cables states
 * @param time pointer to time left in the game
 * @param currGame pointer to the current game
 * 
 * @return None
 * 
 * @note sets up game variables and cables driver
 */
void setupCG(uint8_t* cables, int* time, uint8_t* currGame);

/**
 * @brief cable game loop
 * 
 * @return None
 * 
 * @note the game loop
 * checks the cut cables and gives feedback
 * if all the cables to cut have been cut the player wins
 * if the time runs out the player loses
 * if the game is changed the loop ends
 */
void gameCG();

#endif
