 /******************************************************************************
 *
 * Module: Timer Driver
 *
 * File Name: timer.c
 *
 * Description: functions definition for timer
 *
 * Author: Abdelrhman Adel
 *
 *******************************************************************************/


#include "timer.h"
#include "gpio.h"
#include "common_macros.h"
#include <AVR/io.h>
#include <AVR/interrupt.h>



/*
 * Global pointers to functions for callback functions handling
 * Initialized by Null pointer
 */
static volatile void (*g_callBackPtr_timer0)(void) = NULL_PTR;
static volatile void (*g_callBackPtr_timer1)(void) = NULL_PTR;
static volatile void (*g_callBackPtr_timer2)(void) = NULL_PTR;
/****************************************************************************
 *                             Timers ISRs                                   *
 ****************************************************************************/
ISR((TIMER0_OVF_vect))
{
	if(g_callBackPtr_timer0 != NULL_PTR)
	{
		/* Call the Call Back function in the application after the over flow occur */
		(*g_callBackPtr_timer0)();
	}
}
ISR(TIMER0_COMP_vect)
{
	if(g_callBackPtr_timer0 != NULL_PTR)
	{
		/* Call the Call Back function in the application after the compare match occur */
		(*g_callBackPtr_timer0)();
	}
}
ISR(TIMER1_OVF_vect)
{
	if(g_callBackPtr_timer1 != NULL_PTR)
	{
		/* Call the Call Back function in the application after the over flow occur */
		(*g_callBackPtr_timer1)();
	}
}
ISR(TIMER1_COMPA_vect)
{
	if(g_callBackPtr_timer1 != NULL_PTR)
	{
		/* Call the Call Back function in the application after the compare match occur */
		(*g_callBackPtr_timer1)();
	}
}
ISR((TIMER2_OVF_vect))
{
	if(g_callBackPtr_timer2 != NULL_PTR)
	{
		/* Call the Call Back function in the application after the over flow occur */
		(*g_callBackPtr_timer2)();
	}
}
ISR(TIMER2_COMP_vect)
{
	if(g_callBackPtr_timer2 != NULL_PTR)
	{
		/* Call the Call Back function in the application after the compare match occur */
		(*g_callBackPtr_timer2)();
	}
}
/****************************************************************************/

/****************************************************************************
 *                             Timers Functions                                   *
 ****************************************************************************/
