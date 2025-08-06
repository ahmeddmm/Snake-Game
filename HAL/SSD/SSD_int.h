/*
 * SSD_int.h
 *
 *  Created on: May 17, 2022
 *      Author: ahmed
 */

#ifndef HAL_SSD_SSD_INT_H_
#define HAL_SSD_SSD_INT_H_

#define COM_ANODE 0
#define COM_CATHODE 1

#define SSD_TYPE COM_ANODE
#define SSD_PORT DIO_PORTA

void SSD_vInit(void);
void SSD_vDisplayNum(u8 A_u8Num);
void SSD_vTurnOff(void);




#endif /* HAL_SSD_SSD_INT_H_ */
