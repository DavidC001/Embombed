#include "msp.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "dotMatrix.h"

/**
 * main.c
 */


void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

	/* Setting DCO to 48MHz */
	/* Set the core voltage level to VCORE1 */
	PCM_setCoreVoltageLevel(PCM_VCORE1);
	/* Set 2 flash wait states for Flash bank 0 and 1*/
	FlashCtl_setWaitState(FLASH_BANK0, 2);
	FlashCtl_setWaitState(FLASH_BANK1, 2);
	/* Initializes Clock System */
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48);

	initMatrix(GPIO_PORT_P2, GPIO_PIN5);

	turnOnMat();
	char mat[8]={
	             0x24,
	             0xFF,
                 0xFF,
	             0x7E,
	             0x7E,
	             0x3C,
	             0x18,
	             0x00
	};
	sendMat(mat);
}
