#ifndef __DOTMATRIX_H__
#define __DOTMATRIX_H__

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#define MAT_OP_NOOP 0
#define MAT_OP_DECODEMODE 9
#define MAT_OP_INTENSITY 10
#define MAT_OP_SCANLIMIT 11
#define MAT_OP_SHUTDOWN 12
#define MAT_OP_DISPLAYTEST 15

uint32_t eUSCIPort;
uint_fast16_t matPin;
uint_fast8_t matPort;

const eUSCI_SPI_MasterConfig matrixConfig = {
                                 EUSCI_SPI_CLOCKSOURCE_SMCLK,
                                 48000000,
                                 10000000,
                                 EUSCI_SPI_MSB_FIRST,
                                 EUSCI_B_SPI_PHASE_DATA_CAPTURED_ONFIRST_CHANGED_ON_NEXT ,
                                 EUSCI_SPI_CLOCKPOLARITY_INACTIVITY_LOW,
                                 EUSCI_SPI_3PIN
};

void sendCmdMat(char cmd, char data){
    GPIO_setOutputLowOnPin(matPort, matPin);
    SPI_transmitData(eUSCIPort, cmd);
    while (SPI_isBusy(eUSCIPort) != EUSCI_SPI_NOT_BUSY);
    SPI_transmitData(eUSCIPort, data);
    while (SPI_isBusy(eUSCIPort) != EUSCI_SPI_NOT_BUSY);
    /*
    int i;
    for (i=0;i<8;i++){
        int k;
        if(cmd & 0b10000000){
            GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN6);
        } else {
            GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN6);
        }
        for(k=0;k<500;k++);
        GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN5);
        for(k=0;k<500;k++);
        GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN5);
        cmd=cmd<<1;
    }
    for (i=0;i<8;i++){
        int k;
        if(data & 0b10000000) {
            GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN6);
        } else {
            GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN6);
        }
        for(k=0;k<500;k++);
        GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN5);
        for(k=0;k<500;k++);
        GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN5);
        data=data<<1;
    }
    */
    GPIO_setOutputHighOnPin(matPort, matPin);
}

void sendMat(char mat[8]){
    int i;
    for(i=0;i<8;i++){
        sendCmdMat(i+1, mat[i]);
    }
}

void clearMat(){
    int i;
    for(i=0;i<8;i++){
        sendCmdMat(i+1, 0x00);
    }
}

void shutdownMat(){
    sendCmdMat(MAT_OP_SHUTDOWN, 0x00);
}

void turnOnMat(){
    sendCmdMat(MAT_OP_SHUTDOWN, 0x01);
}

/*
 * di default usa la EUSCI_B2
 * i pin sono:
 * P3.5 come clock
 * P3.6 con data
 *
 * il pin cs pu� essere configurato
 */
void initMatrix(uint_fast8_t csPort, uint_fast16_t csPin){
    matPort = csPort;
    matPin = csPin;
    eUSCIPort = EUSCI_B2_BASE;

    GPIO_setAsOutputPin(csPort, csPin);

    //GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN5 | GPIO_PIN6 | GPIO_PIN7);
    /* Selecting P1.5 P1.6 and P1.7 in SPI mode */
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P3, GPIO_PIN5 | GPIO_PIN6, GPIO_PRIMARY_MODULE_FUNCTION);


    SPI_initMaster(eUSCIPort, &matrixConfig);
    SPI_enableModule(eUSCIPort);

    GPIO_setOutputHighOnPin(matPort, matPin);
    //GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN5);

    sendCmdMat(MAT_OP_DISPLAYTEST, 0x00);
    sendCmdMat(MAT_OP_SCANLIMIT, 0x0f);
    sendCmdMat(MAT_OP_INTENSITY, 0x0e);
    sendCmdMat(MAT_OP_DECODEMODE, 0x00);
    clearMat();
    shutdownMat();
}


#endif