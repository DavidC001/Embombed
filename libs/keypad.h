#ifndef __KEYPAD_H__
#define __KEYPAD_H__

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/**
 * @brief disable interrupt on keypad
 * @return None
 */
void disableInterruptNumPad();

/**
 * @brief enable interrupt on keypad
 * @return None
 */
void enableInterruptNumPad();

/**
 * @brief evaluate keypad
 * @return None
 * 
 * @note return the key pressed on the keypad by writing it in the memory pointed by puntNumPad
 * passed during the setup
 * if the key is released during the reading it returns 'E'
 * the reading must be discarded (could be a debouncing)
 */
void evaluateKeyPad();

/**
 * @brief setup keypad
 * @param rowPorts array of ports used for rows
 * @param rowPins array of pins used for rows
 * @param colPorts array of ports used for columns
 * @param colPins array of pins used for columns
 * @param res pointer to the variable where the pressed key will be written
 * @return None
 * 
 * @note setup the keypad, the ports and pins used for rows and columns
 * and the pointer to the variable where the pressed key will be written
 * 
 * important: the IRQ of the ports used will be blocked
 * calling enableInterrupt the IRQ will be reset
 * it is therefore possible to modify them by changing module
 * after calling disableInterrupt
 */
void setUpKeyPad(uint_fast8_t *rowPorts, uint_fast16_t *rowPins,
                 uint_fast8_t *colPorts, uint_fast16_t *colPins,
                 char *res);

#endif
