#ifndef __JOYSTICK_H__
#define __JOYSTICK_H__

/**
 * @brief disable interrupt on joystick
 * @return None
 * 
 * @note disable interrupt on ADC
 */
void disableInterruptJoystick();

/**
 * @brief enable interrupt on joystick
 * @return None
 * 
 * @note enable interrupt on ADC
 */
void enableInterruptJoystick();

/**
 * @brief setup joystick
 * @param punt pointer to variable that will contain the direction of the joystick
 * @return None
 * 
 * @note setup joystick
 * configures pin 6.0 and 4.4 as ADC input
 * configures ADC memory 1 and 2
 * registers ADCJoystickIRQ as interrupt handler
 */
void setupJoystick(char* punt);

#endif
