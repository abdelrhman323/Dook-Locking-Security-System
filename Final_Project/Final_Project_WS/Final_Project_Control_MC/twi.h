 /******************************************************************************
 *
 * Module: Two Wire Interface (I2C)
 *
 * File Name: twi.h
 *
 * Description: functions prototypes for TWI
 *
 * Author: Abdelrhman Adel
 *
 *******************************************************************************/


#ifndef TWI_H_
#define TWI_H_

#include "std_types.h" /*for typedefs*/

/*******************************************************************************
 *                      Bit Rate Enumeration                                   *
 *******************************************************************************/
typedef enum
{
	Normal_Mode,Fast_Mode,Fast_Plus_Mode,High_Speed_Mode
}BitRate;
/*******************************************************************************
 *                      Configuration Structure                                *
 *******************************************************************************/
typedef struct
{
	uint8   slave_adress;
	BitRate rate;
}TWI_ConfigurationStruct;
/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/

/* I2C Status Bits in the TWSR Register */
#define TWI_START         0x08 /* start has been sent */
#define TWI_REP_START     0x10 /* repeated start */
#define TWI_MT_SLA_W_ACK  0x18 /* Master transmit ( slave address + Write request ) to slave + ACK received from slave. */
#define TWI_MT_SLA_R_ACK  0x40 /* Master transmit ( slave address + Read request ) to slave + ACK received from slave. */
#define TWI_MT_DATA_ACK   0x28 /* Master transmit data and ACK has been received from Slave. */
#define TWI_MR_DATA_ACK   0x50 /* Master received data and send ACK to slave. */
#define TWI_MR_DATA_NACK  0x58 /* Master received data but doesn't send ACK to slave. */

/*******************************************************************************
 *                      Functions Prototypes                                    *
 *******************************************************************************/
void TWI_init(TWI_ConfigurationStruct*);
void TWI_start(void);
void TWI_stop(void);
void TWI_writeByte(uint8);
uint8 TWI_readByteWithACK(void);
uint8 TWI_readByteWithNACK(void);
uint8 TWI_getStatus(void);

#endif /* TWI_H_ */
