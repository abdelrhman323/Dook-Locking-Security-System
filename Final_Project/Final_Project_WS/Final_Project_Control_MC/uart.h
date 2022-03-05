 /******************************************************************************
 *
 * Module: UART Driver
 *
 * File Name: uart.h
 *
 * Description: functions prototypes for uart
 *
 * Author: Abdelrhman Adel
 *
 *******************************************************************************/


#ifndef UART_H_
#define UART_H_

#include "std_types.h"

/***************************************************************************
 *                     UART_Data_Bits Enumeration Definition               *
 **************************************************************************/
/*
 * Enumeration Name :  UART_Data_Bits
 * Members:
 * 			- five_bit    : five  data bits for the frame
 * 			- six_bits    : six   data bits for the frame
 * 			- seven_bit   : seven data bits for the frame
 * 			- eight_bits  : eight data bits for the frame
 */
typedef enum
{
	five_bit,six_bits,seven_bit,eight_bits

}UART_Data_Bits;

/***************************************************************************
 *                     UART_Parity Enumeration Definition                  *
 **************************************************************************/
/*
 * Enumeration Name :  UART_Parity
 * Members:
 * 			- disable   : no parity bit
 * 			- reversed  : reversing the parity of frame
 * 			- even      : enable even parity
 * 			- odd       : enable even parity
 */
typedef enum
{
	disable,reversed,even,odd

}UART_Parity;

/***************************************************************************
 *                     UART_Stop Enumeration Definition                  *
 **************************************************************************/
/*
 * Enumeration Name :  UART_Stop
 * Members:
 * 			- one_bit   : one stop bit for the frame
 * 			- two_bits  : two stop bit for the frame
 */
typedef enum
{
	one_bit,two_bits

}UART_Stop;

/***************************************************************************
 *                     UART_Configuration Structure Definition             *
 **************************************************************************/
/*
 * Structure Name :  UART_Configuration
 * Members:
 * 			- data_bits : number of bits for transmitted data
 * 			- parity    : even or odd parity
 * 			- stop-bits : zero or one or two stop bits for frame
 * 			- baud_rate : value of baud rate (9600 recommended)
 */
typedef struct
{
	UART_Data_Bits data_bits;
	UART_Parity    parity;
	UART_Stop      stop;
	uint16         baud_rate;
}UART_Configuration;
/***************************************************************************
 *                          Functions Prototypes                           *
 **************************************************************************/
/*
 * Description :
 *           this function initialize the UART peripheral by:
 *           	1)  Enable UART receiver
 *           	2)  Enable UART
 *				3)  Select Asynchronous option
 *				4)  Enable Parity bit either even or odd, Or Disable it
 *				5)  Choose data size (2,3,4,5,6,7,8,9)
 *				6)  Choose stop bit 1 bit or 2 bits
 *				7)  Writing baud rate needed in UBRRH register
 *	Asynchronous Function
 *	Input :uint16 baud rate
 *	Output:void
 */
void UART_init(UART_Configuration *);
/*
 * Description :
 *        this function will read the received byte at Rx
 * Synchronous Function
 * Input : void
 * Output: uint8 received data byte
 */
uint8 UART_receiveByte(void);
/*
 * Description :
 *        this function will send byte by Tx
 * Synchronous Function
 * Input : uint8 send data byte
 * Output: void */
void UART_sendByte(uint8);
/*
 * Description :
 *         this function to receive string until '#' character
 */
void UART_receiveString(uint8*);
/*
 * Description :
 *         this function to send string
 */
void UART_sendString(uint8*);
#endif /* UART_H_ */
