#ifndef __DOTMATRIX_H__
#define __DOTMATRIX_H__

#include "../constants.h"
#include "mutex.h"
#include <stdio.h>

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 

int dotMatrixEnabled = 0;

void sendMat(char mat[8]){
    if(!dotMatrixEnabled){
        printf("dot matrix turned off\n");
        return;
    }

    //take mutex
    pthread_mutex_lock(&mutex);
    int i;
    printf("sending dot matrix content\n");
    for(i=0;i<8;i++){
        //write char as number in binary
        printf(BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(mat[i]));
    }
    printf("\n");
    //release mutex
    pthread_mutex_unlock(&mutex);
}

void clearMat(){
    pthread_mutex_lock(&mutex);
    printf("cleared dot matrix\n");
    pthread_mutex_unlock(&mutex);
}

void shutdownMat(){
    dotMatrixEnabled = 0;
}

void turnOnMat(){
    dotMatrixEnabled = 1;
}


void initMatrix(uint_fast8_t csPort, uint_fast16_t csPin){
    clearMat();
    shutdownMat();
}


#endif