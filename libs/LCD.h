#ifndef __LCD_H__
#define __LCD_H__

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

typedef struct{
    uint_fast8_t port_rs;
    uint_fast16_t pin_rs;
    uint_fast8_t port_e;
    uint_fast16_t pin_e;
    uint_fast8_t port_d4;
    uint_fast16_t pin_d4;
    uint_fast8_t port_d5;
    uint_fast16_t pin_d5;
    uint_fast8_t port_d6;
    uint_fast16_t pin_d6;
    uint_fast8_t port_d7;
    uint_fast16_t pin_d7;
}lcdConfig;

typedef enum{
    ROW_1,
    ROW_2
} rowSel;

/**
 * @brief clear the lcd
 * @return None
 */
void clearLCD();

/**
 * @brief setup the lcd
 * @param conf the configuration of the lcd
 * @return None
 * 
 * @note the configuration of the lcd is a struct with the following fields:
 * port_rs, pin_rs, port_e, pin_e, port_d4, pin_d4, port_d5, pin_d5, port_d6, pin_d6, port_d7, pin_d7
 * 
 * for the waiting time the module assumes a frequency of 48MHz
*/
void setupLCD(lcdConfig conf);

/**
 * @brief display a string on the lcd
 * @param row the row where the string will be displayed
 * @param string the string to display
 * @param size the size of the string
 * @return None
 * 
 * @note the string will be centered on the row
 * the row can be ROW_1 or ROW_2
 * the size of the string must be less than 16
 */
void displayLCD(rowSel row, char* string, int size);

#endif
