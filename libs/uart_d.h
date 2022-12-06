#ifndef __UARTCUSTOM_H__
#define __UARTCUSTOM_H__

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <constants.h>
#include <stdlib.h>

const eUSCI_UART_ConfigV1 UARTConfig=
    {
            EUSCI_A_UART_CLOCKSOURCE_SMCLK,          // SMCLK Clock Source
            26,                                      // BRDIV = 13
            0,                                       // UCxBRF = 0
            111,                                      // UCxBRS = 37
            EUSCI_A_UART_NO_PARITY,                  // No Parity
            EUSCI_A_UART_LSB_FIRST,                  // LSB First
            EUSCI_A_UART_ONE_STOP_BIT,               // One stop bit
            EUSCI_A_UART_MODE,                       // UART mode
            EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION,  // Oversampling
            EUSCI_A_UART_8_BIT_LEN                  // 8 bit data length
    };

volatile int UARTPacketSize;
volatile int UARTReceivedSize;
volatile int UARTHeaderSize;
volatile int UARTReceivedHeaderSize;
int* UARTNotifySize;
char** UARTCollectedMessage;


void setupUART(char** message,int* size, int headerSize){
    UARTPacketSize = 0;
    UARTReceivedSize = 0;
    UARTHeaderSize = headerSize;
    UARTReceivedHeaderSize = 0;

    UARTNotifySize = size;
    *UARTNotifySize = 0;

    UARTCollectedMessage = message;
    *UARTCollectedMessage = NULL;


    /* Selecting P1.2 and P1.3 in UART mode */
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
                GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

    /*
    //Setting DCO to 48MHz 
    //Set the core voltage level to VCORE1 
    PCM_setCoreVoltageLevel(PCM_VCORE1);
    //Set 2 flash wait states for Flash bank 0 and 1
    FlashCtl_setWaitState(FLASH_BANK0, 2);
    FlashCtl_setWaitState(FLASH_BANK1, 2);
    //Initializes Clock System
    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48);
    */

    /* Configuring UART Module */
    UART_initModule(EUSCI_A0_BASE, &UARTConfig);

    /* Enable UART module */
    UART_enableModule(EUSCI_A0_BASE);

    /* Enabling interrupts */
    UART_enableInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
    Interrupt_enableInterrupt(INT_EUSCIA0);
}

void sendUART(char* data, unsigned int size){
    int i;

    for(i=0;i<UARTHeaderSize;i++){
        UART_transmitData(EUSCI_A0_BASE, size>>(i*8));
    }

    for(i=0;i<size;i++){
        UART_transmitData(EUSCI_A0_BASE, data[i]);
    }
}

//se libero raccolgo il messaggio, altrimenti rifiuto la connessione
void EUSCIA0_IRQHandler(void)
{
    uint32_t status = MAP_UART_getEnabledInterruptStatus(EUSCI_A0_BASE);

    MAP_UART_clearInterruptFlag(EUSCI_A0_BASE, status);

    if(status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
    {
        if(UARTReceivedHeaderSize != UARTHeaderSize){
            //add value shifted
            UARTPacketSize = UARTPacketSize +
                    (UART_receiveData(EUSCI_A0_BASE) << (UARTReceivedHeaderSize*8));

            //if ended header allocate message vector
            ++UARTReceivedHeaderSize;
            if(UARTReceivedHeaderSize == UARTHeaderSize){
                if(*UARTNotifySize==0) {
                    *UARTCollectedMessage = (char *)malloc(sizeof(char) * UARTPacketSize);
                    UART_transmitData(EUSCI_A0_BASE, 'B'); //begin transission
                } else {
                    UARTReceivedHeaderSize = 0;
                    UARTPacketSize = 0;
                    UART_transmitData(EUSCI_A0_BASE, 'R'); //reset transission
                }
            }
        }else{
            //save char if buffer empty
            (*UARTCollectedMessage)[UARTReceivedSize] = UART_receiveData(EUSCI_A0_BASE);
            ++UARTReceivedSize;

            //if the entire message has been received then send to callback the message
            //and reset state
            if(UARTReceivedSize == UARTPacketSize){
                *UARTNotifySize  = UARTPacketSize;

                UARTReceivedHeaderSize = 0;
                UARTReceivedSize = 0;
                UARTPacketSize = 0;
            }
        }
    }

}


#endif
