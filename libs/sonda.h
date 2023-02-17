#ifndef __SONDA_H__
#define __SONDA_H__

/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/**
 * @brief disable interrupt on sonda
 * @return None
 */
void disableInterruptSonda();

/**
 * @brief enable interrupt on sonda
 * @return None
 */
void enableInterruptSonda();

/**
 * @brief setup sonda
 * @param pin pin of the sonda
 * @param port port of the sonda
 * @param ADCInputMap ADC input map of the sonda
 * @param ADCMemory ADC memory of the sonda
 * @param punt pointer to the variable where the value of the sonda will be written
 * @return None
 * 
 * @note setup the sonda on the pin and port passed as parameters
 * the value of the sonda will be written in the memory pointed by punt
 */
void setupSonda(uint16_t pin, uint8_t port, uint32_t ADCInputMap, uint32_t ADCMemory, uint8_t* punt);

#endif
