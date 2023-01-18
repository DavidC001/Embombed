#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdio.h>
#include <ADC_subscription.h>
#include <constants.h>
#include <accelerometer.h>


/*
 * Main function
 */
int main(void)
{
    uint8_t currGame=GAME_NONE;
    setupADC();
    setupacc(&currGame);
    enableConvADC();

    while (1)
    {
        if(currGame==GAME_NOTNOT){
            printf("up\n");
        }
        if(currGame==GAME_CODECRACKER){
            printf("down\n");
        }
        if(currGame==GAME_SIMONSAYS){
            printf("left\n");
        }
        if(currGame==GAME_CABLES){
            printf("right\n");
        }
        if(currGame==GAME_NONE){
            printf("invalid\n");
        }
        __sleep();
    }
}

