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
	lcdConfig LCD = {
	                   GPIO_PORT_P6, GPIO_PIN5,
	                   GPIO_PORT_P6, GPIO_PIN4,
	                   GPIO_PORT_P3, GPIO_PIN2,
	                   GPIO_PORT_P3, GPIO_PIN3,
	                   GPIO_PORT_P4, GPIO_PIN1,
	                   GPIO_PORT_P4, GPIO_PIN3
	                 };
	uint8_t portsErr[3] = {GPIO_PORT_P2,GPIO_PORT_P5,GPIO_PORT_P5};
    uint16_t pinsErr[3] = {GPIO_PIN5,GPIO_PIN0,GPIO_PIN2};
	setupControl(&seconds, "BELLA\0",&LCD,portsErr,pinsErr);

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
