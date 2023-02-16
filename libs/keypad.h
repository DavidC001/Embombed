#ifndef __KEYPAD_H__
#define __KEYPAD_H__

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <constants.h>

uint_fast16_t rowPin[NUMROWS];
uint_fast16_t colPin[NUMCOLS];
uint_fast8_t rowPort[NUMROWS];
uint_fast8_t colPort[NUMCOLS];

volatile uint_fast16_t statusKeypad[4];

char *puntNumPad;

char numPad[4][4] = { '1', '2', '3', 'A',
                      '4', '5', '6', 'B',
                      '7', '8', '9', 'C',
                      '*', '0', '#', 'D' };

void IRQ_col(void);

/**
 * @brief disable interrupt on keypad
 * @return None
 */
void disableInterruptNumPad()
{
    int i;
    for (i = 0; i < NUMCOLS; i++)
    {
        GPIO_unregisterInterrupt (colPort[i]);
        GPIO_disableInterrupt(colPort[i], colPin[i]);
    }
}

/**
 * @brief enable interrupt on keypad
 * @return None
 */
void enableInterruptNumPad()
{
    int i;
    for (i = 0; i < NUMCOLS; i++)
    {
        GPIO_clearInterruptFlag(colPort[i], colPin[i]);
        GPIO_enableInterrupt(colPort[i], colPin[i]);

        GPIO_unregisterInterrupt (colPort[i]);
        GPIO_registerInterrupt(colPort[i], IRQ_col);
    }
}

/**
 * @brief evaluate keypad
 * @return None
 * 
 * return the key pressed on the keypad by writing it in the memory pointed by puntNumPad
 * passed during the setup
 * if the key is released during the reading it returns 'E'
 * the reading must be discarded (could be a debouncing)
 */
void evaluateKeyPad()
{
    disableInterruptNumPad();

    //debouncing, aspetto che il segnale si stabilizzi per 10ms
    __delay_cycles(480000);

    int i, k;

    for (i = 0; i < NUMCOLS; i++)
    {
        if (statusKeypad[i])
        {
            for (k = 0; k < NUMROWS; k++)
            {
                GPIO_setOutputLowOnPin(rowPort[k], rowPin[k]);

                uint8_t input = GPIO_getInputPinValue(colPort[i], colPin[i]);
                if (!input)
                {
                    *puntNumPad = numPad[k][i];
                }

                GPIO_setOutputHighOnPin(rowPort[k], rowPin[k]);
            }

            //se � stato rilasciato immezzo alla lettura questa non � affidabile e viene scartata
            //oppure era un picco durante il rilascio
            if (!GPIO_getInputPinValue(colPort[i], colPin[i]))
            {
                *puntNumPad = 'E';
            }

        }
    }

    enableInterruptNumPad();
}

void IRQ_col(void)
{
    int i;
    for(i=0;i<NUMCOLS;i++){
        statusKeypad[i] = GPIO_getInputPinValue(colPort[i], colPin[i]);
        GPIO_clearInterruptFlag(colPort[i], colPin[i]);
    }
    if(*puntNumPad==0){
        *puntNumPad = 1;
    }
}

/**
 * @brief setup keypad
 * @param rowPorts array of ports used for rows
 * @param rowPins array of pins used for rows
 * @param colPorts array of ports used for columns
 * @param colPins array of pins used for columns
 * @param res pointer to the variable where the pressed key will be written
 * @return None
 * 
 * setup the keypad, the ports and pins used for rows and columns
 * and the pointer to the variable where the pressed key will be written
 * 
 * important: the IRQ of the ports used will be blocked
 * calling enableInterrupt the IRQ will be reset
 * it is therefore possible to modify them by changing module
 * after calling disableInterrupt
 */
void setUpKeyPad(uint_fast8_t *rowPorts, uint_fast16_t *rowPins,
                 uint_fast8_t *colPorts, uint_fast16_t *colPins,
                 char *res)
{
    int i;

    puntNumPad = res;

    for (i = 0; i < NUMROWS; i++)
    {
        rowPort[i] = rowPorts[i];
        rowPin[i] = rowPins[i];
        
        GPIO_setAsOutputPin(rowPorts[i], rowPins[i]);
        GPIO_setOutputHighOnPin(rowPorts[i], rowPins[i]);
    }
    for (i = 0; i < NUMCOLS; i++)
    {
        colPort[i] = colPorts[i];
        colPin[i] = colPins[i];
        
        GPIO_setAsInputPinWithPullDownResistor(colPort[i], colPin[i]);
        GPIO_interruptEdgeSelect(colPort[i], colPin[i], GPIO_LOW_TO_HIGH_TRANSITION);
        GPIO_clearInterruptFlag(colPort[i], colPin[i]);
        GPIO_enableInterrupt(colPort[i], colPin[i]);

        GPIO_unregisterInterrupt(colPorts[i]);
        GPIO_registerInterrupt(colPort[i], IRQ_col);
    }

    disableInterruptNumPad();

}

#endif
