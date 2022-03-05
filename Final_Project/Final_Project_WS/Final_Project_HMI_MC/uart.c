 /******************************************************************************
 *
 * Module: UART Driver
 *
 * File Name: uart.c
 *
 * Description: functions definition for uart
 *
 * Author: Abdelrhman Adel
 *
 *******************************************************************************/

#include "uart.h"
#include "common_macros.h"
#include <AVR/io.h>

void UART_init(UART_Configuration * config)
{
	/*
	 *  Declare uint16 UBRR_value to calculate UBRR value and store it
	 */
	uint16 UBRR_value = 0;

	/*
	 * We will use double transmission speed by seting U2X by 1
	 */

	UCSRA |= (1<<U2X);
	/*
	 *  Enabling Rx & Tx of the UART peripheral
	 */
	UCSRB |= (1 << RXEN) | (1 << TXEN);
	UCSRC |= 1<<URSEL;
	/*
	 * Choosing 8-bits data
	 */
	UCSRC = (UCSRC & 0xCF) | (config->parity << 4);
	/*
	 * choose Parity bit and choose 1-bit as stop bit not 2-bits
	 */
	UCSRC = (UCSRC & 0xF9) | (config->data_bits << 1);
	/*
	 * Writing the baud rate in UBRR
	 */
	UBRR_value= ((uint16) ( (  ( F_CPU ) / ( 8UL * config->baud_rate ) ) - 1 ) );

	UBRRH = (UBRR_value>>8);
	UBRRL =  UBRR_value;
}

uint8 UART_receiveByte()
{
	/*
	 * Waiting for data utill it be received by UART
	 */
	while(BIT_IS_CLEAR(UCSRA,RXC));
	/*
	 * return the receive data
	 */
	return UDR;
}
void UART_sendByte(uint8 data)
{
	UDR = data;
	while(BIT_IS_CLEAR(UCSRA,TXC)); // Wait until the transmission is complete TXC = 1
	SET_BIT(UCSRA,TXC); // Clear the TXC flag
}
void UART_receiveString(uint8* str)
{
	/*Declare index initialized by */
	uint8 idx=0;
	/*receive first character*/
	str[idx]=UART_receiveByte();
/*receive data until special sign(#) between the two UART*/
	while(str[idx]!='#')
	{
		idx++; /*increment index*/
		str[idx]=UART_receiveByte();/*receive a character*/
	}
	str[idx]='\0';
}
void UART_sendString(uint8* str)
{
	/*declare an index  and initialized by 0*/
	uint8 idx=0;
	/*sending the string*/
	while(str[idx] != '\0')
	{
		UART_sendByte(str[idx]);
		idx++;
	}
}
