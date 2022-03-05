 /******************************************************************************
 *
 * Module: Timer Driver
 *
 * File Name: timer.h
 *
 * Description: functions prototypes for timer
 *
 * Author: Abdelrhman Adel
 *
 *******************************************************************************/


#ifndef TIMER_H_
#define TIMER_H_

#include "std_types.h"  /* for typedefs */

/***************************************************************************
 *                Timer_ID Enumeration  Definition
 ***************************************************************************/
/*
 * Enumeration Name: Timer_ID
 * 			this is enumeration for one of the three timers
 * Members:
 *        - timer0 -----> 0
 * 		  -	timer1 -----> 1
 * 		  - timer2 -----> 2
 */
typedef enum
{
	timer0,timer1,timer2
}Timer_ID;
/**************************************************************************
 *             Enumeration for Timer Prescale
 **************************************************************************/
/*
 * Enumeration Name: TIMER_Mode
 * Members:
 * 			- Normal_Mode : overflow mode which counts the ticks till overflow again to initial value
 * 			- CMP_Mode    : compare  mode which compare the counter with compare value until they match each other
 * 			- PWM_Mode    : pulse width modulation mode, used as analog input to analog peripherals
 */
typedef enum
{
	Normal_Mode,  CMP_Mode,  Fast_PWM_Mode

}TIMER_Mode;


/**************************************************************************
 *             Enumeration for Timer Prescale
 **************************************************************************/
/*
 * Enumeration Name: TIMER_Prescale
 * Members:
 * 			- No_clock   : when there is no frequency input to timer               	(value:0)
 * 			- F_cpu_8    : when there is a frequency input to timer equal to F_CPU    (value:1)
 * 			- F_cpu_8    : when there is a frequency input to timer equal to F_CPU/8    (value:2)
 * 			- F_cpu_64   : when there is a frequency input to timer equal to F_CPU/64   (value:3)
 * 			- F_cpu_256  : when there is a frequency input to timer equal to F_CPU/256  (value:4)
 * 			- F_cpu_1024 : when there is a frequency input to timer equal to F_CPU/1024 (value:5)
 */
typedef enum
{
	No_clock,
	F_cpu,
	F_cpu_8,
	F_cpu_64,
	F_cpu_256,
	F_cpu_1024,

}TIMER_Prescale;

/**************************************************************************
 *             Structure for Timer Configuration
 **************************************************************************/
typedef struct
{
	Timer_ID 	    id;
	TIMER_Mode 		mode;
	uint8      		initial_value;
	uint16     		cmp_value;
	TIMER_Prescale  Freq_prescaler;
	uint16     		set_duty; /*for fast PWM mode if exist will be valued else will be 0*/

}TIMER_Configuration;

/**************************************************************************
 *                    Functions Prototypes
 **************************************************************************/
/*
 * Description:
 *           - This function will be used to initialize timer peripheral
 * Asynchronous Function
 * Input : TIMER_Configuration structure passed by address
 * Output: Void
 */
void TIMER_init(TIMER_Configuration *);

/*
 * Description:
 * 			- This Function is to deinit timer peripheral
 * Synchronous Function : as it deinit timer peripheral instantaneously
 * Input : Timer_ID
 * Output: Void
 */
void TIMER_Deinit (Timer_ID);

/*
 * Description:
 * 			- This Function is to deinitialize timer peripheral
 * Synchronous Function : as it deinitialize timer peripheral instantaneously
 * Input : pointer to void function & TIMER_Mode & Timer_ID
 * Output: Void
 */
void TIMER_setCallback(void(*)(void),TIMER_Mode,Timer_ID);
#endif /* TIMER_H_ */
