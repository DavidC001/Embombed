#ifndef __ADCSUB_H__
#def __ADCSUB_H__

/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#define MAX_FUNC 10


void (*intHandlers[MAX_FUNC])(void);
uint32_t ADCFuncIntNum[MAX_FUNC];
uint8_t ADCenabled[MAX_FUNC];
int ADCFuncIndex;

void disableADC(int index){
    ADCenabled[index] = 0;
}

void enableADC(int index){
    ADCenabled[index] = 1;
}

//usata quasi mai credo poco importa l'efficenza
void removeADC(int index){
    while(!ADC14_disableModule());
    int i;
    for(i=index;i<ADCFuncIndex-1;i++){
        intHandlers[i] = intHandlers[i+1];
        ADCenabled[i] = ADCenabled[i+1];
    }
    ADCFuncIndex--;
    ADC14_enableModule();
}


/*
 * register interrupt
 * ritorna l'indice dove viene salvato
 * non gestisce la mappatura della memoria che va fatta esternamente
 */
int registerADC(void (*intHandler)(void), uint32_t ADCIntNum){
    ADC14_enableInterrupt(ADCIntNum);

    ADCFuncIntNum[ADCFuncIndex] = ADCIntNum;
    intHandlers[ADCFuncIndex] = intHandler;
    ADCenabled[ADCFuncIndex] = 1;
    ADCFuncIndex++;

    if(ADCFuncIndex == 1) {
        ADC14_toggleConversionTrigger();
    }

    return (ADCFuncIndex-1);
}


/*
 * start automatic iteration
 * and clock to ADCOSC/64/8
 */
void setupADC(){
    ADCFuncIndex = 0;

    //init module
    ADC14_enableModule();
    ADC14_initModule(ADC_CLOCKSOURCE_ADCOSC, ADC_PREDIVIDER_64, ADC_DIVIDER_8,
                0);

    /* Enabling Interrupts */
    Interrupt_enableInterrupt(INT_ADC14);

    //automatic iteration
    ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);

    /* Triggering the start of the sample */
    ADC14_enableConversion();
}

void ADC14_IRQHandler(void)
{
    uint64_t status = ADC14_getEnabledInterruptStatus();
    ADC14_clearInterruptFlag(status);

    //printf("ADC: %d\n", ADC14_getResult(ADC_MEM0));

    int i;

    for(i=0;i<ADCFuncIndex;i++){
        if(ADCenabled[i] && (status & ADCFuncIntNum[i])){
            (*intHandlers[i])();
        }
    }
}

#endif