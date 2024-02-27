#include <LCD.h>
#include <string.h>
#include <buzzer.h>
#include <control.h>

volatile uint8_t numMistekesControl;
int* timeControl;
volatile uint8_t timeStepperControl;
uint8_t errLedPortsControl[3] = {GPIO_PORT_P3,GPIO_PORT_P4,GPIO_PORT_P5};
uint16_t errLedPinsControl[3] = {GPIO_PIN0,GPIO_PIN0,GPIO_PIN2};
int errFreqControl[6] = {150,150,150,100,100,100};
int correctFreqControl[6] = {300,300,300,400,400,400};
int victoryFreqControl[11] = {600,500,600,400,600,300,600,200,600,800,800};
int explosionFreqControl[20] = {300,300,300,300,0,200,200,200,200,200,200,0,100,100,100,100,100,100,100,100};

void setupTimer(){
    Timer_A_UpModeConfig config = {
                                       TIMER_A_CLOCKSOURCE_ACLK,
                                       TIMER_A_CLOCKSOURCE_DIVIDER_1,
                                       250,
                                       TIMER_A_TAIE_INTERRUPT_DISABLE,
                                       TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE,
                                       TIMER_A_SKIP_CLEAR
    };
    Timer_A_configureUpMode(TIMER_A1_BASE, &config);
}

void TA1_0_IRQHandler(void)
{
    /* clear the timer pending interrupt flag */
    Timer_A_clearCaptureCompareInterrupt(TIMER_A1_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_0);
    timeStepperControl++;

    if(timeStepperControl>=4-numMistekesControl){
        (*timeControl)--;
        timeStepperControl = 0;
        //send time to LCD
        int minutes = *timeControl / 60;
        int seconds = *timeControl % 60;
        char time[5];
        time[0] = minutes / 10 + '0';
        time[1] = minutes % 10 + '0';
        time[2] = ':';
        time[3] = seconds / 10 + '0';
        time[4] = seconds % 10 + '0';
        displayLCD(ROW_2, time, 5);

        if(*timeControl == 0){
            Interrupt_disableInterrupt(INT_TA1_0);
            while(numMistekesControl!=3){
                addMistakeControl();
            }
        }
    }
}

void addMistakeControl(){
    GPIO_setOutputHighOnPin(errLedPortsControl[numMistekesControl], errLedPinsControl[numMistekesControl]);

    numMistekesControl++;
    if(numMistekesControl==3){
        Interrupt_disableInterrupt(INT_TA1_0);
        *timeControl = 0;
    }

    sendBuzzer(errFreqControl, 6);
    speedUpMusic(1+0.8*numMistekesControl);
}

void correctFeedBackControl(){
    sendBuzzer(correctFreqControl, 6);
}

void victoryFeedBackControl(){
    sendBuzzer(victoryFreqControl, 11);
}

void explosionSound(){
    sendBuzzer(explosionFreqControl, 20);
    stopMusic();
}

void startTimerControl(){
    Interrupt_enableInterrupt(INT_TA1_0);

    /* Starting the Timer_A0 in up mode */
    Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_UP_MODE);
}

void setupControl(int* time, char* SN){
    timeControl = time;
    numMistekesControl = 0;
    timeStepperControl = 0;

    /*
     * inizializzo LCD
     */
    lcdConfig LCD = {
                           GPIO_PORT_P6, GPIO_PIN5,
                           GPIO_PORT_P6, GPIO_PIN4,
                           GPIO_PORT_P3, GPIO_PIN2,
                           GPIO_PORT_P3, GPIO_PIN3,
                           GPIO_PORT_P4, GPIO_PIN1,
                           GPIO_PORT_P4, GPIO_PIN3
                         };
    setupLCD(LCD);

    //send SN to LCD
    displayLCD(ROW_1, SN, strlen(SN));
    //send time to LCD
    int minutes = *timeControl / 60;
    int seconds = *timeControl % 60;
    char timeS[5];
    timeS[0] = minutes / 10 + '0';
    timeS[1] = minutes % 10 + '0';
    timeS[2] = ':';
    timeS[3] = seconds / 10 + '0';
    timeS[4] = seconds % 10 + '0';
    displayLCD(ROW_2, timeS, 5);

    /*
     * inizializzo timer
     */
    setupTimer();

    /*
     * inizializzo LEDs
     */
    int i;
    for(i=0;i<3;i++){
        GPIO_setAsOutputPin(errLedPortsControl[i], errLedPinsControl[i]);
        GPIO_setOutputLowOnPin(errLedPortsControl[i], errLedPinsControl[i]);
    }

    /*
     * inizializzo buzzer
    */
    setupBuzzer();
}
