#ifndef __ADCSUB_H__
#define __ADCSUB_H__

/**
 * @brief disable ADC handler
 * 
 * @param index the index of the handler to disable
 * 
 * @return None
 * 
 * @note the handler is not removed from the list, it is just disabled
 */
void disableADC(int index);

/**
 * @brief enable ADC handler
 * 
 * @param index the index of the handler to enable
 * 
 * @return None
 * 
 * @note the handler is not removed from the list, it is just enabled
 */
void enableADC(int index);

/**
 * @brief remove ADC handler
 * 
 * @param index the index of the handler to remove
 * 
 * @return None
 * 
 * @note the handler is removed from the list
 */
void removeADC(int index);


/**
 * @brief registerADC
 * @param intHandler A pointer to the interrupt handler function.
 * @return int
 * 
 * @note register interrupt
 * return the index where it is saved
 * does not handle the mapping of the memory that must be done externally
 */
int registerADC(void (*intHandler)(void));

/**
 * @brief start ADC conversion
 * @return None
 * 
 * @note enable conversion and interrupt,
 * enable automatic iteration,
 * enable conversion trigger.
 * From now on the interrupt will be called
 */
void enableConvADC();

/**
 * @brief setup ADC
 * @return None
 * 
 * @note start automatic iteration
 * and clock to ADCOSC/64/8
 */
void setupADC();

#endif
