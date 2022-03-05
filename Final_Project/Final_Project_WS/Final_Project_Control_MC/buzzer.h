 /******************************************************************************
 *
 * Module: Buzzer Driver
 *
 * File Name: buzzer.h
 *
 * Description: functions prtotypes for buzzer
 *
 * Author: Abdelrhman Adel
 *
 *******************************************************************************/

#ifndef BUZZER_H_
#define BUZZER_H_

#include "std_types.h" /*for typedefs*/
#include "gpio.h"

/**************************************************************************
 *                    Buzzer Positions on MC Definitions                  *
 **************************************************************************/
#define BUZZER_PORT_ID     PORTA_ID
#define BUZZER_PIN_ID      PIN0_ID

extern uint8 error;
/**************************************************************************
 *                           Functions Prototypes                         *
 **************************************************************************/
/*
 * Description : this function initialize buzzer Peripheral
 * Asynchronous Function
 * Input : Void
 * Output: Void
 */
void BUZZER_init(void);
/*
 * Description : this function switch on  the buzzer
 * Synchronous Function
 * Input : Void
 * Output: Void
 */
void BUZZER_ON(void);
/*
 * Description : this function switch off the buzzer
 * Synchronous Function
 * Input : Void
 * Output: Void
 *
 */
void BUZZER_OFF(void);

#endif /* BUZZER_H_ */
