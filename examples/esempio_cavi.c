/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#include "cables.h"
#include <stdio.h>
#include <constants.h>

int main(void)
{
    volatile uint8_t conn[NUMCAVI];

    //to show those can be deleted they will be defined in this block

        uint_fast8_t ports[NUMCAVI] = {GPIO_PORT_P4, GPIO_PORT_P4, GPIO_PORT_P3, GPIO_PORT_P2};
        uint_fast16_t pins[NUMCAVI] = {GPIO_PIN7, GPIO_PIN6, GPIO_PIN7, GPIO_PIN5};

        setupCables(pins, ports, conn);


    enableInterruptCables();

    int i;
    while(1){
        //__sleep();
        for(i=0;i<NUMCAVI;i++){
            printf("cavo %d: %d\n", i, conn[i]);
            printf("%d\n",GPIO_getInputPinValue(ports[i], pins[i]));
        }
        printf("\n");
    }
}
