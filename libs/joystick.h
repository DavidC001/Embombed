#ifndef __JOYSTICK_H__
#define __JOYSTICK_H__

#include <ADC_subscription.h>

char* puntDirectionJoystick;
int indexJoystickADC;
uint8_t releasedJoystick = 1;

void ADCJoystickIRQ(){
    if(*puntDirectionJoystick==0){
        float x = ADC14_getResult(ADC_MEM1)-300; //account for shift
        x /= 16083.0;
        x -= 0.5;
        float y = ADC14_getResult(ADC_MEM2);
        y /= 16383.0;
        y -= 0.5;

        float d = x*x+y*y;
        if(d>0.1089) {
            if(releasedJoystick){
                if(x>0.30){
                    *puntDirectionJoystick = 'R';
                }
                if(x<-0.3){
                    *puntDirectionJoystick = 'L';
                }
                if(y>0.3){
                    *puntDirectionJoystick = 'U';
                }
                if(y<-0.3){
                    *puntDirectionJoystick = 'D';
                }
                releasedJoystick = 0;
            }
        }else{
            releasedJoystick = 1;
        }
    }
}

/**
 * @brief disable interrupt on joystick
 * @return None
 * 
 * disable interrupt on ADC
 */
void disableInterruptJoystick(){
    disableADC(indexJoystickADC);
}

/**
 * @brief enable interrupt on joystick
 * @return None
 * 
 * enable interrupt on ADC
 */
void enableInterruptJoystick(){
    enableADC(indexJoystickADC);
}

/**
 * @brief setup joystick
 * @param punt pointer to variable that will contain the direction of the joystick
 * @return None
 * 
 * setup joystick
 * configures pin 6.0 and 4.4 as ADC input
 * configures ADC memory 1 and 2
 * registers ADCJoystickIRQ as interrupt handler
 */
void setupJoystick(char* punt){
    puntDirectionJoystick = punt;
    /* Configures Pin 6.0 and 4.4 as ADC input */
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6, GPIO_PIN0, GPIO_TERTIARY_MODULE_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4, GPIO_PIN4, GPIO_TERTIARY_MODULE_FUNCTION);


    /* Configuring ADC Memory (ADC_MEM1 - ADC_MEM2 (A15, A9)  with repeat)
     * with internal 2.5v reference */
    ADC14_configureConversionMemory(ADC_MEM1,
                    ADC_VREFPOS_AVCC_VREFNEG_VSS,
                    ADC_INPUT_A15, ADC_NONDIFFERENTIAL_INPUTS);
    ADC14_configureConversionMemory(ADC_MEM2,
                    ADC_VREFPOS_AVCC_VREFNEG_VSS,
                    ADC_INPUT_A9, ADC_NONDIFFERENTIAL_INPUTS);

    indexJoystickADC = registerADC(ADCJoystickIRQ);
    disableInterruptJoystick();
}

#endif
