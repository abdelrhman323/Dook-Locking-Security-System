 /******************************************************************************
 *
 * Module: Keypad Driver
 *
 * File Name: keypad.c
 *
 * Description: functions prototypes for keypad
 *
 * Author: Abdelrhman Adel
 *
 *******************************************************************************/


#ifndef KEYPAD_H_
#define KEYPAD_H_

#include "std_types.h"
/************************************************
 *           Keypad Configurations          *
 ************************************************/

/**********Port configuration*********/

#define KEYPAD_PORT            PORTA_ID

#define KEYPAD_FIRST_PIN_COLs  PIN4_ID
#define KEYPAD_FIRST_PIN_ROWs  PIN0_ID

/*********Number of Rows and Columns***********/

#define KEYPAD_NUM_OF_COLs     4
#define KEYPAD_NUM_OF_ROWs     4

/***************Keys Pressing Logic****************/
#define KEYPAD_KEY_PRESSED     LOGIC_LOW
#define KEYPAD_KEY_RELESED     LOGIC_HIGH
/**************************************************
 *             Functions Prototypes               *
 *************************************************/
/*
 * Description:
 * Get the ASCII of pressed key
 */
uint8 KEYPAD_getPressedKey(void);
#endif /* KEYPAD_H_ */
