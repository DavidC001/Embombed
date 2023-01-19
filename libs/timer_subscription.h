#ifndef __TIMERSUB_H__
#define __TIMERSUB_H__

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <constants.h>

void(*TimerIntHandlers[MAX_FUNC])(void);
uint8_t TimerEnabled[MAX_FUNC];
int TimerFuncIndex;

void disableTimer(int index){
    TimerEnabled[index] = 0;
}

void enableTimer(int index){
    TimerEnabled[index] = 1;
}

/*
 * register interrupt
 * ritorna l'indice dove viene salvato
 * non gestisce la mappatura della memoria che va fatta esternamente
 */
int registerTimer(void (*intHandler)(void)){

    TimerIntHandlers[TimerFuncIndex] = intHandler;
    TimerEnabled[TimerFuncIndex] = 1;
    TimerFuncIndex++;

    return (TimerFuncIndex-1);
}


void setupTimer(){
    TimerFuncIndex = 0;

    Timer_A_UpModeConfig config = {
                                       TIMER_A_CLOCKSOURCE_ACLK,
                                       TIMER_A_CLOCKSOURCE_DIVIDER_1,
                                       250,
                                       TIMER_A_TAIE_INTERRUPT_DISABLE,
                                       TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE,
                                       TIMER_A_SKIP_CLEAR
    };
    Timer_A_configureUpMode(TIMER_A1_BASE, &config);
    Interrupt_enableInterrupt(INT_TA1_0);

    /* Starting the Timer_A0 in up mode */
    Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_UP_MODE);
}

void TA1_0_IRQHandler(void)
{
    /* clear the timer pending interrupt flag */
    Timer_A_clearCaptureCompareInterrupt(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);

    int i;
    for(i=0;i<TimerFuncIndex;i++){
        if(TimerEnabled[i]){
            (*TimerIntHandlers[i])();
        }
    }
}


#endif
