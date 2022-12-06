#ifndef __LCD_H__
#define __LCD_H__

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <constants.h>

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

//TODO: usare puntatore a struttura in main
lcdConfig H_LCD_CONFIG;

void serialSend(char d){
    //send 4 high

    if(d & 16) GPIO_setOutputHighOnPin(H_LCD_CONFIG.port_d4, H_LCD_CONFIG.pin_d4);
    else GPIO_setOutputLowOnPin(H_LCD_CONFIG.port_d4, H_LCD_CONFIG.pin_d4);
    if(d & 32) GPIO_setOutputHighOnPin(H_LCD_CONFIG.port_d5, H_LCD_CONFIG.pin_d5);
    else GPIO_setOutputLowOnPin(H_LCD_CONFIG.port_d5, H_LCD_CONFIG.pin_d5);
    if(d & 64) GPIO_setOutputHighOnPin(H_LCD_CONFIG.port_d6, H_LCD_CONFIG.pin_d6);
    else GPIO_setOutputLowOnPin(H_LCD_CONFIG.port_d6, H_LCD_CONFIG.pin_d6);
    if(d & 128) GPIO_setOutputHighOnPin(H_LCD_CONFIG.port_d7, H_LCD_CONFIG.pin_d7);
    else GPIO_setOutputLowOnPin(H_LCD_CONFIG.port_d7, H_LCD_CONFIG.pin_d7);

    GPIO_setOutputHighOnPin(H_LCD_CONFIG.port_e, H_LCD_CONFIG.pin_e);
    GPIO_setOutputLowOnPin(H_LCD_CONFIG.port_e, H_LCD_CONFIG.pin_e);

    //send 4 lower

    if(d & 1) GPIO_setOutputHighOnPin(H_LCD_CONFIG.port_d4, H_LCD_CONFIG.pin_d4);
    else GPIO_setOutputLowOnPin(H_LCD_CONFIG.port_d4, H_LCD_CONFIG.pin_d4);
    if(d & 2) GPIO_setOutputHighOnPin(H_LCD_CONFIG.port_d5, H_LCD_CONFIG.pin_d5);
    else GPIO_setOutputLowOnPin(H_LCD_CONFIG.port_d5, H_LCD_CONFIG.pin_d5);
    if(d & 4) GPIO_setOutputHighOnPin(H_LCD_CONFIG.port_d6, H_LCD_CONFIG.pin_d6);
    else GPIO_setOutputLowOnPin(H_LCD_CONFIG.port_d6, H_LCD_CONFIG.pin_d6);
    if(d & 8) GPIO_setOutputHighOnPin(H_LCD_CONFIG.port_d7, H_LCD_CONFIG.pin_d7);
    else GPIO_setOutputLowOnPin(H_LCD_CONFIG.port_d7, H_LCD_CONFIG.pin_d7);

    GPIO_setOutputHighOnPin(H_LCD_CONFIG.port_e, H_LCD_CONFIG.pin_e);
    GPIO_setOutputLowOnPin(H_LCD_CONFIG.port_e, H_LCD_CONFIG.pin_e);

    //aspetto 40 microsecondi
    __delay_cycles(1920);
}

void sendCommand(char cm){
    GPIO_setOutputLowOnPin(H_LCD_CONFIG.port_rs, H_LCD_CONFIG.pin_rs);
    serialSend(cm);
}

void sendData(char data){
    GPIO_setOutputHighOnPin(H_LCD_CONFIG.port_rs, H_LCD_CONFIG.pin_rs);
    serialSend(data);
}

void clearLCD(){
    sendCommand(0x01);
}

/*
 * presume 48MHz nei wait time
 */
void setupLCD(lcdConfig conf){
    H_LCD_CONFIG = conf;

    GPIO_setAsOutputPin(conf.port_rs, conf.pin_rs);
    GPIO_setAsOutputPin(conf.port_e, conf.pin_e);

    GPIO_setAsOutputPin(conf.port_d4, conf.pin_d4);
    GPIO_setAsOutputPin(conf.port_d5, conf.pin_d5);
    GPIO_setAsOutputPin(conf.port_d6, conf.pin_d6);
    GPIO_setAsOutputPin(conf.port_d7, conf.pin_d7);

    //subito 8 bit mando prima tutti 0 poi transisco a 4 bit
    //se gi� 4 bit interpretato come comando return home
    sendCommand(0x02);

    __delay_cycles(96000);
    //max 2ms per return home command -> frequencyHz quindi devo aspettare frequency/1000*2
    //imposto
    sendCommand(0x28);
    clearLCD();
}

void displayLCD(rowSel row, char* string, int size){
    if(row==ROW_1){
        sendCommand(0x80);
    }else{
        sendCommand(0xC0);
    }
    sendCommand(0x00);

    int i;
    int space = (16-size)/2;
    for(i=0;i<space;i++){
        sendData(' ');
    }
    for (i=0;i<size;i++){
        sendData(string[i]);
    }
    for(i=0;i<16-size-space;i++){
        sendData(' ');
    }
}


#endif
