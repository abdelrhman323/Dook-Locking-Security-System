 /******************************************************************************
 *
 * Module: Keypad Driver
 *
 * File Name: keypad.c
 *
 * Description: functions definition for keypad
 *
 * Author: Abdelrhman Adel
 *
 *******************************************************************************/

#include "Keypad.h"
#include "gpio.h"

#if(KEYPAD_NUM_OF_COLs == 3)
/*
 * Description:
 *	return corresponding value of ASCII of pressed key in 4x3 keypad
 */
static uint8 KEYPAD_adjustkey_4x3(uint8);

#elif(KEYPAD_NUM_OF_COLs == 4)
/*
 * Description:
 *	return corresponding value of ASCII of pressed key in 4x4 keypad
 */
static uint8 KEYPAD_adjustkey_4x4(uint8);

#endif

uint8 KEYPAD_getPressedKey()
{
	uint8 row,col;
	uint8 keypad_pressed_value=0;
	while(1)
	{
		uint8 fl=0;
		for(col=0; col < KEYPAD_NUM_OF_COLs; col++)
		{
			GPIO_setupPortDirection(KEYPAD_PORT,PORT_INPUT);
			GPIO_setupPinDirection(KEYPAD_PORT,KEYPAD_FIRST_PIN_COLs+col,PIN_OUTPUT);
#if(KEYPAD_KEY_PRESSED == LOGIC_LOW)
			keypad_pressed_value = ~(1<<(KEYPAD_FIRST_PIN_COLs+col));
#else
			keypad_pressed_value =  (1<<(KEYPAD_FIRST_PIN_COLs+col));
#endif
			GPIO_writePort(KEYPAD_PORT,keypad_pressed_value);
			for(row=0; row < KEYPAD_NUM_OF_ROWs; row++)
			{
				if(GPIO_readPin(PORTA_ID,row)==KEYPAD_KEY_PRESSED && fl==0)
				{
					fl=1;
#if(KEYPAD_NUM_OF_COLs == 3)
					return KEYPAD_adjustkey_4x3((row*KEYPAD_NUM_OF_COLs)+col+1);
#elif(KEYPAD_NUM_OF_COLs == 4)
					return KEYPAD_adjustkey_4x4((row*KEYPAD_NUM_OF_COLs)+col+1);
#endif
				}
			}
		}
	}
}

#if(KEYPAD_NUM_OF_COLs == 3)

static uint8 KEYPAD_adjustkey_4x3(uint8 button_pressed)
{
	uint8 button_value=0;
	switch(button_pressed)
	{
	case 10:
		button_value='*';
		break;
	case 11:
		button_value=0;
		break;
	case 12:
		button_value='#';
		break;
	default:
		button_value=button_pressed;
		break;
	}
	return button_value;
}
#elif(KEYPAD_NUM_OF_COLs == 4)

static uint8 KEYPAD_adjustkey_4x4(uint8 button_pressed)
{
	uint8 button_value=0;
	switch(button_pressed)
	{
	case 1:
		button_value='7';
		break;
	case 2:
		button_value='8';
		break;
	case 3:
		button_value='9';
		break;
	case 4:
		button_value='/';
		break;
	case 5:
		button_value='4';
		break;
	case 6:
		button_value='5';
		break;
	case 7:
		button_value='6';
		break;
	case 8:
		button_value='*';
		break;
	case 9:
		button_value='1';
		break;
	case 10:
		button_value='2';
		break;
	case 11:
		button_value='3';
		break;
	case 12:
		button_value='-';
		break;
	case 13:
		button_value=13;
		break;
	case 14:
		button_value='0';
		break;
	case 15:
		button_value='=';
		break;
	case 16:
		button_value='+';
		break;
	default:
		button_value=button_pressed;
		break;
	}
	return button_value;
}
#endif
