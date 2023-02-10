#ifndef __BUZZER_H__
#define __BUZZER_H__

/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <constants.h>
#include <notes.h>

/* Timer_A Compare Configuration Parameter  (PWM) */
Timer_A_CompareModeConfig compareConfig_PWM = {
TIMER_A_CAPTURECOMPARE_REGISTER_4,                  // Use CCR3
        TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE,   // Disable CCR interrupt
        TIMER_A_OUTPUTMODE_TOGGLE_SET,              // Toggle output but
        0                                           // compareValue       
        };

/* Timer_A Up Configuration Parameter */
Timer_A_UpModeConfig upConfig = {
        TIMER_A_CLOCKSOURCE_SMCLK,              // SMCLK = 48 MhZ
        TIMER_A_CLOCKSOURCE_DIVIDER_12,         // SMCLK/12 = 4 MhZ
        4000,                                   //timerPeriod                         
        TIMER_A_TAIE_INTERRUPT_DISABLE,         // Disable Timer interrupt
        TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE,    // Disable CCR0 interrupt
        TIMER_A_DO_CLEAR                        // Clear value
        };

int notesMusic[NUM_NOTES] = {
  0, G5,
  A5, A5S,
  A5, F5,
  A5, G5,
  0, G5,
  A5, A5S,
  C6, A5S,

  A5, G5, //8
  0, G5,
  A5, A5S,
  A5, F5,
  A5, G5,
  D6, 0, C6,
  0, A5S,

  A5, A5S, C6, //15
  F6, 0, 0,
  G5, D5, D6, D5, C6, D5, A5S, D5,
  A5, D5, A5S, D5, A5, D5, G5, D5,
  A5, D5, A5S, D5, C6, D5, A5S, D5,

  A5, D5, F5, D5, A5, D5, G5, D5, //20
  G5, D5, D6, D5, C6, D5, A5S, D5,
  A5, D5, A5S, D5, A5, D5, A5S, D5,
  A5, D5, A5S, D5, C6, D5, A5S, D5,
  A5, D5, F5, D5, A5, D5, G5, D5,

  G5, D5, D6, D5, C6, D5, A5S, D5, //25
  A5, D5, A5S, D5, A5, D5, G5, D5,
  A5, D5, A5S, D5, C6, D5, A5S, D5,
  A5, D5, F5, D5, A5, D5, G5, D5,
  A5S, D5, D6, D5, C6, D5, A5S, D5,

  A5, D5, A5S, D5, A5, D5, G5, D5,
  A5, D5, A5S, D5, C6, D5, A5S, D5,
  A5, D5, F5, D5, A5, D5, G5, D5,
  C6, C6, F6, D6, 0, 0,
  0, C6, A5S,

  C6,  F6, D6, //35
  C6, A5S,
  C6, F6, D6, 0, 0,
  0, C6, D6,
  D6S, F6,

  D6, 0, D6S, 0, //40
  C6, F6, D6, 0, 0,
  0, C6, A5S,
  C6,  F6, D6,
  C6, A5S,

  C6, F6, D6, 0, 0, //45
  0, C6, D6,
  D6S, F6,
  D5, F5S, F5, A5,
  A5, G5,

  A5, G5, //50
  A5, G5,
  A5S, A5, G5, F5,
  A5, G5, D5,
  A5, G5, D5,
  A5, G5, D5,

  A5S, C6, A5, A5S,
  G5, D5, D6, D5, C6, D5, A5S, D5,//56 //r
  A5, D5, A5S, D5, A5, D5, G5, D5,
  A5, D5, A5S, D5, C6, D5, A5S, D5,
  A5, D5, F5, D5, A5, D5, G5, D5,

  G5, D5, D6, D5, C6, D5, A5S, D5,//61
  A5, D5, A5S, D5, A5, D5, A5S, D5,
  A5, D5, A5S, D5, C6, D5, A5S, D5,
  A5, D5, F5, D5, A5, D5, G5, D5,
  G5, D5, D6, D5, C6, D5, A5S, D5,

  A5, D5, A5S, D5, A5, D5, G5, D5,//66
  A5, D5, A5S, D5, C6, D5, A5S, D5,
  A5, D5, F5, D5, A5, D5, G5, D5,
  A5S, D5, D6, D5, C6, D5, A5S, D5,
  A5, D5, A5S, D5, A5, D5, G5, D5,

  A5, D5, A5S, D5, C6, D5, A5S, D5,//71 //
  A5, D5, F5, D5, A5, G5, A5, A5S, C6,
  D6, G5, A5S, G5, C6, G5, D6, G5,
  C6, G5, A5, G5, F6, G5, D6, D5S,
  D6, 0,

  C5, 0, A4, A4S, C5, //76
  D6, G4, A4S, G4, C5, G4, D6, G4,
  C6, F4, A4, F4, F5, F4, D6, D4S,
  D6, 0, E4, F4,
  
  //change of key B Major A# C# D# F# G#
  G4S, 0, A4S, 0,

  D5S, G4S, B4, G4S, C5S, G4S, D5S, G4S, //81
  C5S, F4S, A4S, F4S, F5S, F4S, D5S, E5,
  D5, 0,
  C5S, 0, A4S,  B4, C5S,
  D5S, G4S, B4, G4S, C5S, G4S, D5S, G4S,
  
  C5S, F4S, A4S, F4S, F5S, F4S, D5S, E5,
  D5S, 0, D5S,  E5,
  F5S, C5S, E5, C4S, D5S, E5, G5, A5S,
  G5S, D5S, D6S, D5S, C6S, D5S, B5, D5S,

  A5S, D5S, B5, D5S, A5S, D5S, G5S, D5S, //90
  A5S, D5S, B5, D5S, C6S, D5S, B5, D5S,
  A5S, D5S, F5S, D5S, A5S, D5S, G5S, D5S,
  G5S, D5S, D6S, D5S, C6S, D5S, B5, D5S,

  A5S, D5S, B5, D5S, A5S, D5S, G5S, D5S,//94
  A5S, D5S, B5, D5S, C6S, D5S, B5, D5S,
  A5S, D5S, F5S, D5S, A5S, D5S, G5S, D5S,
  G5S, D5S, D6S, D5S, C6S, D5S, B5, D5S,

  A5S, D5S, B5, D5S, A5S, D5S, G5S, D5S,//98
  A5S, D5S, B5, D5S, C6S, D5S, B5, D5S,
  A5S, D5S, F5S, D5S, A5S, D5S, G5S, D5S,
  G5S, D5S, D6S, D5S, C6S, D5S, B5, D5S,

  A5S, D5S, B5, D5S, A5S, D5S, G5S, D5S,//102
  A5S, D5S, B5, D5S, C6S, D5S, B5, D5S,
  A5S, D5S, F5S, D5S, A5S, D5S, G5S, D5S,
  
  C6S, F6S, D6S, 0, 0, //107
  0, C6S, B5,
  C6S, F6S, D6S,
  C6S, B5,
  C6S, F6S, D6S, 0, 0,
  0, C6S, B5,
  E6, F6,
  
  D6S, 0, E6, 0, 0, //112
  C6S, F6S, D6S, 0, 0,
  0, C6S, B5,
  C6S, F6S, D6S,
  C6S, B5,
  
  C6S, F6S, D6S, 0, 0, //117
  0, C5S, D5S,
  E5, F5,
  D5S, G5, G5S, A5S,
  A5S, G5S,

  A5S, G5S,//122
  A5S, G5S,
  B6, A5S, G5S, F5S,
  A5S, G6S, D5S,
  A5S, G6S, D5S,
  A5S, G6S, D5S,

  B5, C6S, A5S, B5,//128
  G5S, 0, 0
};
int durationMusic[NUM_NOTES] = {
     8 ,  8 ,
   8 ,  8 ,
   8 ,  8 ,
   8 ,  8 ,
   8 ,  8 ,
   8 ,  8 ,
   8 ,  8 ,

   8 ,  8 , //8
   8 ,  8 ,
   8 ,  8 ,
   8 ,  8 ,
   8 ,  8 ,
   8 ,  4,  4,
   8 ,  8 ,

   8 ,  4,  4, //15
   4,  4,  8 ,
   2,  2,  2,  2,  2,  2,  2,  2,
   2,  2,  2,  2,  2,  2,  2,  2,
   2,  2,  2,  2,  2,  2,  2,  2,

   2,  2,  2,  2,  2,  2,  2,  2, //20
   2,  2,  2,  2,  2,  2,  2,  2,
   2,  2,  2,  2,  2,  2,  2,  2,
   2,  2,  2,  2,  2,  2,  2,  2,
   2,  2,  2,  2,  2,  2,  2,  2,

   2,  2,  2,  2,  2,  2,  2,  2, //25
   2,  2,  2,  2,  2,  2,  2,  2,
   2,  2,  2,  2,  2,  2,  2,  2,
   2,  2,  2,  2,  2,  2,  2,  2,
   2,  2,  2,  2,  2,  2,  2,  2,

   2,  2,  2,  2,  2,  2,  2,  2,
   2,  2,  2,  2,  2,  2,  2,  2,
   2,  2,  2,  2,  2,  2,  2,  2,
   2,  2,  2,  4,  2,  4,
   8 ,  2,  2,

   6,   6,  12, //35
   4,  4,
   4,  2,  4,  2,  4,
   8 ,  4,  4,
   6,  6,

   6,  2,  4,  4, //40
   4,  2,  4,  2,  4,
   8 ,  4,  4,
   6,   6,  12,
   4,  4,

   4,  2,  4,  2,  4, //45
   8 ,  4,  4,
   6,  6,
   4,  4,  4,  4,
   6,  12,

   6,  12, //50
   6,  12,
   4,  4,  4,  4,
   6,  6,  4,
   6,  6,  4,
   6,  6,  4,

   8 ,  8 ,  8 ,  8 ,
  2, 2, 2, 2, 2, 2, 2, 2,//56 //r
  2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2,

  2, 2, 2, 2, 2, 2, 2, 2,//61
  2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2,

  2, 2, 2, 2, 2, 2, 2, 2,//66
  2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2,

  2, 2, 2, 2, 2, 2, 2, 2,//71 //
   2,  2,  2,  2,  4,  1,  1,  1,  1,
   2,  2,  2,  2,  2,  2,  2,  2,
   2,  2,  2,  2,  2,  2,  2,  2,
   8 ,  8 ,

   4,  4,  3,  3,  2, //76
   2,  2,  2,  2,  2,  2,  2,  2,
   2,  2,  2,  2,  2,  2,  2,  2,
   2,  4,  2,  2,
  
  //change of key B Major A# C# D# F# G#
   4,  4,  4,  4,

   2,  2,  2,  2,  2,  2,  2,  2, //81
   2,  2,  2,  2,  2,  2,  2,  2,
   8 ,  8 ,
   4,  4,  3,   3,  2,
   2,  2,  2,  2,  2,  2,  2,  2,
  
   2,  2,  2,  2,  2,  2,  2,  2,
   8 ,  4,  2,   2,
   2,  2,  2,  2,  2,  2,  2,  2,
   2,  2,  2,  2,  2,  2,  2,  2,

   2,  2,  2,  2,  2,  2,  2,  2, //90
   2,  2,  2,  2,  2,  2,  2,  2,
   2,  2,  2,  2,  2,  2,  2,  2,
   2,  2,  2,  2,  2,  2,  2,  2,

   2,  2,  2,  2,  2,  2,  2,  2,//94
   2,  2,  2,  2,  2,  2,  2,  2,
   2,  2,  2,  2,  2,  2,  2,  2,
   2,  2,  2,  2,  2,  2,  2,  2,

   2,  2,  2,  2,  2,  2,  2,  2,//98
   2,  2,  2,  2,  2,  2,  2,  2,
   2,  2,  2,  2,  2,  2,  2,  2,
   2,  2,  2,  2,  2,  2,  2,  2,

   2,  2,  2,  2,  2,  2,  2,  2,//102
   2,  2,  2,  2,  2,  2,  2,  2,
   2,  2,  2,  2,  2,  2,  2,  2,
  
   4,  2,  4, 2, 4, //107
  8 ,  4,  4,
  6,  6,  12,
   4,  4,
   4,  2,  4, 2, 4,
  8 ,  4,  4,
   6,  6,
  
  6, 2, 4, 2, 2, //112
   4,  2,  4, 2, 4,
  8 ,  4,  4,
  6,  6,  12,
   4,  4,
  
   4,  2,  4, 2, 4, //117
  8 ,  4,  4,
   6,  6,
   4,  4,  4,  4,
   6,  6,

   6,  6,//122
   6,  6,
   4,  4,  4,  4,
  6,  6,  4,
  6,  6,  4,
  6,  6,  4,

  4,  4,  4,  4,//128
  4, 4,  2
};
int currentNote = 0;
int currentNoteDuration = 0;
uint8_t pauseMusic = 0;

void startMusic(){
    //set up A2 timer as metronome
    Timer_A_UpModeConfig config = {
            TIMER_A_CLOCKSOURCE_ACLK,
            TIMER_A_CLOCKSOURCE_DIVIDER_1,
            60000/(TEMPO*GRID),
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
        if(currentNoteDuration >= GRID/durationMusic[currentNote]){
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
