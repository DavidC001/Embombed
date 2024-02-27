#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <constants.h>
#include <dotMatrix.h>

uint32_t eUSCIPort; // the port of the EUSCI
uint_fast16_t matPin; // the pin of the chip select
uint_fast8_t matPort; // the port of the chip select

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
