#ifndef __NOTNOTGAME_H__
#define __NOTNOTGAME_H__

#ifndef TESTING
#include <joystick.h>
#include <control.h>
#else
#include "../libs/emulators/joystick.h"
#include "../libs/emulators/control.h"
#endif

#include <constants.h>
#include <game_parser.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <string.h>

#ifndef TESTING
#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/grlib/grlib.h>
#include <LcdDriver/Crystalfontz128x128_ST7735.h>
#include <LcdDriver/HAL_MSP_EXP432P401R_Crystalfontz128x128_ST7735.h>
//TODO sistema linking
#include <LcdDriver/Crystalfontz128x128_ST7735.c>
#include <LcdDriver/HAL_MSP_EXP432P401R_Crystalfontz128x128_ST7735.c>
/* Graphic library context */
Graphics_Context g_sContext;
uint8_t sec;

void _graphicsInit()
{
    /* Initializes display */
    Crystalfontz128x128_Init();

    /* Set default screen orientation */
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);

    /* Initializes graphics context */
    Graphics_initContext(&g_sContext, &g_sCrystalfontz128x128,
                         &g_sCrystalfontz128x128_funcs);
    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_RED);
    Graphics_setBackgroundColor(&g_sContext, GRAPHICS_COLOR_WHITE);
    GrContextFontSet(&g_sContext, &g_sFontFixed6x8);
    Graphics_clearDisplay(&g_sContext);
}
#endif

notNot* notNotGameNN; // The notNot game info
int* NNTime; // The time left in the game
uint8_t* currGameNN; // The current game

volatile char directionJoystickNN; // The direction of the joystick
int turnNN; // The turn of the game
int sSecondsNN; // Time at turn start
int turnTimeNN; // Time for the turn


void drawLCDNN(){
    //clear the screen
    #ifndef TESTING
    Graphics_clearDisplay(&g_sContext);
    #endif
    if(turnNN==notNotGameNN->num_turni){
        #ifndef TESTING
        Graphics_drawStringCentered(&g_sContext, (int8_t *) "YOU WON!", 8, 64, 64,
                                    OPAQUE_TEXT);
        #else
        printf("YOU WON!\n");
        #endif
        return;
    }
    #ifndef TESTING
    Graphics_drawStringCentered(&g_sContext, (int8_t *) notNotGameNN->messaggi[turnNN], strlen(notNotGameNN->messaggi[turnNN]), 64, 64,
                                OPAQUE_TEXT);
    //remaining time bar
    //calculate the remaining time
    int remainingTime = turnTimeNN-(sSecondsNN-*NNTime);
    if(remainingTime<=0){
        //write the time is up
        Graphics_drawStringCentered(&g_sContext, (int8_t *) "TIME IS UP!", 11, 64, 80,
                                    OPAQUE_TEXT);
    }else{
        //calculate the bar length
        int barLength = remainingTime*92*4/turnTimeNN;
        //draw the bars
        if(barLength>0) Graphics_drawLineH(&g_sContext, 18, 18+min(92,barLength), 110); //draw bottom bar
        if(barLength>92) Graphics_drawLineV(&g_sContext, 110, 110, 110-min(92,barLength-92)); //draw right bar
        if(barLength>184) Graphics_drawLineH(&g_sContext, 110, 110-min(92,barLength-184), 18); //draw top bar
        if(barLength>276) Graphics_drawLineV(&g_sContext, 18, 18, 18+min(92,barLength-276)); //draw left bar
    }
    #else
    printf("%s\n", notNotGameNN->messaggi[turnNN]);
    printf("Remaining time: %d\n", turnTimeNN-(sSecondsNN-*NNTime));
    #endif
}

void setupNN(notNot* game, int* time, uint8_t* currGame){
    notNotGameNN = game; // Set the game info
    NNTime = time; // Set the time
    currGameNN = currGame; // Set the current game

    setupJoystick(&directionJoystickNN); // Setup the joystick

    #ifndef TESTING
    _graphicsInit();
    #endif
}

void gameNN(){
    //initialization
    directionJoystickNN = 0; // Set the direction of the joystick to 0
    if(!(turnNN==notNotGameNN->num_turni)) turnNN = 0; // reset the turn
    sSecondsNN = *NNTime; // Set the time at turn start
    turnTimeNN = min(10,20-turnNN*2); // Set the time for the turn
    int seconds = *NNTime; // Set the time at game start
    enableInterruptJoystick(); // Enable the interrupts
    drawLCDNN();

    //game loop
    while(*NNTime>0 && *currGameNN==GAME_NOTNOT){ // While there is time left and the current game is notNot
        __sleep(); // waked by timer and ADC interrupts
        
        if(seconds!=*NNTime){ // If a second has passed
            seconds = *NNTime;
            drawLCDNN(); // Draw the LCD
        }

        if(sSecondsNN-*NNTime>=turnTimeNN){ // If the time is up
            addMistakeControl(); // Add a mistake
            turnNN = 0; // Reset the turn
            turnTimeNN = min(10,20-turnNN*2); // Set the time for the turn
            sSecondsNN = *NNTime; // Set the time at turn start
            drawLCDNN(); // Draw the LCD
        }

        if(directionJoystickNN!=0){

            if(turnNN==notNotGameNN->num_turni){
                victoryFeedBackControl();
            }else{
                uint8_t correct = 0;
                switch (directionJoystickNN){
                case 'L':
                    if(notNotGameNN->mosseCorrette[turnNN][0]){
                        correct = 1;
                    }
                    break;
                case 'U':
                    if(notNotGameNN->mosseCorrette[turnNN][1]){
                        correct = 1;
                    }
                    break;
                case 'R':
                    if(notNotGameNN->mosseCorrette[turnNN][2]){
                        correct = 1;
                    }
                    break;
                case 'D':
                    if(notNotGameNN->mosseCorrette[turnNN][3]){
                        correct = 1;
                    }
                    break;
                }

                if(correct){
                    correctFeedBackControl();
                    turnNN++;
                    turnTimeNN = min(10,20-turnNN*2);

                    if(turnNN==notNotGameNN->num_turni){
                        victoryFeedBackControl();
                    }

                }else {
                    addMistakeControl();
                    turnNN = 0;
                    turnTimeNN = min(10,20-turnNN*2);
                }

                sSecondsNN = *NNTime; // Set the time at turn start
                drawLCDNN();
            }

            directionJoystickNN=0;
        }
    }

    //disabling
    disableInterruptJoystick(); // Disable the interrupts
    //clear the screen
    #ifndef TESTING
    Graphics_clearDisplay(&g_sContext);
    #endif
}

#endif
