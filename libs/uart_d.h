#ifndef __UARTCUSTOM_H__
#define __UARTCUSTOM_H__

/**
 * @brief setup UART
 * 
 * @param message address of the array where the message will be stored
 * @param size address of the variable where the size of the message will be stored
 * @param headerSize size of the header
 * 
 * @return None
 *
 * @note setup the UART module to receive a message with a header of size headerSize
 * the message will be stored in the array pointed by message
 * the size of the message will be stored in the variable pointed by size
 * the header is composed of headerSize bytes
 * 
 * for more details on the """protocol""" see the development_process_spec.pdf file
 */
void setupUART(char* message,int* size, int headerSize);

/**
 * @brief send a message through UART
 * 
 * @param data pointer to the array containing the message
 * @param size size of the message
 * 
 * @return None
 * 
 * @note send a message through UART
 * the message is composed of a header of size UARTHeaderSize (defined during setup) and a body of size size
 * the header is composed of UARTHeaderSize bytes
 * the body is composed of size bytes
 */
void sendUART(char* data, unsigned int size);

#endif
