#include "msp.h"
#include "keypad.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	uint_fast16_t rowPin[4]={
	                         GPIO_PIN1,
                             GPIO_PIN3,
                             GPIO_PIN7,
                             GPIO_PIN6
	};
	uint_fast8_t rowPort[4]={
	                    GPIO_PORT_P5,
	                    GPIO_PORT_P2,
                        GPIO_PORT_P6,
                        GPIO_PORT_P6
	};
	uint_fast16_t colPin[4]={
                             GPIO_PIN6,
                             GPIO_PIN4,
                             GPIO_PIN6,
                             GPIO_PIN7
	};
	uint_fast8_t colPort[4]={
	                         GPIO_PORT_P5,
	                         GPIO_PORT_P2,
	                         GPIO_PORT_P2,
	                         GPIO_PORT_P2
	};

	char pressed = 0;

	/* Setting DCO to 48MHz */
	/* Set the core voltage level to VCORE1 */
	PCM_setCoreVoltageLevel(PCM_VCORE1);
	/* Set 2 flash wait states for Flash bank 0 and 1*/
	FlashCtl_setWaitState(FLASH_BANK0, 2);
	FlashCtl_setWaitState(FLASH_BANK1, 2);
	/* Initializes Clock System */
	CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48);

	setUpKeyPad(rowPort, rowPin, colPort, colPin, &pressed);

	int i = 1;

	while(1){
	    __sleep();
	    //printf("%d\n", GPIO_getInputPinValue(colPort[0], colPin[0]));
	    if(pressed){
	        printf("%d premuto il tasto %c\n", i, pressed);
	        ++i;
	        pressed = 0;
	    }
	}
}
