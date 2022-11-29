#ifndef __KEYPAD_H__
#define __KEYPAD_H__

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#define NUMROWS 4
#define NUMCOLS 4

uint_fast16_t *rowPin;
uint_fast16_t *colPin;
uint_fast8_t *rowPort;
uint_fast8_t *colPort;

uint_fast16_t status[4];

char *puntNumPad;

char numPad[4][4] = { '1', '2', '3', 'A',
                      '4', '5', '6', 'B',
                      '7', '8', '9', 'C',
                      '*', '0', '#', 'D' };

void IRQ_col(void);

void disableInterruptNumPad()
{
    int i;
    for (i = 0; i < NUMCOLS; i++)
    {
        GPIO_unregisterInterrupt (colPort[i]);
        GPIO_disableInterrupt(colPort[i], colPin[i]);
    }
}

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

/*
 * ritorna la chiave che � stata premuta
 * nel caso di rilevazione durante il rilascio
 * viene ritornato il carattere 'E'
 * va quindi scartata la lettura
 */
void evaluateKeyPad()
{
    disableInterruptNumPad();

    //debouncing, aspetto che il segnale si stabilizzi per 10ms
    __delay_cycles(480000);

    int i, k;

    for (i = 0; i < NUMCOLS; i++)
    {
        if (status[i])
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
        status[i] = GPIO_getInterruptStatus(colPort[i], colPin[i]);
        GPIO_clearInterruptFlag(colPort[i], colPin[i]);
    }
    *puntNumPad = 1;
}

/*
 * NOTA: gli array non devo essere distrutti nel chiamante
 * IMPORTANTE: gli IRQ delle porte usate saranno bloccati
 * chiamando enableInterrupt vengono resettati gli IRQ
 * � quindi possibile modificarli cambiando modulo
 * previa chiamata di disableInterrupt
 */
void setUpKeyPad(uint_fast8_t *rowPorts, uint_fast16_t *rowPins,
                 uint_fast8_t *colPorts, uint_fast16_t *colPins,
                 char *res)
{
    int i;

    puntNumPad = res;

    colPort = colPorts;
    colPin = colPins;
    rowPort = rowPorts;
    rowPin = rowPins;

    for (i = 0; i < NUMCOLS; i++)
    {
        GPIO_setAsInputPinWithPullDownResistor(colPort[i], colPin[i]);
        GPIO_clearInterruptFlag(colPort[i], colPin[i]);
        GPIO_enableInterrupt(colPort[i], colPin[i]);

        GPIO_unregisterInterrupt(colPorts[i]);
        GPIO_registerInterrupt(colPort[i], IRQ_col);
    }

    for (i = 0; i < NUMROWS; i++)
    {
        GPIO_setAsOutputPin(rowPorts[i], rowPins[i]);
        GPIO_setOutputHighOnPin(rowPorts[i], rowPins[i]);
    }

    disableInterruptNumPad();

}

#endif