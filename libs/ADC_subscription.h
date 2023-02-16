#ifndef __ADCSUB_H__
#define __ADCSUB_H__

/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <constants.h>


void (*ADCIntHandlers[MAX_FUNC])(void);
uint8_t ADCenabled[MAX_FUNC];
int ADCFuncIndex;

/**
 * @brief disable ADC handler
 * 
 * @param index the index of the handler to disable
 * 
 * @return None
 * 
 * @note the handler is not removed from the list, it is just disabled
 */
void disableADC(int index){
    ADCenabled[index] = 0;
}

/**
 * @brief enable ADC handler
 * 
 * @param index the index of the handler to enable
 * 
 * @return None
 * 
 * @note the handler is not removed from the list, it is just enabled
 */
void enableADC(int index){
    ADCenabled[index] = 1;
}

//usata credo mai poco importa l'efficenza
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


/**
 * @brief registerADC
 * @param intHandler A pointer to the interrupt handler function.
 * @return int
 * 
 * @note register interrupt
 * return the index where it is saved
 * does not handle the mapping of the memory that must be done externally
 */
int registerADC(void (*intHandler)(void)){
    /* Triggering the start of the sample */

    ADCIntHandlers[ADCFuncIndex] = intHandler;
    ADCenabled[ADCFuncIndex] = 1;
    ADCFuncIndex++;

    return (ADCFuncIndex-1);
}

/**
 * @brief start ADC conversion
 * @return None
 * 
 * @note enable conversion and interrupt
 * enable automatic iteration
 * enable conversion trigger
 * from now on the interrupt will be called
 */
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

/**
 * @brief setup ADC
 * @return None
 * 
 * @note start automatic iteration
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
