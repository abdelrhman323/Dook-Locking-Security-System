 /******************************************************************************
 *
 * Module: HMI - ECU
 *
 * File Name: hmi.c
 *
 * Description: main function and other assisting functions for hmi
 *
 * Author: Abdelrhman Adel
 *
 *******************************************************************************/


#include "lcd.h"
#include "keypad.h"
#include "uart.h"
#include "timer.h"
#include <AVR/io.h>
#include <string.h>
#include <util/delay.h>

/****************************************************************************
 *                  Global Variables for the file scope
 ****************************************************************************/

uint8 g_buzzerflag=0;/*flag for Error loop begin and termination*/
uint8 g_buzzerticks=0;/*ticks counter for Error eraise time calculation*/

/****************************************************************************
 *              Assisting functions for HMI MC to do it's function          *
 ****************************************************************************/


uint8 IfMatch(uint8* str1, uint8* str2)
{
	/*
	 * Description : function check for passwords strings matching
	 * Synchronous Function
	 * Input : two passwords strings
	 * Output: 0 (not match)  or 1 (match)
	 */
	uint8 match=1; /*assume they match firstly */
	uint8 idx; /*idx for password looping*/
	for(idx=0; str1[idx] != '='; idx++)
	{
		/*if there is a bit difference --> not match*/
		if(str1[idx] != str2[idx])
		{
			match=0;
		}
	}
	return match;
}
uint8 PasswordSetting(uint8* pass)
{
	/*
	 * Description : function setting passwords and check it from user
	 * Synchronous Function
	 * Input : one passwords strings
	 * Output: 0 (not match)  or 1 (match) (user repeated password enterence)
	 */
	uint8 idx1=0;/*for looping of first password entering*/
	uint8 idx2=0;/*for looping of second password entering*/
	uint8 password_first[20]; /*first password entering*/
	uint8 password_second[20];/*second password entering*/
	LCD_displayString("enter new pass:"); /*take password from user */
	/*new line*/
	LCD_moveCursor(1,0);
	/*writing encoded characters in '*' form*/
	while(password_first[idx1-1] != '=')
	{
		/*take inputs for first time*/
		password_first[idx1]=KEYPAD_getPressedKey();
		if((password_first[idx1]-'0') <= 9 && (password_first[idx1]-'0') >= 0)
			LCD_displayCharacter('*');
		_delay_ms(4000);/*delay for keypad*/
		idx1++; /*increment index*/
	}
	password_first[idx1]='#';/*for UART transmission*/
	LCD_clearScreen(); /*clean the screen for second time*/
	LCD_displayString("Re-enter pass:");/*enter password again*/
	LCD_moveCursor(1,0);/*new line*/
	/*take password and encode it for second time*/
	while(password_second[idx2-1] != '=')
	{
		password_second[idx2]=KEYPAD_getPressedKey();
		if((password_second[idx2]-'0') <= 9 && (password_second[idx2]-'0') >= 0)
			LCD_displayCharacter('*');
		_delay_ms(4000);
		idx2++;
	}
	strcpy(pass,password_first);/*store password to send it to control MC*/
	return IfMatch(password_first,password_second);/*check if password entered correctly or not*/
}
uint8 Main_Options()
{
	/*
	 * Description : function display and get the main option
	 * Synchronous Function
	 * Input : void
	 * Output: uint8 option (+ / -)
	 */
	uint8 option =0; /*to stor the input option*/
	LCD_displayString("+ :Open the door");/*open the door*/
	LCD_moveCursor(1,0);/*new line*/
	LCD_displayString("- :Change Pass    ");/*change the password*/
	option = KEYPAD_getPressedKey();/*get the input option*/
	_delay_ms(4000);/*delay for keypad*/
	return option;
}
void Enter_Password(uint8* password)
{
	/*
	 * Description : function handle password entering
	 * Asynchronous Function
	 * Input : one passwords strings
	 * Output: void
	 */
	uint8 idx=0;/*looping on password entering and encoding*/
	LCD_displayString("enter pass:");/*ask  for password entering*/
	LCD_moveCursor(1,0);/*new line*/
	/*take the password from user*/
	while(password[idx-1] != '=')
	{
		password[idx]=KEYPAD_getPressedKey();
		if((password[idx]-'0') <= 9 && (password[idx]-'0') >= 0)
			LCD_displayCharacter('*');
		_delay_ms(4000);
		idx++;
	}
	password[idx]='#';/*for UART transmission*/
	LCD_clearScreen();
}
void Buzzer_Stop()
{
	/*
	 * Description : CallBack function for Error in case 3 times wrong passwords
	 * Asynchronous Function
	 * Input : Void
	 * Output: Void
	 */
	g_buzzerticks++; /*Counting ticks for 1 min Error displaying time calculations*/
	if(g_buzzerticks == 8)
	{
		g_buzzerflag=0;/*reset flag for loop termination*/
		g_buzzerticks=0;/*reset ticks counter*/
		TIMER_Deinit(timer1);/*reset timer for another usage*/
	}

}
void Change_Pass(uint8 flag)
{
	/*
	 * Description : function password changing
	 * Synchronous Function
	 * Input : Void
	 * Output: Void
	 */
	uint8 error; /*to store wrong password times received from control MC*/
	uint8 match=0;/*for store if matching or not*/
	uint8 entered_pass[15];/*to store entered password*/
	uint8 password_real[15];/*to store new password*/
	error = UART_receiveByte(); /*receive the wrong password times */
	_delay_ms(200); /*delay for receiving from UART*/
	LCD_clearScreen();
	Enter_Password(entered_pass);/*taking the password*/
	UART_sendString(entered_pass);/*sending it */
	_delay_ms(2000); /*delay for UART transmission*/
	match = UART_receiveByte(); /*recieve if match the or not from control MC*/
	_delay_ms(500);
	if(match == 1)
	{
		/*if match do the change*/
		if(PasswordSetting(password_real))
		{
			UART_sendString(password_real);
			LCD_clearScreen();
		}
		else
		{
			/*if not ask for password again*/
			PasswordSetting(password_real);
		}

	}
	else
	{
		/*if the wrong times get 3 times 0,1,2 */
		if(error == 2)
		{
			g_buzzerflag=1;/*Set the flag to enter the loop*/
			/*Turn on the timer to count 1 Min for Error !!*/
			TIMER_Configuration config={timer1,CMP_Mode,0,58594,F_cpu_1024,0};
			TIMER_setCallback(Buzzer_Stop,CMP_Mode,timer1);
			TIMER_init(&config);
			LCD_clearScreen();
			while(g_buzzerflag == 1)
			{
				LCD_moveCursor(0,4);
				LCD_displayString("ERROR!!!");
				_delay_ms(500);
			}
			LCD_clearScreen();
		}
	}
}
int main(void)
{
	uint8 flag=0; /*flag for password handle for one time*/
	uint8 option=0; /*to store option*/
	uint8 error;/*to store wrong times received from control MC*/
	uint8 password_real[15];/*to get the password first time as an admin*/
	uint8 pass[15];/*to store the entered password from user each timer*/
	SREG |= (1<<7);/*enable interrupts for timer*/
	/*configure UART*/
	UART_Configuration  configuration={eight_bits,disable,one_bit,9600};
	LCD_init();/*initialize LCD*/
	_delay_ms(100);/*delay for LCD initialization*/
	UART_init(&configuration);
	/*reset password as an admin*/
	while(flag == 0)
	{
		if(PasswordSetting(password_real))
			flag=1;
		LCD_clearScreen();
	}
	/*send the first password to control MC to store it*/
	UART_sendString(password_real);
	while(1)
	{
		/*get the  option from user*/
		option = Main_Options();
		if(option == '+')
		{
			/*if '+' ask for password for opening the door*/
			UART_sendByte(option);
			_delay_ms(2000);/*delay for option transmission*/
			LCD_clearScreen();
			Enter_Password(pass);/*get password from user*/
			error = UART_receiveByte();/*receive wrong pass times*/
			_delay_ms(2000);
			UART_sendString(pass);/*send the entered pass for checking in control MC*/
			_delay_ms(1500);/*delay for transmission*/
			/*if wrong time get 3 times (0,1,2) eraise an error message*/
			if(error == 2)
			{
				g_buzzerflag=1;
				TIMER_Configuration config={timer1,CMP_Mode,0,58594,F_cpu_1024,0};
				TIMER_setCallback(Buzzer_Stop,CMP_Mode,timer1);
				_delay_ms(1000);
				TIMER_init(&config);
				LCD_clearScreen();
				while(g_buzzerflag == 1)
				{
					LCD_moveCursor(0,4);
					LCD_displayString("ERROR!!!");
					_delay_ms(500);
				}
				LCD_clearScreen();
			}
		}
		else if(option == '-')
		{
			/*
			 * if the option = '-' .....> change the password
			 */
			UART_sendByte(option);
			_delay_ms(1000);
			flag=0;
			Change_Pass(flag);
		}

	}
	return 0;
}
