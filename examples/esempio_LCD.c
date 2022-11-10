#include "msp.h"
#include "LCD.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>


/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	//setto i pin a cui il display è connesso
	lcdConfig config= {
	             GPIO_PORT_P2, GPIO_PIN5, //rs
	             GPIO_PORT_P2,  GPIO_PIN6, //e
	             GPIO_PORT_P4, GPIO_PIN5, //d4
                 GPIO_PORT_P4, GPIO_PIN4, //d5
                 GPIO_PORT_P4, GPIO_PIN2, //d6
                 GPIO_PORT_P4, GPIO_PIN0  //d7
	};
	//inizializzo lo schermo
	setupLCD(config);

	int i;
	while(1){
	    //per mostrare sul display
	    //definisco la riga su cui visualizzare il messaggio, il messaggio e la sua lunghezza
	    displayLCD(ROW_1, "ciao!", 5);
        displayLCD(ROW_2, "mondo", 5);
        for(i=0;i<300000;i++);
        displayLCD(ROW_1, "mondo", 5);
        displayLCD(ROW_2, "ciao!", 5);
        for(i=0;i<300000;i++);
	}
}
