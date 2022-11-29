#include "msp.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <uart_d.h>
#include <game_parser.h>

/*
 * esempio JSON:
    {
        "SN": "AB123456789",
        "TIME": 320,
        "cavi": 1,
        "numPad": "1234AjjSDsd",
        "notNot":{
            "messaggi":[
                "down",
                "up"
            ],
            "mosse":[
                4,
                2
            ]
        },
        "simonSays":{
            "ledAccesi":[
                [
                    ["a","b","c","d","a","b","c","d"],
                    ["a","b","c","d","a","b","c","d"],
                    ["a","b","c","d","a","b","c","d"]
                ],
                [
                    ["a","b","c","d","a","b","c","d"],
                    ["a","b","c","d","a","b","c","d"]
                ]
            ],
            "mosse":[
                [1 , 3, 4],
                [2 , 5]
            ]
        }
    }
 */

int main(void)
{
    /* Halting WDT  */
    MAP_WDT_A_holdTimer();

    //dove appoggiare l'indirizzo del messaggio da leggere
    char* message;
    //questa è la mia flag che mi dice se ho ricevuto un messaggio
    //dove viene anche salvato quanto lungo è il messaggio
    int size;


    setupUART(&message, &size, 2);
    // messaggio , size, dimensione dell'header in byte (2 = 16bit => messaggi lunghi massimo 2^16-1)
    sendUART("start", 5);

    //struttura dati in cui saranno salvati i dati del gioco
    game gameInfo;

    while(1){
        __sleep();
        //se dopo aver ricevuto un interrrupt mi ritrovo con size diverso da 0 ho ricevuto un messaggio e posso leggerlo
        if(size!=0){
            //chiamo la mia funzione che leggerà il messaggio ricevuto
            //si può cambiare ma ho messo che se il primo carattere della comunicazione è G significa
            //che sto passando le informazioni del gioco si potrebbe anche non mettere
            //definendo un ordine delle operazioni
            if(message[0]=='G'){

                //passo il messaggio json con la sua grandezza
                //che verrà interpretato
                //ritornando la struct game
                gameInfo = gameParser(message+1, size-1);

                //se c'erano errori nel json generato viene ritornato un errore al dispositivo che ha mandato i dati
                if(gameInfo.valid_game == 0){
                    sendUART("ERR",3);
                }else{
                    sendUART("received data",13);
                    printGame(gameInfo);
                }
            }else{
                printf(message);
                sendUART("received data",13);
            }


            //resetto size quando ho finito di leggere il messaggio e libero la comunicazione
            //comunicazioni mentre elaboravo il messaggio sono state rifiutate dopo l'invio dell'header
            //inviando il messaggio 'R'
            free(message);
            size=0;
        }
    }
}
