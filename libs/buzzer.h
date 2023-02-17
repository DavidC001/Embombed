#ifndef __BUZZER_H__
#define __BUZZER_H__

/**
 * @brief starts the music
 * @return None
 * 
 * @note This function starts the music by setting up the timer and interrupt
 * It uses the A2 timer as a metronome set to 1KHz that counts up to 60000/(TEMPO*GRID)
 */
void startMusic();

/**
 * @brief stops the music
 * @return None
 * 
 * @note This function stops the music by stopping the timers and setting the pause flag
 */
void stopMusic();

/**
 * @brief send a buzzer melody
 * 
 * @param freq array of frequencies to play
 * @param size size of the array
 * 
 * @return None
 * 
 * @note This function plays a melody on the buzzer
 * It uses the A0 timer to generate the sound
 * each frequency is played for 20ms
 * to make it easier to hear the melody, there is 20ms of silence before and after the melody
 */
void sendBuzzer(int freq[], int size);

/**
 * @brief sets up the buzzer
 * @return None
 * 
 * @note This function sets up the buzzer by setting up the timer and the PWM
 * It uses the A0 timer to generate the sound
 */
void setupBuzzer();

#endif
