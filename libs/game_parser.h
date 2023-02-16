#ifndef __GAMEPARSER_H__
#define __GAMEPARSER_H__

#include <json.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <constants.h>
#include <stdio.h>
#include <string.h>

typedef struct
{
    char messaggi[NOTNOTMAXTURNS][NOTNOTMAXMSGDIM];    //what to show on screen
    uint8_t mosseCorrette[NOTNOTMAXTURNS][4]; //what is the correct move (1-left 2-up 3-right 4-down), I can have more than one correct moves
    int num_turni;                                   //number of turns in the game
} notNot;

typedef struct
{
    char matrix[SIMONSAYSMAXTURNS][SIMONSAYSMAXSTEPPERTURN][8];         //matrix[1][2][1] -> how to turn on the led in the first turn, second flash, line 1 matrix led
    uint8_t mosseCorrette[SIMONSAYSMAXTURNS][SIMONSAYSMAXSTEPPERTURN];  //index logic is the same, contains the correct sequence of moves for each turn (1-9 for which button is correct)
    int mossePerTurno[SIMONSAYSMAXTURNS];                               //for each turn how many moves there are
    int numTurni;                                                       //how many turns there are
} simonSays;

typedef struct
{
    char SerialNumber[SNMAXDIM];        //serial number bomb
    int time;                           //time before explosion in seconds
    uint8_t FiliDaTagliare[NUMCAVI];    //cable order: P Y G R
    notNot notNotGame;       
    simonSays simonSaysGame; 
    char numPadCode[CODEPADMAXDIM];     //code for the keypad
    uint8_t valid_game;                 //1 if the game is valid, 0 otherwise
} game;

/**
 * @brief print the game info
 * @param gameInfo struct with the game info
 * @return None
 */
void printGame(game gameInfo)
{
    int i, j, k;
    printf("\nSerialNumber: %s\n", gameInfo.SerialNumber);
    printf("time: %d seconds\n", gameInfo.time);

    printf("FiliDaTagliare: ");
    for (i = 0; i < NUMCAVI; i++)
    {
        printf("%d ", gameInfo.FiliDaTagliare[i]);
    }

    printf("\nCodice tastierino: %s\n", gameInfo.numPadCode);
    printf("\nnotNotGame turni %d: \n", gameInfo.notNotGame.num_turni);
    for (i = 0; i < gameInfo.notNotGame.num_turni; i++)
    {
        printf("  turno %d: messaggio %s mosse corrette %d %d %d %d\n", i, gameInfo.notNotGame.messaggi[i], 
                gameInfo.notNotGame.mosseCorrette[i][0], gameInfo.notNotGame.mosseCorrette[i][1], gameInfo.notNotGame.mosseCorrette[i][2], gameInfo.notNotGame.mosseCorrette[i][3]);
    }
    printf("\nsimonSaysGame turni %d: \n", gameInfo.simonSaysGame.numTurni);
    for (i = 0; i < gameInfo.simonSaysGame.numTurni; i++)
    {
        printf("  turno %d: mosse %d\n", i, gameInfo.simonSaysGame.mossePerTurno[i]);
        printf("    luci: \n");
        for (j = 0; j < gameInfo.simonSaysGame.mossePerTurno[i]; j++)
        {
            printf("     matrice %d: ", j);
            for (k = 0; k < 8; k++)
            {
                printf("%d ", gameInfo.simonSaysGame.matrix[i][j][k]);
            }
            printf("\n");
        }
        printf("    mosse corrette: \n");
        for (j = 0; j < gameInfo.simonSaysGame.mossePerTurno[i]; j++)
        {
            printf("     mossa %d: %d\n", j, gameInfo.simonSaysGame.mosseCorrette[i][j]);
        }
    }
}

int jsoneq(const char *json, jsmntok_t *tok, const char *s)
{
    if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start
            && strncmp(json + tok->start, s, tok->end - tok->start) == 0)
    {
        return 0;
    }
    return -1;
}

/**
 * @brief parse the game json
 * @param data json string
 * @param size size of the json string
 * @return game struct with the game info
 */
