#ifndef __ADCSUB_H__
#define __ADCSUB_H__

/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <constants.h>


void (*ADCIntHandlers[MAX_FUNC])(void);
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
        ADCIntHandlers[i] = ADCIntHandlers[i+1];
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
int registerADC(void (*intHandler)(void)){
    /* Triggering the start of the sample */

    ADCIntHandlers[ADCFuncIndex] = intHandler;
    ADCenabled[ADCFuncIndex] = 1;
    ADCFuncIndex++;

    return (ADCFuncIndex-1);
}

void enableConvADC(){
    ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM4, true);
    ADC14_enableInterrupt(ADC_INT4);
    /* Enabling Interrupts */
    Interrupt_enableInterrupt(INT_ADC14);
    //automatic iteration
    ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);
    ADC14_enableConversion();
    ADC14_toggleConversionTrigger();
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
}

void ADC14_IRQHandler(void)
{
    uint64_t status = ADC14_getEnabledInterruptStatus();
    ADC14_clearInterruptFlag(status);

    int i;

    for(i=0;i<ADCFuncIndex;i++){
        if(ADCenabled[i]){
            (*ADCIntHandlers[i])();
        }
    }
}

#endif
