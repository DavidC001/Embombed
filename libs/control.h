#ifndef __CONTROL_H__
#define __CONTROL_H__

/**
 * @brief add a mistake
 * @return None
 * 
 * @note add a mistake to the counter, turn on the led and send the buzzer melody for a mistake
 * if the counter is 3 is game over and the timer is stopped
 */
void addMistakeControl();

/**
 * @brief feedback for a correct answer
 * @return None
 * 
 * @note send the buzzer melody for a correct answer
 */
void correctFeedBackControl();

/**
 * @brief feedback for a solved module
 * @return None
 * 
 * @note send the buzzer melody for a solved module
 */
void victoryFeedBackControl();

/**
 * @brief feedback for game over
 * @return None
 * 
 * @note send the buzzer melody for game over
 * and stops the music
 */
void explosionSound();

/**
 * @brief start the countdown
 * @return None
 * 
 * @note start the timer for the countdown and enable the interrupt
 */
void startTimerControl();

/**
 * @brief setup the control module
 * @param time pointer to the time variable
 * @param SN serial number of the module
 * @return None
 * 
 * @note setup the control module
 * inizialize the LCD, the timer, the LEDs and the buzzer
 */
void setupControl(int* time, char* SN);

#endif
