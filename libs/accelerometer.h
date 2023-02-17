#ifndef __ACCELEROMETER_H__
#define __ACCELEROMETER_H__

/**
 * @brief This function sets up the accelerometer sensor for the game.
 * @param currGameP A pointer to the current game state variable.
 * @return None
 *
 * @note This function initializes the accelerometer sensor by setting up two ADC input pins (P4.2 and P6.1) 
 * and configuring two conversion memory registers (ADC_MEM3 and ADC_MEM4) for the x-axis and z-axis readings. 
 * It also registers an interrupt handler function (ADCAccelerometerIRQ) that will be called when a conversion is complete.
 * The function uses three global variables: accFacePunt, accValid, and accBuffer,
 * which store the pointer to the current game state, a flag indicating if the accelerometer reading is valid, 
 * and a buffer for storing the game face value, respectively.
 */
void setupAccelerometer(uint8_t* currGameP);

#endif
