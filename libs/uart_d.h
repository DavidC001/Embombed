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
char* UARTCollectedMessage;

/**
 * @brief setup UART
 * 
 * @param message address of the array where the message will be stored
 * @param size address of the variable where the size of the message will be stored
 * @param headerSize size of the header
 * 
 * @return None
 *
 * setup the UART module to receive a message with a header of size headerSize
 * the message will be stored in the array pointed by message
 * the size of the message will be stored in the variable pointed by size
 * the header is composed of headerSize bytes
 * 
 * for more details on the """protocol""" see the development_process_spec.pdf file
 */
void setupUART(char* message,int* size, int headerSize){
    UARTPacketSize = 0;
    UARTReceivedSize = 0;
    UARTHeaderSize = headerSize;
    UARTReceivedHeaderSize = 0;

    UARTNotifySize = size;
    *UARTNotifySize = 0;

    UARTCollectedMessage = message;


    /* Selecting P1.2 and P1.3 in UART mode */
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
                GPIO_PIN2 | GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);

    /* Configuring UART Module */
    UART_initModule(EUSCI_A0_BASE, &UARTConfig);

    /* Enable UART module */
    UART_enableModule(EUSCI_A0_BASE);

    /* Enabling interrupts */
    UART_enableInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
    Interrupt_enableInterrupt(INT_EUSCIA0);
}

/**
 * @brief send a message through UART
 * 
 * @param data pointer to the array containing the message
 * @param size size of the message
 * 
 * @return None
 * 
 * send a message through UART
 * the message is composed of a header of size UARTHeaderSize (defined during setup) and a body of size size
 * the header is composed of UARTHeaderSize bytes
 * the body is composed of size bytes
 */
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
                    UART_transmitData(EUSCI_A0_BASE, 'B'); //begin transission
                } else {
                    UARTReceivedHeaderSize = 0;
                    UARTPacketSize = 0;
                    UART_transmitData(EUSCI_A0_BASE, 'R'); //reset transission
                }
            }
        }else{
            //save char if buffer empty
            UARTCollectedMessage[UARTReceivedSize] = UART_receiveData(EUSCI_A0_BASE);
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
