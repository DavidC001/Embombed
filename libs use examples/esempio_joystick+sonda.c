#include "msp.h"
/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "joystick.h"
#include <stdio.h>
#include <sonda.h>


/**
 * main.c
 */
void main(void)
{
    MAP_WDT_A_holdTimer();

    setupADC();

    char ril=0;

    uint8_t rilevato = 0;
    setupSonda(GPIO_PIN5, GPIO_PORT_P5, ADC_INPUT_A0, ADC_INT0,
                   &rilevato);
    enableInterruptSonda();

    setupJoystick(&ril);
    enableInterruptJoystick();
    enableConvADC();

    while(1){
        __sleep();
        if(ril!=0){
            printf("Rilevato %c\n",ril);
            ril=0;
        }
        if(rilevato!=0){
            printf("sonda %d\n", rilevato);
            rilevato = 0;
        }
    }
}
