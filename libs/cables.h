#ifndef __CABLES_H__
#define __CABLES_H__

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/**
 * @brief enable interrupt on cables
 * @return None
 * 
 * @note enable interrupt on pin and register IRQ on port
 */
void enableInterruptCables();

/**
 * @brief disable interrupt on cables
 * @return None 
 * 
 * @note remove IRQ handler on port and disable interrupt on pin
 */
void disableInterruptCables();

/**
 * @brief setup cables
 * @param pins vector of pins
 * @param ports vector of ports
 * @param conn vector of connection status
 * 
 * 
 * @note all vectors must be of size NUMCAVI
 * defined inside constants.h
 * pins and ports can be deleted after calling this function
 * conn vector will be used to communicate with the caller
 * conn will be initialized with ones
 *
 * by default interrupts are disabled
 * will overwrite port interrupt!!!
 *
 * ordine cavi: P Y G R
 */
void setupCables(uint_fast16_t* pins, uint_fast8_t* ports,uint8_t* conn);

#endif
