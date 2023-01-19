#ifndef __BUZZER_H__
#define __BUZZER_H__

/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <constants.h>

/* Timer_A Compare Configuration Parameter  (PWM) */
Timer_A_CompareModeConfig compareConfig_PWM = {
TIMER_A_CAPTURECOMPARE_REGISTER_4,          // Use CCR3
        TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE,   // Disable CCR interrupt
        TIMER_A_OUTPUTMODE_TOGGLE_SET,              // Toggle output but
        0                                    
        };

/* Timer_A Up Configuration Parameter */
Timer_A_UpModeConfig upConfig = {
        TIMER_A_CLOCKSOURCE_SMCLK,               // SMCLK = 48 MhZ
        TIMER_A_CLOCKSOURCE_DIVIDER_12,         // SMCLK/12 = 4 MhZ
        4000,                                   
        TIMER_A_TAIE_INTERRUPT_DISABLE,         // Disable Timer interrupt
        TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE,    // Disable CCR0 interrupt
        TIMER_A_DO_CLEAR                        // Clear value
        };


const int notesMusic[NUM_NOTES]={110,82,104,69,73,69};
//uses block size of site:https://onlinesequencer.net/ with 110 bpm (around 125ms)
const int durationMusic[NUM_NOTES]={3,3,4,2,2,2};
int currentNote = 0;
int currentNoteDuration = 0;
int pauseMusic = 0;

void startMusic(){
    //set up A2 timer as metronome
    Timer_A_UpModeConfig config = {
            TIMER_A_CLOCKSOURCE_ACLK,
            TIMER_A_CLOCKSOURCE_DIVIDER_1,
            125,
            TIMER_A_TAIE_INTERRUPT_DISABLE,
            TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE,
            TIMER_A_SKIP_CLEAR
    };
    Timer_A_configureUpMode(TIMER_A2_BASE, &config);
    Interrupt_enableInterrupt(INT_TA2_0);
    Timer_A_startCounter(TIMER_A2_BASE, TIMER_A_UP_MODE);
}

void stopMusic(){
    Timer_A_stopTimer(TIMER_A2_BASE);
    Timer_A_stopTimer(TIMER_A0_BASE);
    pauseMusic = 1;
}

void TA2_0_IRQHandler(void)
{
    // clear the timer pending interrupt flag
    Timer_A_clearCaptureCompareInterrupt(TIMER_A2_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);
    currentNoteDuration++;
    if(!pauseMusic){
        if(currentNoteDuration >= durationMusic[currentNote]){
            currentNote++;
            currentNoteDuration = 0;
            if(currentNote == NUM_NOTES){
                currentNote = 0;
            }
            int period = 4000000/notesMusic[currentNote];
            upConfig.timerPeriod = period;
            Timer_A_configureUpMode(TIMER_A0_BASE, &upConfig);
            Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);
            Timer_A_setCompareValue(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_4, period/2);
        }
    }
}


/*
 * send frequency (in Hz) played for 20ms to buzzer
*/
void sendBuzzer(int freq[], int size){
    //stop music
    pauseMusic = 1;
    int period, i;

    //20ms of silence
    Timer_A_setCompareValue(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_4, 0);
    __delay_cycles(960000);

    //play sound
    for(i = 0; i < size; i++){
        period = 4000000/freq[i];
        upConfig.timerPeriod = period;
        Timer_A_configureUpMode(TIMER_A0_BASE, &upConfig);
        Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);
        Timer_A_setCompareValue(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_4, period/2);
        __delay_cycles(960000);
    }

    //20ms of silence
    Timer_A_setCompareValue(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_4, 0);
    __delay_cycles(960000);
    //start music again
    pauseMusic = 0;
}

void setupBuzzer(){
    /* Configures P2.7 to PM_TA0.4 for using Timer PWM to control the buzzer */
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2, GPIO_PIN7,
    GPIO_PRIMARY_MODULE_FUNCTION);

    /* Initialize compare registers to generate PWM */
    Timer_A_initCompare(TIMER_A0_BASE, &compareConfig_PWM); // For P2.7

    startMusic();
}

#endif
