#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <constants.h>
#include <cables.h>

uint_fast16_t pinCables[NUMCAVI];
uint_fast8_t portCables[NUMCAVI];
uint8_t* connectedCables;

void IRQ_cables(void){
    int i;
    //debounce
    __delay_cycles(48000);
    for(i=0;i<NUMCAVI;i++){
        GPIO_clearInterruptFlag(portCables[i], pinCables[i]);
        uint8_t val = GPIO_getInputPinValue(portCables[i], pinCables[i]);
        if(!val){
            connectedCables[i] = 0;
        }
    }
}


void enableInterruptCables(){
    int i;
    for (i = 0; i < NUMCAVI; i++)
    {
        GPIO_clearInterruptFlag(portCables[i], pinCables[i]);
        GPIO_enableInterrupt(portCables[i], pinCables[i]);

        GPIO_registerInterrupt(portCables[i], IRQ_cables);
    }
}

void disableInterruptCables(){
    int i;
    for (i = 0; i < NUMCAVI; i++)
    {
        GPIO_unregisterInterrupt (portCables[i]);
        GPIO_disableInterrupt(portCables[i], pinCables[i]);
    }
}

void setupCables(uint_fast16_t* pins, uint_fast8_t* ports,uint8_t* conn){
    connectedCables = conn;

    int i;
    for(i=0;i<NUMCAVI;i++){
        pinCables[i] = pins[i];
        portCables[i] = ports[i];
        connectedCables[i] = 1;

        //enable interrupts
        GPIO_setDriveStrengthHigh(portCables[i], pinCables[i]);
        GPIO_setAsInputPinWithPullDownResistor(portCables[i], pinCables[i]);
        GPIO_interruptEdgeSelect(portCables[i], pinCables[i], GPIO_HIGH_TO_LOW_TRANSITION);
        GPIO_clearInterruptFlag(portCables[i], pinCables[i]);
    }

    disableInterruptCables();
}
