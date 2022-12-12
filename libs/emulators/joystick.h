#ifndef __JOYSTICK_H__
#define __JOYSTICK_H__

/* DriverLib Includes */
#include "../constants.h"
#include "mutex.h"
#include <stdio.h>

char* puntDirectionJoystick;
int enableJoystick = 0;

void ADCJoystickIRQ(void){
    while(1){
        if(enableJoystick && *puntDirectionJoystick==0){
            pthread_mutex_lock(&mutex);
            printf("vuoi simulare il joystick? (y/n) ");
            char c;
            scanf(" %c", &c);
            if(c=='y'){
                do{
                    printf("quale input? (1-4) ");
                    char c;
                    scanf(" %c", &c);
                }while(c<'1' || c>'4');
                *puntDirectionJoystick = c-'0';
            }
            pthread_mutex_unlock(&mutex);
        }
        sleep(5);
    }
}

void disableInterruptJoystick(){
    enableJoystick = 0;
}

void enableInterruptJoystick(){
    enableJoystick = 1;
}

void setupJoystick(char* punt){
    puntDirectionJoystick = punt;

    //start thread
    pthread_t thread;
    pthread_create(&thread, NULL, (void*) &ADCJoystickIRQ, NULL);
}

#endif