void  TIMER_init(TIMER_Configuration * Configuration)
{
	/*Check which timer of the three timers in AVR MC*/

	/*Timer0*/
	if(Configuration->id == 0)
	{
		/*Check which mode of the three modes in timer*/
		if((Configuration->mode) == Normal_Mode)
		{
			/*initialize timer*/
			TCNT0=Configuration->initial_value;
			/*enable interrupt*/
			TIMSK |= (1<<TOIE0);
			/*Non-PWM*/
			TCCR0 |= (1<<FOC0);
			/*inserting prescaler value in 3 LSB (CS02,CS01,CS00) */
			TCCR0 = (TCCR0 & 0xF8) | (Configuration->Freq_prescaler & 0x07);
		}
		else if((Configuration->mode) == CMP_Mode)
		{
			/*initialize timer*/
			TCNT0=Configuration->initial_value;
			/*set compare value for compare mode*/
			OCR0=Configuration->cmp_value;
			/*activate Non-PWM,CTC mode*/
			TCCR0 |= (1<<FOC0) | (1<<WGM01);
			/*inserting prescaler value in 3 LSB (CS02,CS01,CS00) */
			TCCR0 = (TCCR0 & 0xF8) | (Configuration->Freq_prescaler & 0x07);
			/*enable interrupt*/
			TIMSK |= (1<<OCIE0);
		}
		else if((Configuration->mode) == Fast_PWM_Mode)
		{
			/*initialize timer*/
			TCNT0=Configuration->initial_value;
			/*set PB3 output pin to get the output pulses*/
			GPIO_setupPinDirection(PORTB_ID,PIN3_ID,PIN_OUTPUT);
			/*set compare value in PWM mode*/
			OCR0= Configuration->set_duty;
			/*activate Fast_PWM mode and clear OC0 on compare match*/
			TCCR0 |= (1<<WGM01) | (1<<WGM00) | (1<<COM01);
			/*inserting prescaler value in 3 LSB (CS02,CS01,CS00) */
			TCCR0 = (TCCR0 & 0xF8) | (Configuration->Freq_prescaler & 0x07);
		}
	}

	/*Timer1*/
	else if(Configuration->id == 1)
	{
		/*Check which mode of the three modes in timer*/
		if((Configuration->mode) == Normal_Mode)
		{
			/*initialize timer*/
			TCNT1=Configuration->initial_value;
			/*enable interrupt*/
			TIMSK |= (1<<TOIE1);
			/*Non-PWM for channel A and channel B*/
			TCCR1A |= (1<<FOC1A);
			/*activate normal mode by default values 0*/
			/*inserting prescaler value in 3 LSB (CS02,CS01,CS00) */
			TCCR1B = (TCCR1B & 0xF8) | (Configuration->Freq_prescaler & 0x07);
		}
		else if((Configuration->mode) == CMP_Mode)
		{
			/*initialize timer*/
			TCNT1=Configuration->initial_value;
			/*set compare value in compare mode*/
			OCR1A= Configuration->cmp_value;
			/*Non-PWM for channel A and channel B*/
			TCCR1A |= (1<<FOC1A);
			/*activate CTC mode*/
			TCCR1B = (1<<WGM12);
			/*inserting prescaler value in 3 LSB (CS02,CS01,CS00) */
			TCCR1B = (TCCR1B & 0xF8) | (Configuration->Freq_prescaler & 0x07);
			/*enable interrupt*/
			TIMSK |= (1<<OCIE1A);
		}
		else if((Configuration->mode) == Fast_PWM_Mode)
		{
			/*initialize timer*/
			TCNT1=Configuration->initial_value;
			/*set PD5 output pin to get the output pulses*/
			GPIO_setupPinDirection(PORTD_ID,PIN5_ID,PIN_OUTPUT);
			/*set compare value in PWM mode*/
			OCR1A= Configuration->set_duty;
			/*activate Fast PWM mode with OC1A output pulses*/
			TCCR1A |= (1 <<  WGM11) | (1 <<  WGM10) ; /*Register TCCR1A*/
			TCCR1B |= (1 <<  WGM13) | (1 <<  WGM12) ; /*Register TCCR1B*/
			/*inserting prescaler value in 3 LSB (CS02,CS01,CS00) */
			TCCR1B = (TCCR1B & 0xF8) | (Configuration->Freq_prescaler & 0x07);
		}
	}

	/*Timer2*/
	else if(Configuration->id == 2)
	{
		/*Check which mode of the three modes in timer*/
		if((Configuration->mode) == Normal_Mode)
		{
			/*initialize timer*/
			TCNT2=Configuration->initial_value;
			/*Non-PWM*/
			TCCR2 |= (1<<FOC2);
			/*inserting prescaler value in 3 LSB (CS22,CS21,CS20) */
			TCCR2 = (TCCR2 & 0xF8) | (Configuration->Freq_prescaler & 0x07);
			/*enable interrupt*/
			TIMSK |= (1<<TOIE2);
		}
		else if((Configuration->mode) == CMP_Mode)
		{
			/*initialize timer*/
			TCNT2=Configuration->initial_value;
			/*set compare value for compare mode*/
			OCR2=Configuration->cmp_value;
			/*activate Non-PWM,CTC mode*/
			TCCR2 |= (1<<FOC2) | (1<<WGM21);
			/*inserting prescaler value in 3 LSB (CS22,CS21,CS20) */
			TCCR2 = (TCCR2 & 0xF8) | (Configuration->Freq_prescaler & 0x07);
			/*enable interrupt*/
			TIMSK |= (1<<OCIE2);
		}
		else if((Configuration->mode) == Fast_PWM_Mode)
		{
			/*initialize timer*/
			TCNT2=Configuration->initial_value;
			/*set PD7 output pin to get the output pulses*/
			GPIO_setupPinDirection(PORTD_ID,PIN7_ID,PIN_OUTPUT);
			/*set compare value in PWM mode*/
			OCR2= Configuration->set_duty;
			/*activate Fast_PWM mode and clear OC0 on compare match*/
			TCCR2 |= (1<<WGM21) | (1<<WGM20) | (1<<COM21);
			/*inserting prescaler value in 3 LSB (CS22,CS21,CS20) */
			TCCR2 = (TCCR2 & 0xF8) | (Configuration->Freq_prescaler & 0x07);
		}

	}
}
void TIMER_Deinit (Timer_ID id)
{
	/*Check which timer of the three timers in AVR MC*/

	/*Timer0*/
	if(id == 0)
	{
		/*Clear TCCR0 register*/
		TCCR0 = 0x00;
		/*Clear TCNT0 register*/
		TCNT0= 0;
		/*Clear OCR0 register*/
		OCR0=0;
	}

	/*Timer1*/
	else if(id == 1 )
	{
		/*Clear TCCR1A register*/
		TCCR1A = 0x00;
		/*Clear TCCR1B register*/
		TCCR1B = 0x00;
		/*Clear TCNT1 register*/
		TCNT1= 0;
		/*Clear OCR1A register*/
		OCR1A=0;
		/*Clear OCR1B register*/
		OCR1B=0;
	}

	/*Timer2*/
	else if(id == 2)
	{
		/*Clear TCCR2 register*/
		TCCR2 = 0x00;
		/*Clear TCNT2 register*/
		TCNT2= 0;
		/*Clear OCR2 register*/
		OCR2=0;
	}
	/*Clear TIMSK register for all timers interrupt*/
	TIMSK=0x00;
}
void TIMER_setCallback(void(*a_ptr)(),TIMER_Mode mode,Timer_ID id)
{
	/*Check which timer of the three timers in AVR MC*/

	/*Timer0*/
	if(id == 0)
	{
		g_callBackPtr_timer0  = a_ptr;
	}

	/*Timer1*/
	else if(id == 1)
	{
		g_callBackPtr_timer1  = a_ptr;
	}

	/*Timer2*/
	else if(id == 2)
	{
		g_callBackPtr_timer2  = a_ptr;
	}
}
