/*
 * KPD_prog.c
 *
 *  Created on: May 17, 2022
 *      Author: ahmed
 */

#include"../../LIB/STD_TYPES.h"
#include"../../MCAL/DIO/DIO_int.h"
#include"KPD_int.h"
#include<util/delay.h>
u8 keypad[4][4]={{'7','8','9','/'},
								{'4','5','6','*'},
								{'1','2','3','-'},
								{'o','0','=','+'}
												};


void KPD_vInit(void){
	DIO_vSetPortDir(KPD_PORT,0x0f);
	DIO_vSetPortVal(KPD_PORT,0xff);


}
u8 KPD_u8GetPressedKey(void){
	u8 col=0,row=0,pressed_key=0;
//	u8 u8Pressed_key=0;
	for(col=0;col<4;col++){
					DIO_vSetPinVal(KPD_PORT,col+4,DIO_LOW);
					for(row=0;row<4;row++){
						//pressed_key=3;
						if(DIO_u8GetPinVal(KPD_PORT, row) == 0){
							//pressed_key=row+(col*4)+1;
							pressed_key=(keypad[row][col])-'0';
					    }
						while(DIO_u8GetPinVal(KPD_PORT, row) == 0);
									_delay_ms(10);
					}
					DIO_vSetPinVal(KPD_PORT,col+4,DIO_HIGH);
			}
	return pressed_key;
}
