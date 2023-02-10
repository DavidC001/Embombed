#include "msp.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <LCD.h>
#include "control.h"
#include <timer_subscription.h>
#include <stdio.h>


/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	int seconds = 100;

	//deve essere inizializzato fuori, può essere fatto dopo
	setupTimer();

	//inizializzo il modulo control
	setupControl(&seconds, "BELLA\0");

	//faccio partire il countdown
	startTimerControl();
	int e=0;

	while(1){
	    __sleep();
	    if(seconds==80 && e==0){
			//aggiungo un errore
	        addMistakeControl();
	        e++;
	    }
	    if(seconds==60 && e==1){
			//aggiungo un errore
	        addMistakeControl();
	        e++;
	    }
	}
}
