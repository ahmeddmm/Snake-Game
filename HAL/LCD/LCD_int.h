/*
 * LCD_int.h
 *
 *  Created on: May 19, 2022
 *      Author: ahmed
 */

#ifndef HAL_LCD_LCD_INT_H_
#define HAL_LCD_LCD_INT_H_

#define LCD_CTRL_PORT  		DIO_PORTA
#define LCD_DATA_PORT  		DIO_PORTD
#define LCD_RS_PIN     		DIO_PIN0
#define LCD_RW_PIN     		DIO_PIN1
#define LCD_EN_PIN     		DIO_PIN2

#define MAX_IDX_OF_ROS 		1
#define	MAX_IDX_OF_COL 		15
#define FIRST_ROW_INDEX 	0
#define	FIRST_ROW_START 	0X00
#define SECOND_ROW_START	0X40
#define SET_AC_MASK 		0x80


void HLCD_vinit(void);
void HLCD_vSendCommand(u8 A_u8Command);
void HLCD_vSendChar(u8 A_u8Char);
void HLCD_vPrintString(char A_s8String[]);
void HLCD_vPrintNum(s32 A_s32Number);
void HLCD_vSetCursorPosition(u8 A_u8LinePos,u8 A_u8LinenNo);
void HLCD_vReturnCursorHome(void);
void HLCD_vSetCursorAtx_y(u8 A_u8PosX,u8 A_u8Posy);
void HLCD_vWrightCustomChar(u8 A_u8PatternIdx,u8 *A_pu8PatternValue);
//void HLCD_vSaveCustom();
void HLCD_vDisplayCustom();
//void HLCD_ClearScrean();
//void Scroll();

#endif /* HAL_LCD_LCD_INT_H_ */
