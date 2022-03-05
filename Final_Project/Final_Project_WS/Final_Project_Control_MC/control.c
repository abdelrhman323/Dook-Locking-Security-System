 /******************************************************************************
 *
 * Module: Control - ECU
 *
 * File Name: control.c
 *
 * Description: main function and other assisting functions for control
 *
 * Author: Abdelrhman Adel
 *
 *******************************************************************************/

#include "buzzer.h"
#include "external_eeprom.h"
#include "motor.h"
#include "uart.h"
#include "timer.h"
#include "gpio.h"
#include <AVR/io.h>
#include <AVR/interrupt.h>
#include <util/delay.h>

/****************************************************************************
 *                  Global Variables for the file scope
 ****************************************************************************/

uint8 g_wrong_password  = 0;/*wrong password times counter*/
uint8 g_password_length = 0;/*password length*/
uint8 g_ticks_rotate_motor = 0;/*tick counters for rotating motor time*/
uint8 g_ticks_close_door =0;/*tick counters for holding door time*/
volatile uint8 g_buzzerflag=0;/*flag for Error loop begin and termination*/
uint8 g_buzzerticks=0;/*ticks counter for Error eraise time calculation*/

/****************************************************************************
 *            Assisting functions for Control MC to do it's function        *
 ****************************************************************************/


void Save_Password(uint8* pass)
{
	/*
		 * Description : function for saving passwords in EEPROM
		 * Synchronous Function
		 * Input : one passwords strings
		 * Output: Void
		 */

	g_password_length=0;/*reset password length global variable*/
	uint8 idx = 0; /*index for saving loop*/
	uint16 address = 0x0311; /*assumed address in EEPROM*/
	/*Saving loop*/
	while(pass[idx-1] != '=')
	{
		EEPROM_writeByte(address,pass[idx]);
		_delay_ms(1000);
		idx++;
		address++;
	}
	g_password_length=idx;/*get the saved address length*/
}
uint8 Check_Password_Match(uint8* str)
{
	/*
		 * Description : function check for passwords strings matching
		 * Synchronous Function
		 * Input : one passwords strings
		 * Output: 0 (not match)  or 1 (match)
		 */

	uint8 match=1; /*assumed firstly the match until diffrence*/
	uint16 address = 0x0311; /*the assumed address for reding from EEPROM*/
	uint8 idx1=0;/*index for reading  loop*/
	uint8 idx2=0;/*index for checking loop*/
	uint8 real_password[20]; /*to store the read password from EEPROM*/
	/*reading password loop*/
	while(idx1 < g_password_length)
	{
		EEPROM_readByte(address,&(real_password[idx1]));
		_delay_ms(1000);
		idx1++;
		address++;
	}
	/*checking password loop*/
	while(idx2 < g_password_length)
	{
		if(str[idx2] != real_password[idx2])
		{
			match = 0;
		}
		idx2++;
	}

	return match;
}

void Stop_Motor_Final()
{
	/*
		 * Description : CallBack function for timer to stop motor after closing the door
		 * Asynchronous Function
		 * Input : Void
		 * Output: Void
		 */
	g_ticks_rotate_motor++;/*increment ticks for 15 sec calculations*/
	if(g_ticks_rotate_motor == 2)
	{
		DcMotor_Rotate(stop);/*stop motor*/
		TIMER_Deinit(timer1);/*clear timer for another usage*/
		g_ticks_rotate_motor=0; /*clear timer ticks*/
	}
}

void Close_Door()
{
	/*
		 * Description : function for closing the door
		 * Asynchronous Function
		 * Input : Void
		 * Output: Void
		 */
	DcMotor_Init(); /*Motor initialization*/
	/*timer initialization*/
	TIMER_Configuration config={timer1,CMP_Mode,0,58500,F_cpu_1024,0};
	TIMER_setCallback(Stop_Motor_Final,CMP_Mode,timer1);
	DcMotor_Rotate(A_CW);/*Motor rotate Anti-clockwise for closing*/
	TIMER_init(&config);
}

void Wait_door_callback()
{
	/*
		 * Description : CallBack function for timer to wait stop motor before closing the door
		 * Asynchronous Function
		 * Input : Void
		 * Output: Void
		 */

	g_ticks_close_door++;/*increment ticks for 15 sec calculations*/
	if(g_ticks_close_door == 1)
	{
		TIMER_Deinit(timer1);
		Close_Door();/*close the door*/
		g_ticks_close_door=0;
	}
}
void Wait_door()
{
	/*
		 * Description :  function for waiting to closing the door after 2-3 seconds
		 * Asynchronous Function
		 * Input : Void
		 * Output: Void
		 */
	SREG |= (1<<7);/*enable interrupt for timer*/
	/*initialize timer*/
	TIMER_Configuration config={timer1,CMP_Mode,0,23439,F_cpu_1024,0};
	TIMER_setCallback(Wait_door_callback,CMP_Mode,timer1);
	TIMER_init(&config);
}

