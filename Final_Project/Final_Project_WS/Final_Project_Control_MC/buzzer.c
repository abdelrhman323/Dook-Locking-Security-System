 /******************************************************************************
 *
 * Module: Buzzer Driver
 *
 * File Name: buzzer.c
 *
 * Description: functions definition for buzzer
 *
 * Author: Abdelrhman Adel
 *
 *******************************************************************************/

#include "buzzer.h"
#include <AVR/io.h>

void BUZZER_init()
{
	/*Make direction of buzzer pin as output*/
	GPIO_setupPinDirection(BUZZER_PORT_ID,BUZZER_PIN_ID,PIN_OUTPUT);
}
void BUZZER_ON()
{
	/*Set direction of buzzer pin */
	GPIO_writePin(BUZZER_PORT_ID,BUZZER_PIN_ID,LOGIC_HIGH);
}
void BUZZER_OFF()
{
	/*Clear direction of buzzer pin as output*/
	GPIO_writePin(BUZZER_PORT_ID,BUZZER_PIN_ID,LOGIC_LOW);
}

