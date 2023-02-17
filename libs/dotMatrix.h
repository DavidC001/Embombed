#ifndef __DOTMATRIX_H__
#define __DOTMATRIX_H__

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/**
 * @brief clear the matrix of led
 * 
 * @return void
 * 
 * @note send the command to clear the matrix of led
 */
void clearMat();

/**
 * @brief shutdown the matrix of led
 * 
 * @return void
 * 
 * @note send the command to shutdown the matrix of led
 */
void shutdownMat();

/**
 * @brief turn on the matrix of led
 * 
 * @return void
 * 
 * @note send the command to turn on the matrix of led
 */
void turnOnMat();

/**
 * @brief initialize the matrix of led
 * 
 * @param csPort the port of the chip select
 * @param csPin the pin of the chip select
 * 
 * @return void
 * 
 * @note uses the EUSCI_B2 by default the pins are:
 * P3.5 as clock
 * P3.6 as data
 * the cs pin can be configured
 * 
 * the pins are configured and then the matrix is initialized with the following commands:
 * display test off
 * scan limit 8
 * intensity 14
 * decode mode off
 * clear the matrix
 * shutdown the matrix
 */
void initMatrix(uint_fast8_t csPort, uint_fast16_t csPin);


#endif
