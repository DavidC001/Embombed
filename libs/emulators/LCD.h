#ifndef __LCD_H__
#define __LCD_H__

#include "../constants.h"
#include "mutex.h"

typedef struct{
    uint_fast8_t port_rs;
    uint_fast16_t pin_rs;
    uint_fast8_t port_e;
    uint_fast16_t pin_e;
    uint_fast8_t port_d4;
    uint_fast16_t pin_d4;
    uint_fast8_t port_d5;
    uint_fast16_t pin_d5;
    uint_fast8_t port_d6;
    uint_fast16_t pin_d6;
    uint_fast8_t port_d7;
    uint_fast16_t pin_d7;
}lcdConfig;

typedef enum{
    ROW_1,
    ROW_2
} rowSel;

char H_LCD_ROW[2][16];

void clearLCD(){
    //take mutex
    pthread_mutex_lock(&mutex);
    printf("cleared LCD\n");
    H_LCD_ROW[0][0] = '\0';
    H_LCD_ROW[1][0] = '\0';
    //release mutex
    pthread_mutex_unlock(&mutex);
}

void setupLCD(lcdConfig conf){
    clearLCD();
}

void displayLCD(rowSel row, char* string, int size){
    H_LCD_ROW[row][size] = '\0';
    int i;
    for(i=0;i<size;i++){
        H_LCD_ROW[row][i] = string[i];
    }

    //take mutex
    pthread_mutex_lock(&mutex);
    printf("current LCD content\n");
    printf("\t%s\n", H_LCD_ROW[0]);
    printf("\t%s\n",H_LCD_ROW[1]);
    //release mutex
    pthread_mutex_unlock(&mutex);

}


#endif
