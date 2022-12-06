#include "msp.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "timer_subscription.h"
#include <stdio.h>

volatile int counter = 0;

void secondo(){
    counter++;
    if (counter==4){
        counter = 0;
        printf("passato un secondo\n");
    }
}

/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	setupTimer();
	registerTimer(secondo);

	while(1){
	    __sleep();
	}
}
