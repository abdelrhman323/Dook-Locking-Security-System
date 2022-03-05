 /******************************************************************************
 *
 * Module: Motor
 *
 * File Name: motor.c
 *
 * Description: functions prototypes for motor
 *
 * Author: Abdelrhman Adel
 *
 *******************************************************************************/


#ifndef MOTOR_H_
#define MOTOR_H_

#include "std_types.h"

/*******************************************
 *         Definitions            *
 ******************************************/
/*
 * Description:
 *      this enum for the motor rotation state so it contains:
 *               -stop :0 >> motor not moving
 *               -A_CW :1 >> motor moves anti-clock wise
 *               -CW   :2 >> motor moves      clock wise
 */
typedef enum{
	stop,A_CW,CW
}DcMotor_State;

/*
 * definitions of motor pins and port in MC
 */
#define MOTOR_PORT_ID     PORTB_ID
#define MOTOR_FIRST_PIN   PIN0_ID
#define MOTOR_SECOND_PIN  PIN1_ID
#define MOTOR_ENABLE_PIN  PIN3_ID
/*******************************************
 *         Functions Prototypes            *
 ******************************************/
/* Description:
 *       The Function responsible for setup the direction for the two
 *      motor pins through the GPIO driver.
        Stop at the DC-Motor at the beginning through the GPIO driver.
 * Inputs: None
 * Return: None
 */
void DcMotor_Init(void);
/*
 * Description:
	* 	The function responsible for rotate the DC Motor CW/ or A-CW or
	* 	stop the motor based on the state input state value.
	* 	Send the required duty cycle to the PWM driver based on the
	* 	required speed value.
	* Inputs:
	*	 state: The required DC Motor state, it should be CW or A-CW or stop.
	*  	DcMotor_State data type should be declared as enum or uint8.
	*  	speed: decimal value for the required motor speed, it should be from
	*  	0 : 100. For example, if the input is 50, The motor should rotate with
	*  	50% of its maximum speed.
	* Return: None
 */

void DcMotor_Rotate(DcMotor_State);


#endif /* MOTOR_H_ */
