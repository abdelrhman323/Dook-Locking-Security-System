 /******************************************************************************
 *
 * Module: Motor
 *
 * File Name: motor.c
 *
 * Description: functions definitions for motor
 *
 * Author: Abdelrhman Adel
 *
 *******************************************************************************/

#include "motor.h"
#include "timer.h"
#include <AVR/io.h>
#include "gpio.h"


TIMER_Configuration config={timer0,Fast_PWM_Mode,0,2000,F_cpu_8,255};

void DcMotor_Init()
{
	/*
	 * setup the motor pin directions
	 */
	GPIO_setupPinDirection(MOTOR_PORT_ID,MOTOR_FIRST_PIN,PIN_OUTPUT); //set PB0 as output pin --> pin where direction will be controlled.
	GPIO_setupPinDirection(MOTOR_PORT_ID,MOTOR_SECOND_PIN,PIN_OUTPUT); //set PB1 as output pin --> pin where direction will be controlled..
	/*
	 * Stop the motor at the beginning
	 */
	GPIO_writePin(MOTOR_PORT_ID,MOTOR_FIRST_PIN,LOGIC_LOW);
	GPIO_writePin(MOTOR_PORT_ID,MOTOR_SECOND_PIN,LOGIC_LOW);

	TIMER_init(&config);
}
void DcMotor_Rotate(DcMotor_State state)
{
	/*
	 * this switch case for controlling the motor state
	 *  stop: motor will stop
	 *  A_CW: motor will rotate anti clock wise
	 *  CW  : motor will rotate      clock wise
	 */
	switch (state)
	{
	case stop:
		GPIO_writePin(MOTOR_PORT_ID,MOTOR_FIRST_PIN,LOGIC_LOW);
		GPIO_writePin(MOTOR_PORT_ID,MOTOR_SECOND_PIN,LOGIC_LOW);
		break;
	case A_CW:
		GPIO_writePin(MOTOR_PORT_ID,MOTOR_FIRST_PIN,LOGIC_LOW);
		GPIO_writePin(MOTOR_PORT_ID,MOTOR_SECOND_PIN,LOGIC_HIGH);
		break;
	case CW:
		GPIO_writePin(MOTOR_PORT_ID,MOTOR_FIRST_PIN,LOGIC_HIGH);
		GPIO_writePin(MOTOR_PORT_ID,MOTOR_SECOND_PIN,LOGIC_LOW);
		break;
	}

}
