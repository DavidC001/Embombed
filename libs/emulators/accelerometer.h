#ifndef __ACCEL_H__
#define __ACCEL_H__

/* DriverLib Includes */
#include "../constants.h"
#include "mutex.h"
#include <stdio.h>

uint8_t *accelerometerFacePunt;

void ADCAccelerometerIRQ(void)
{
    while (1)
    {
        pthread_mutex_lock(&mutex);
        printf("vuoi simulare il joystick? (y/n) ");
        char c;
        scanf(" %c", &c);
        if (c == 'y')
        {
            do
            {
                printf("quale input? (0-4) ");
                char c;
                scanf(" %c", &c);
            } while (c < '0' || c > '4');
            *accelerometerFacePunt = c - '0';
        }
        pthread_mutex_unlock(&mutex);
        sleep(5);
    }
}

void setupAccelerometer(uint8_t *punt)
{
    accelerometerFacePunt = punt;

    // start thread
    pthread_t thread;
    pthread_create(&thread, NULL, (void *)&ADCAccelerometerIRQ, NULL);
}

#endif
