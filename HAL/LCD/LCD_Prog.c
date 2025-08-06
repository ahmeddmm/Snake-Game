/*
 * LCD_Prog.c
 *
 *  Created on: May 19, 2022
 *      Author: ahmed
 */

#include"../../LIB/STD_TYPES.h"
#include"../../MCAL/DIO/DIO_int.h"
#include"LCD_int.h"
#include<util/delay.h>


void HLCD_vinit(void){
	/* set pin directuion*/
	DIO_vSetPortDir(LCD_DATA_PORT,0xff);
	DIO_vSetPinDir(LCD_CTRL_PORT,LCD_RS_PIN,DIO_HIGH);
	DIO_vSetPinDir(LCD_CTRL_PORT,LCD_RW_PIN,DIO_HIGH);
	DIO_vSetPinDir(LCD_CTRL_PORT,LCD_EN_PIN,DIO_HIGH);
	_delay_ms(50);
	/* start init sequence*/
	HLCD_vSendCommand(0b00111100);_delay_ms(1);
	HLCD_vSendCommand(0b00001100);_delay_ms(1);
	HLCD_vSendCommand(0b00000001);_delay_ms(3);
	HLCD_vSendCommand(0b00000110);_delay_ms(3);
}
void HLCD_vSendCommand(u8 A_u8Command){
	DIO_vSetPinVal(LCD_CTRL_PORT,LCD_RS_PIN,DIO_LOW);//rs
	DIO_vSetPinVal(LCD_CTRL_PORT,LCD_RW_PIN,DIO_LOW);//RW

	DIO_vSetPortVal(LCD_DATA_PORT,A_u8Command);

	DIO_vSetPinVal(LCD_CTRL_PORT,LCD_EN_PIN,DIO_HIGH);_delay_ms(1);
	DIO_vSetPinVal(LCD_CTRL_PORT,LCD_EN_PIN,DIO_LOW) ;_delay_ms(1);
}
void HLCD_vSendChar(u8 A_u8Char){
	DIO_vSetPinVal(LCD_CTRL_PORT,LCD_RS_PIN,DIO_HIGH);//rs
	DIO_vSetPinVal(LCD_CTRL_PORT,LCD_RW_PIN,DIO_LOW);//RW

	DIO_vSetPortVal(LCD_DATA_PORT,A_u8Char);

	DIO_vSetPinVal(LCD_CTRL_PORT,LCD_EN_PIN,DIO_HIGH);_delay_ms(1);
	DIO_vSetPinVal(LCD_CTRL_PORT,LCD_EN_PIN,DIO_LOW) ;_delay_ms(1);

}

void HLCD_vPrintString(char A_s8String[]){

	for(u8 L_u8i=0;A_s8String[L_u8i] != '\0';L_u8i++){
		HLCD_vSendChar(A_s8String[L_u8i]);
	}
}

void HLCD_vPrintNum(s32 A_s32Number){
	u8 L_digits[10]={0};
	u8 L_u8Counter=0;
	while(A_s32Number>0){
	//	A_s32Number%10;
		L_digits[L_u8Counter]=A_s32Number;
		L_u8Counter++;
		A_s32Number/=10;

	}
	/*print array in reverse*/
	for(s8 L_s8PrintIndx=0;L_s8PrintIndx>=0;L_s8PrintIndx--){
		HLCD_vSendChar('0'+L_digits[L_s8PrintIndx]);
	}
}
void HLCD_vReturnCursorHome(void){
	HLCD_vSendCommand(0x80);_delay_ms(1);
}
void HLCD_vSetCursorAtx_y(u8 A_u8PosY,u8 A_u8PosX){
	if (A_u8PosY == 0 && A_u8PosX<16 && A_u8PosX>=0)
		HLCD_vSendCommand((A_u8PosX & 0x0F)|0x80);	/* Command of first row and required position<16 */
	else if (A_u8PosY == 1 && A_u8PosX<16 && A_u8PosX>=0)
		HLCD_vSendCommand((A_u8PosX & 0x0F)|0xC0);	/* Command of first row and required position<16 */
}
void HLCD_vWrightCustomChar(u8 A_u8PatternIdx,u8 *A_pu8PatternValue){
	u8 L_u8LoopIterator;
	switch(A_u8PatternIdx){
	case 0:HLCD_vSendCommand(0x40);break;
	case 1:HLCD_vSendCommand(0x48);break;
	case 2:HLCD_vSendCommand(0x50);break;
	case 3:HLCD_vSendCommand(0x58);break;
	case 4:HLCD_vSendCommand(0x60);break;
	case 5:HLCD_vSendCommand(0x68);break;
	case 6:HLCD_vSendCommand(0x70);break;
	case 7:HLCD_vSendCommand(0x78);break;

	}
	for(L_u8LoopIterator=0;L_u8LoopIterator<8;L_u8LoopIterator++){
		HLCD_vSendChar(A_pu8PatternValue[L_u8LoopIterator]);
	}
}
/*
void HLCD_vSetCursorPosition(u8 A_u8LinePos,u8 A_u8LinenNo){

}
*/
void HLCD_vSaveCustom(u8 A_u8Index){


}
//void HLCD_vDisplayCustom();
