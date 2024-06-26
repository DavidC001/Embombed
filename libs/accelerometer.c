#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ADC_subscription.h>
#include <constants.h>
#include <accelerometer.h>

/* ADC results buffer */
volatile int accResultsBuffer[2]; // x and z
volatile uint8_t accBuffer;
volatile uint8_t accValid;

uint8_t* accFacePunt;
int indexADCAcc;

int accDecisionTree(){
    if(accResultsBuffer[0]<=6164.5){
        if(accResultsBuffer[1]<=10232.5){
            if(accResultsBuffer[1]<=10020.5){
                if(accResultsBuffer[1]<=5607){
                    //impurità 0.49 albero, ritorno cosa è stato rilevato prima
                    return *accFacePunt;
                }else{
                    return GAME_SIMONSAYS;
                }
            }else{
                if(accResultsBuffer[0]<=5815){
                    return GAME_SIMONSAYS;
                }else{
                    return GAME_NOTNOT; //N.B. impurità 0.36 con SIMONSAYS
                }
            }
        }else{
            return GAME_NOTNOT;
        }
    }else{
        if(accResultsBuffer[1]<=10329){
            if(accResultsBuffer[0]<=10234){
                if(accResultsBuffer[1]<=6124.5){
                    return GAME_CODECRACKER;
                }else{
                    return GAME_NONE;
                }
            }else{
                if(accResultsBuffer[1]<=6696.5){
                    //impurità 0.462 ritorno il precedente
                    return *accFacePunt;
                }else{
                    return GAME_CABLES;
                }
            }
        }else{
            if(accResultsBuffer[1]<=10512.5){
                if(accResultsBuffer[0]<=9403.5){
                    //impurità 0.477 ritorno precedente
                    return *accFacePunt;
                }else{
                    return GAME_NOTNOT;//N.B. imp 0.27 con cavi
                }
            }else{
                return GAME_NOTNOT;
            }
        }
    }
}

void ADCAccelerometerIRQ(void)
{
    accResultsBuffer[0] = ADC14_getResult(ADC_MEM3);
    accResultsBuffer[1] = ADC14_getResult(ADC_MEM4);
    disableADC(indexADCAcc);

    uint8_t temp = accDecisionTree();
    if(temp==accBuffer){
        accValid++;
    }else{
        accValid=0;
        accBuffer=temp;
    }

    if(accValid>NUM_SAMPLES_ACC){
        *accFacePunt = accBuffer; //aggiunge delay, ma risultati più stabili
        accValid = 0;
    }
    enableADC(indexADCAcc);
}

void setupAccelerometer(uint8_t* currGameP)
{
    accFacePunt = currGameP;
    accValid = 0;
    accBuffer = GAME_NONE;

    /* Configures Pin 4.2 and 6.1 as ADC input */
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4, GPIO_PIN2,
                                               GPIO_TERTIARY_MODULE_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6, GPIO_PIN1,
                                               GPIO_TERTIARY_MODULE_FUNCTION);


    ADC14_configureConversionMemory(ADC_MEM3,
    ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                    ADC_INPUT_A14, ADC_NONDIFFERENTIAL_INPUTS);

    ADC14_configureConversionMemory(ADC_MEM4,
    ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                    ADC_INPUT_A11, ADC_NONDIFFERENTIAL_INPUTS);

    /* Enabling the interrupt when a conversion on channel 2 (end of sequence)
     *  is complete and enabling conversions */
    indexADCAcc = registerADC(ADCAccelerometerIRQ);
}

