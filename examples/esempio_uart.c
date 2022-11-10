#include "msp.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <uart_d.h>
#include <json.h>

/*
 * static const char *JSON_STRING =
 * "{\"user\": \"johndoe\", \"admin\": false, \"uid\": 1000, \"groups\": [\"users\", \"wheel\", \"audio\", \"video\"]}";
 */

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
  if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start &&
      strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
    return 0;
  }
  return -1;
}

void consumePacket(char* data, int size){
    int i=0,r;
    printf("packet of size %d:\n", size);
    for(i=0;i<size;i++){
        printf("%c", data[i]);
    }
    printf("\n<end>\n");

    jsmn_parser p;
    jsmntok_t t[128]; /* We expect no more than 128 JSON tokens */

    jsmn_init(&p);
    r = jsmn_parse(&p, data, size, t, 128); // "s" is the char array holding the json content

    if (r < 0) {
        sendUART("Failed to parse JSON", 20);
        return;
      }

      /* Assume the top-level element is an object */
      if (r < 1 || t[0].type != JSMN_OBJECT) {
        sendUART("Failed to parse JSON", 20);
        return;
      }

      /* Loop over all keys of the root object */
      for (i = 1; i < r; i++) {
        //controlla se una chiave matcha un certo valore e esegui di conseguenza
        if (jsoneq(data, &t[i], "user") == 0) {
          /* We may use strndup() to fetch string value */
          printf("- User: %.*s\n", t[i + 1].end - t[i + 1].start,
                 data + t[i + 1].start);
          i++;
        } else if (jsoneq(data, &t[i], "admin") == 0) {
          /* We may additionally check if the value is either "true" or "false" */
          printf("- Admin: %.*s\n", t[i + 1].end - t[i + 1].start,
                 data + t[i + 1].start);
          i++;
        } else if (jsoneq(data, &t[i], "uid") == 0) {
          /* We may want to do strtol() here to get numeric value */
          printf("- UID: %.*s\n", t[i + 1].end - t[i + 1].start,
                 data + t[i + 1].start);
          i++;
        } else if (jsoneq(data, &t[i], "groups") == 0) {
          int j;
          printf("- Groups:\n");
          if (t[i + 1].type != JSMN_ARRAY) {
            continue; /* We expect groups to be an array of strings */
          }
          for (j = 0; j < t[i + 1].size; j++) {
            jsmntok_t *g = &t[i + j + 2];
            printf("  * %.*s\n", g->end - g->start, data + g->start);
          }
          i += t[i + 1].size + 1;
        } else {
          printf("Unexpected key: %.*s\n", t[i].end - t[i].start,
                 data + t[i].start);
        }
      }

    sendUART("received data",13);
}

int main(void)
{
    /* Halting WDT  */
    MAP_WDT_A_holdTimer();

    //dove appoggiare l'indirizzo del messaggio da leggere
    char* message;
    //questa � la mia flag che mi dice se ho ricevuto un messaggio
    //dove viene anche salvato quanto lungo � il messaggio
    int size;


    setupUART(&message, &size, 2);
    // messaggio , size, dimensione dell'header in byte (2 = 16bit => messaggi lunghi massimo 2^16-1)

    while(1){
        __sleep();
        //se dopo aver ricevuto un interrrupt mi ritrovo con size diverso da 0 ho ricevuto un messaggio e posso leggerlo
        if(size!=0){
            //chiamo la mia funzione che legger� il messaggio ricevuto
            consumePacket(message, size);
            //resetto size quando ho finito di leggere il messaggio e libero la comunicazione
            //comunicazioni mentre elaboravo il messaggio sono state rifiutate dopo l'invio dell'header
            //inviando il messaggio 'R'
            free(message);
            size=0;
        }
    }
}