void Stop_Motor()
{
	/*
		 * Description : CallBack function for timer to stop motor after closing the door
		 * Asynchronous Function
		 * Input : Void
		 * Output: Void
		 */
	g_ticks_rotate_motor++;/*increment ticks for 15 sec calculations*/
	if(g_ticks_rotate_motor == 2)
	{
		DcMotor_Rotate(stop); /*stop motor after opening the door*/
		TIMER_Deinit(timer1);
		g_ticks_rotate_motor=0;
		Wait_door();
	}
}
void Open_Door()
{
	/*
		 * Description : k function for  opening the door
		 * Asynchronous Function
		 * Input : Void
		 * Output: Void
		 */
	DcMotor_Init(); /*initialize motor*/
	/*timer1 initialization*/
	TIMER_Configuration config={timer1,CMP_Mode,0,58594,F_cpu_1024,0};
	TIMER_setCallback(Stop_Motor,CMP_Mode,timer1);
	DcMotor_Rotate(CW);
	TIMER_init(&config);
}

void Buzzer_Stop()
{
	/*
		 * Description : CallBack function for timer to stop buzzer after 1 min
		 * Asynchronous Function
		 * Input : Void
		 * Output: Void
		 */
	g_buzzerticks++;/*increment ticks counter to calculate 1min*/
	if(g_buzzerticks == 8)
	{
		g_buzzerflag=0;
		g_buzzerticks=0;
		TIMER_Deinit(timer1);
	}
}
void Change_pass()
{
	/*
		 * Description : function for changing password and store it again in EEPROM
		 * Synchronous Function
		 * Input : Void
		 * Output: Void
		 */
	uint8 entered_pass[15];/*to store received entered pass from HMI MC*/
	uint8 match=0;/*to store match check and send it to HMI*/
	uint8 new_pass[15];/*to store the new changed password*/
	UART_sendByte(g_wrong_password);/*send wrong pass times*/
	_delay_ms(100);/*delay for uart transmission*/
	UART_receiveString(entered_pass);/*receive entered pass for checking*/
	_delay_ms(1000);/*delay for uart transmission*/
	match = Check_Password_Match(entered_pass);/*checking pass for changing*/
	UART_sendByte(match);/*send result of checking*/
	_delay_ms(1000);/*delay for uart transmission*/
	if(match ==1)
	{
		/*
		 * if match -----> change password
		 */
		g_wrong_password=0;
		_delay_ms(2500);/*delay for uart transmission*/
		UART_receiveString(new_pass);/*receive new password*/
		Save_Password(new_pass);/*store new pass*/
	}
	else
	{
		/*
		 * if the wrong times get 3 times ----> turn on buzzer for 1 min
		 */
		g_wrong_password++;/*increment wrong times*/
		if(g_wrong_password == 3)
		{
			g_buzzerflag=1;/*to begin buzzer loop*/
			/*initialize timer*/
			TIMER_Configuration t_config={timer1,CMP_Mode,0,58594,F_cpu_1024,0};
			TIMER_setCallback(Buzzer_Stop,CMP_Mode,timer1);
			TIMER_init(&t_config);
			/*if 1 min passed flag =0 and loop will be terminated*/
			while(g_buzzerflag == 1)
			{
				BUZZER_ON();/*turn buzzer on*/
			}
			g_wrong_password=0;
			BUZZER_OFF();/*turn off buzzer after 1 min*/
		}

	}
}

int main(void)
{
	uint8 option=0;/*to store option*/
	uint8 password[20];/*to get the password first time as an admin*/
	uint8 entered_pass[20];/*to store the entered password from user each timer*/
	SREG |= (1<<7);/*enable interrupts for timer*/
	uint8 match =0;/*to store matching result*/
	/*UART initialization*/
	UART_Configuration  configuration={eight_bits,disable,one_bit,9600};
	BUZZER_init();	/*buzzer initialization*/
	UART_init(&configuration);
	/*EEPROM initialization*/
	EEPROM_init();
	UART_receiveString(password);/*receive the first password to HMI MC to store it*/
	_delay_ms(1000);/*delay for uart transmission*/
	Save_Password(password);/*save first password*/
	while(1)
	{
		/*get the  option from user*/
		option = UART_receiveByte();
		_delay_ms(2000);/*delay for uart transmission*/
		if(option == '-')
		{
			/*
			 * if the option = '-' .....> change the password
			 */
			Change_pass();
		}
		else if(option == '+')
		{
			/*if '+' ask for password for opening the door*/
			UART_sendByte(g_wrong_password);/*send wrong pass times to HMI for Eroor message displaying*/
			_delay_ms(2000);/*delay for uart transmission*/
			UART_receiveString(entered_pass);
			_delay_ms(1000);/*delay for uart transmission*/
			/*password match checking for door opening*/
			match = Check_Password_Match(entered_pass);
			if(match)
			{
				g_wrong_password=0;
				Open_Door();/*open the door*/
			}
			else
			{
				/*if wrong time get 3 times (0,1,2) eraise an error message*/
				g_wrong_password++;
				if(g_wrong_password == 3)
				{
					g_buzzerflag=1;/*to begin buzzer loop*/
					/*
					 * initialize timer1
					 */
					TIMER_Configuration t_config={timer1,CMP_Mode,0,58594,F_cpu_1024,0};
					TIMER_setCallback(Buzzer_Stop,CMP_Mode,timer1);
					TIMER_init(&t_config);
					/*
					 * if 1 min passed ---> flag =0 --> loop terminated
					 */
					while(g_buzzerflag == 1)
					{
						BUZZER_ON();/*turn on the buzzer*/
					}
					g_wrong_password=0;
					BUZZER_OFF();/*turn on the buzzer*/
				}
			}
		}
	}
	return 0;
}