game gameParser(char *data, int size)
{
    int r, i, k, j, q, l;
    jsmn_parser p;
    jsmntok_t t[MAXJSONTOKEN]; /* We expect no more than MAXJSONTOKEN JSON tokens */
    game gameInfo;
    gameInfo.valid_game = 1;

    jsmn_init(&p);
    r = jsmn_parse(&p, data, size, t, MAXJSONTOKEN); // "s" is the char array holding the json content

    if (r < 0)
    {
        //printf("JSON invalido jsmn %d", r);
        gameInfo.valid_game = 0;
        return gameInfo;
    }

    /* Assume the top-level element is an object */
    if (r < 1 || t[0].type != JSMN_OBJECT)
    {
        //printf("JSON invalido obj");
        gameInfo.valid_game = 0;
        return gameInfo;
    }

    /* Loop over all keys of the root object */
    i = 1;
    while (i < r)
    {
        //controlla se una chiave matcha un certo valore e esegui di conseguenza
        if (jsoneq(data, &t[i], "SN") == 0)
        {
            ++i;
            if (t[i].end - t[i].start >= SNMAXDIM)
            {
                //printf("SN too long %d", t[i].end - t[i].start);
                gameInfo.valid_game = 0;
                return gameInfo;
            }

            strncpy(gameInfo.SerialNumber, data + t[i].start,
                    t[i].end - t[i].start);
            gameInfo.SerialNumber[t[i].end - t[i].start] = '\0';
            //printf("SerialNumber: %s\n", gameInfo.SerialNumber);
            ++i;
        }
        else if (jsoneq(data, &t[i], "TIME") == 0)
        {
            ++i;
            gameInfo.time = atoi(data + t[i].start);
            //printf("time: %d seconds\n", gameInfo.time);
            ++i;
        }
        else if (jsoneq(data, &t[i], "cavi") == 0)
        {
            ++i;

            //sono dentro l'array
            if(t[i].type != JSMN_ARRAY)
            {
                //printf("JSON invalido cavi");
                gameInfo.valid_game = 0;
                return gameInfo;
            }
            //controlla dimensione array
            if(t[i].size != NUMCAVI)
            {
                //printf("JSON invalido num cavi");
                gameInfo.valid_game = 0;
                return gameInfo;
            }
            ++i;

            //copia valori array
            for (k = 0; k < NUMCAVI; k++)
            {
                if(t[i].type != JSMN_PRIMITIVE)
                {
                    //printf("JSON invalido cavi");
                    gameInfo.valid_game = 0;
                    return gameInfo;
                }

                gameInfo.FiliDaTagliare[k] = atoi(data + t[i].start);
                //printf("FiliDaTagliare[%d]: %d\n", k, gameInfo.FiliDaTagliare[k]);

                ++i;
            }
        }
        else if (jsoneq(data, &t[i], "numPad") == 0)
        {
            ++i;

            if (t[i].end - t[i].start >= CODEPADMAXDIM)
            {
                //printf("code too long");
                gameInfo.valid_game = 0;
                return gameInfo;
            }

            strncpy(gameInfo.numPadCode, data + t[i].start,
                    t[i].end - t[i].start);
            gameInfo.numPadCode[t[i].end - t[i].start] = '\0';
            //printf("numPadCode: %s\n", gameInfo.numPadCode);
            ++i;
        }
        else if (jsoneq(data, &t[i], "notNot") == 0)
        {
            ++i;
            //inside object
            ++i;
            int size = 0;
            for (k = 0; k < 2; k++)
            {
                if (jsoneq(data, &t[i], "messaggi") == 0)
                {
                    ++i;
                    //inside array

                    //se gia' lette mosse e non rispettano la stessa dim allora JSON errato
                    if ((size != 0 && t[i].size != size)
                            || t[i].size > NOTNOTMAXTURNS)
                    {
                        //printf("errore dimensione messaggi %d %d", size, t[i].size);
                        gameInfo.valid_game = 0;
                        return gameInfo;
                    }

                    size = t[i].size;
                    //printf("size messaggi: %d\n", size);
                    gameInfo.notNotGame.num_turni = size;
                    ++i;
                    //iterate over array
                    for (j = 0; j < size; j++)
                    {
                        if (t[i].end - t[i].start >= NOTNOTMAXMSGDIM)
                        {
                            //printf("messaggio %d troppo lungo", j);
                            gameInfo.valid_game = 0;
                            return gameInfo;
                        }

                        strncpy(gameInfo.notNotGame.messaggi[j],
                                data + t[i].start, t[i].end - t[i].start);
                        gameInfo.notNotGame.messaggi[j][t[i].end - t[i].start] =
                                '\0';
                        //printf("messaggi %d: %s\n", j, gameInfo.notNotGame.messaggi[j]);
                        ++i;
                    }
                }
                else if (jsoneq(data, &t[i], "mosse") == 0)
                {
                    ++i;
                    //inside array

                    //se gia' letti messaggi e non rispettano la stessa dim allora JSON errato
                    if (size != 0
                            && t[i].size != size|| t[i].size>NOTNOTMAXTURNS)
                    {
                        //printf("errore dimensione mosse");
                        gameInfo.valid_game = 0;
                        return gameInfo;
                    }
                    size = t[i].size;
                    //printf("size mosse: %d\n", size);
                    ++i;
                    int internal_size;
                    //iterating objects
                    for (j = 0; j < size; j++)
                    {
                        //dentro array per iterare
                        internal_size = t[i].size;
                        ++i;
                        //dentro array mosse corrette
                        for(k=0;k<4;k++){
                            if(k<internal_size){
                                gameInfo.notNotGame.mosseCorrette[j][k] = atoi(
                                    data + t[i].start);
                                ++i;
                            }else{
                                gameInfo.notNotGame.mosseCorrette[j][k] = gameInfo.notNotGame.mosseCorrette[j][k-1];
                            }
                        }
                    }
                }
            }
            gameInfo.notNotGame.num_turni = size;
        }
        else if (jsoneq(data, &t[i], "simonSays") == 0)
        {
            ++i;
            //token oggetto
            int size = 0;

            //inizializzo dimensione turni per verifica errori
            for (k = 0; k < SIMONSAYSMAXTURNS; k++)
            {
                gameInfo.simonSaysGame.mossePerTurno[k] = 0;
            }

            ++i;
            //token dentro oggetto
            for (k = 0; k < 3; k++)
            {
                if (jsoneq(data, &t[i], "ledAccesi") == 0)
                {
                    //dovrebbe essere un array di array di array di char
                    ++i;

                    //se ho gia' letto qualcuno che mi ha detto il numero di turni ma non corrisponde
                    if ((size != 0 && size != t[i].size)
                            || t[i].size > SIMONSAYSMAXTURNS)
                    {
                        //printf("errore dimensione turni in matrice led");
                        gameInfo.valid_game = 0;
                        return gameInfo;
                    }
                    size = t[i].size;
                    gameInfo.simonSaysGame.numTurni = size;
                    ++i;
                    //printf("numero turni: %d\n", size);

                    //itero gli array dentro l'array
                    for (j = 0; j < size; j++)
                    {
                        //sono in un array
                        if ((gameInfo.simonSaysGame.mossePerTurno[j] != 0
                                && gameInfo.simonSaysGame.mossePerTurno[j]
                                        != t[i].size)
                                || t[i].size > SIMONSAYSMAXSTEPPERTURN)
                        {
                            //printf("errore negli dimensione step nella definizione dei led del turno %d",j);
                            gameInfo.valid_game = 0;
                            return gameInfo;
                        }
                        gameInfo.simonSaysGame.mossePerTurno[j] = t[i].size;
                        ++i;
                        //printf("numero di step nel turno %d: %d\n", j, gameInfo.simonSaysGame.mossePerTurno[j]);

                        //itero nell'array, che contiene array di char
                        for (q = 0; q < gameInfo.simonSaysGame.mossePerTurno[j];
                                q++)
                        {
                            if (t[i].size != 8)
                            {
                                //printf("errore nella definizione della matrice nello step %d del turno %d", q, j);
                                gameInfo.valid_game = 0;
                                return gameInfo;
                            }
                            ++i;
                            //printf("salvando la riga %d, step %d, turno %d\n",l,q,j);
                            for (l = 0; l < 8; l++)
                            {
                                //salvo il char che codifica in binario gli 8 led della riga l
                                gameInfo.simonSaysGame.matrix[j][q][l] =atoi(data + t[i].start);
                                //printf("il numero binario che codifica la riga di led e': %d\n", gameInfo.simonSaysGame.matrix[j][q][l]);
                                ++i;
                            }
                        }
                    }
                }
                else if (jsoneq(data, &t[i], "mosse") == 0)
                {
                    //array di array di numeri
                    ++i;

                    //se ho gia' letto qualcuno che mi ha detto il numero di turni ma non corrisponde
                    if ((size != 0 && size != t[i].size)
                            || t[i].size > SIMONSAYSMAXTURNS)
                    {
                        //printf("errore dimensione turni in matrice led");
                        gameInfo.valid_game = 0;
                        return gameInfo;
                    }
                    size = t[i].size;
                    gameInfo.simonSaysGame.numTurni = size;
                    ++i;
                    //printf("numero turni: %d\n", size);

                    //itero nell'array di numeri
                    for (j = 0; j < size; j++)
                    {
                        //controllo che il numero di step nel turno sia rispettato
                        if ((gameInfo.simonSaysGame.mossePerTurno[j] != 0
                                && gameInfo.simonSaysGame.mossePerTurno[j]
                                        != t[i].size)
                                || t[i].size > SIMONSAYSMAXSTEPPERTURN)
                        {
                            //printf("errore negli dimensione step nella definizione delle mosse corrette del turno %d", j);
                            gameInfo.valid_game = 0;
                            return gameInfo;
                        }
                        gameInfo.simonSaysGame.mossePerTurno[j] = t[i].size;
                        ++i;
                        //printf("numero di step nella soluzione del turno %d: %d\n",j,gameInfo.simonSaysGame.mossePerTurno[j]);

                        for(q=0;q<gameInfo.simonSaysGame.mossePerTurno[j];q++){
                            gameInfo.simonSaysGame.mosseCorrette[j][q] = atoi(data+t[i].start);
                            //printf("mossa corretta nello step %d del turno %d: %d\n", q, j, gameInfo.simonSaysGame.mosseCorrette[j][q]);
                            ++i;
                        }
                    }
                }
            }
        }
        else
        {
            //printf("Unexpected key: %.*s\n", t[i].end - t[i].start, data + t[i].start);
            ++i;
        }
    }

    //printf("ended correctly\n");

    return gameInfo;
}

#endif
