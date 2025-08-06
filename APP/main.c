/*
 * main.c - Snake Game for AVR Microcontroller
 *
 *  Created on: May 17, 2022
 *      Author: ahmed
 *
 *  Description: A Snake game implementation for AVR microcontroller using LCD display
 *               The game uses custom characters on LCD to display the snake and food
 *               Controls are connected to PORTB pins for directional movement
 */

// Include necessary libraries
#include"../LIB/BIT_MATH.h"      // Bit manipulation macros
#include"../LIB/STD_TYPES.h"     // Standard data types (u8, s8, etc.)
#include"../MCAL/DIO/DIO_int.h"  // Digital I/O interface
#include"../HAL/SSD/SSD_int.h"   // Seven Segment Display interface
#include"../HAL/KPD/KPD_int.h"   // Keypad interface
#include"../HAL/LCD/LCD_int.h"   // LCD interface
#include<util/delay.h>           // Delay functions

// Custom character pattern for empty/border cells
u8 snake_use[] ={0x1B, 0x11, 0x00, 0x00, 0x00, 0x11, 0x1B, 0x00};

// 3D array representing the LCD screen content including snake positions
// Format: snake[row][column][custom_char_line]
// This stores the current state of each LCD character position
u8 snake[2][16][8]      ={{  {0x1B, 0x11, 0x00, 0x00, 0x00, 0x11, 0x1B, 0x00},
							 {0x1B, 0x11, 0x00, 0x00, 0x00, 0x11, 0x1B, 0x00},
							 {0x1B, 0x11, 0x00, 0x00, 0x00, 0x11, 0x1B, 0x00},
							 {0x1B, 0x11, 0x00, 0x00, 0x00, 0x11, 0x1B, 0x00},
							 {0x1B, 0x11, 0x00, 0x00, 0x00, 0x11, 0x1B, 0x00},
							 {0x1B, 0x11, 0x00, 0x00, 0x00, 0x11, 0x1B, 0x00},
							 {0x1B, 0x11, 0x00, 0x00, 0x00, 0x11, 0x1B, 0x00},
							 {0x1B, 0x11, 0x00, 0x00, 0x00, 0x11, 0x1B, 0x00},
							 {0x1B, 0x11, 0x00, 0x00, 0x00, 0x11, 0x1B, 0x00},
							 {0x1B, 0x11, 0x00, 0x00, 0x00, 0x11, 0x1B, 0x00},
							 {0x1B, 0x11, 0x00, 0x00, 0x00, 0x11, 0x1B, 0x00},
							 {0x1B, 0x11, 0x00, 0x00, 0x00, 0x11, 0x1B, 0x00},
							 {0x1B, 0x11, 0x00, 0x00, 0x00, 0x11, 0x1B, 0x00},
							 {0x1B, 0x11, 0x00, 0x00, 0x00, 0x11, 0x1B, 0x00},
							 {0x1B, 0x11, 0x00, 0x00, 0x00, 0x11, 0x1B, 0x00},
							 {0x1B, 0x11, 0x00, 0x00, 0x00, 0x11, 0x1B, 0x00} },

							 {  {0x1B, 0x11, 0x00, 0x00, 0x00, 0x11, 0x1B, 0x00},
								{0x1B, 0x11, 0x00, 0x00, 0x00, 0x11, 0x1B, 0x00},
								{0x1B, 0x11, 0x00, 0x00, 0x00, 0x11, 0x1B, 0x00},
								{0x1B, 0x11, 0x00, 0x00, 0x00, 0x11, 0x1B, 0x00},
								{0x1B, 0x11, 0x00, 0x00, 0x00, 0x11, 0x1B, 0x00},
								{0x1B, 0x11, 0x00, 0x00, 0x00, 0x11, 0x1B, 0x00},
								{0x1B, 0x11, 0x00, 0x00, 0x00, 0x11, 0x1B, 0x00},
								{0x1B, 0x11, 0x00, 0x00, 0x00, 0x11, 0x1B, 0x00},
								{0x1B, 0x11, 0x00, 0x00, 0x00, 0x11, 0x1B, 0x00},
								{0x1B, 0x11, 0x00, 0x00, 0x00, 0x11, 0x1B, 0x00},
								{0x1B, 0x11, 0x00, 0x00, 0x00, 0x11, 0x1B, 0x00},
								{0x1B, 0x11, 0x00, 0x00, 0x00, 0x11, 0x1B, 0x00},
								{0x1B, 0x11, 0x00, 0x00, 0x00, 0x11, 0x1B, 0x00},
								{0x1B, 0x11, 0x00, 0x00, 0x00, 0x11, 0x1B, 0x00},
								{0x1B, 0x11, 0x00, 0x00, 0x00, 0x11, 0x1B, 0x00},
								{0x1B, 0x11, 0x00, 0x00, 0x00, 0x11, 0x1B, 0x00}}

																			};

// Snake position array - stores coordinates and status of each snake segment
// Format: [segment][row, column, y_offset, x_offset, active_flag]
// row: LCD row (0-1), column: LCD column (0-15)
// y_offset: position within character (0-7), x_offset: bit position (0-4)
// active_flag: 1 if segment is active, 0 if inactive
u8 u8_SnakePos[11][5]={
					{0,0,3,4,1},  // Head segment - active
					{0,0,3,3,1},  // Body segment 1 - active
					{0,0,3,2,1},  // Body segment 2 - active
					{0,0,3,1,1},  // Body segment 3 - active
					{0,0,3,0,1},  // Body segment 4 - active
					{0,0,3,0,0},  // Tail segment - inactive
					{0,0,0,0,0},  // Unused segment
					{0,0,0,0,0},  // Unused segment
					{0,0,0,0,0},  // Unused segment
					{0,0,0,0,0},  // Unused segment
					{0,0,0,0,0}   // Unused segment
						};

// Food position array - predefined food locations for the game
// Format: [food_number][row, column, y_offset, x_offset]
u8 u8_Food[11][4]={
					{0,12,3,4},  // Food position 0
					{1,14,3,3},  // Food position 1
					{0,13,3,2},  // Food position 2
					{0,1,2,3},   // Food position 3
					{0,12,3,0},  // Food position 4
					{0,9,2,1},   // Food position 5
					{0,13,3,4},  // Food position 6
					{1,7,3,3},   // Food position 7
					{1,9,3,2},   // Food position 8
					{1,1,2,3},   // Food position 9
					{0,12,3,0}   // Food position 10
						};

// Custom character pattern for food display
u8 snake_food[] ={0x1B, 0x11, 0x00, 0x00, 0x00, 0x11, 0x1B, 0x00};

// Game state variables
u8 u8_Food_num=1;     // Current food index (0-10)
u8 u8_Direction=1;    // Current movement direction (1=right, 2=down, 3=left, 4=up)
u8 u8_die=0;          // Death flag (0=alive, 1=dead)
s8 x=2;               // Snake head X coordinate (0-79)
s8 y=3;               // Snake head Y coordinate (0-13)

/**
 * Function: vSetFood
 * Description: Places food on the LCD at the current food position and removes previous food
 * Parameters: None
 * Returns: None
 */
void vSetFood(void){
	u8 L_u8Temp=0;  // Temporary variable for bit manipulation
	
	// Set the bit corresponding to the food position within the character
	SET_BIT(L_u8Temp,4-u8_Food[u8_Food_num][3]);
	
	// Update the screen buffer with the food bit
	snake[u8_Food[u8_Food_num][0]][u8_Food[u8_Food_num][1]][u8_Food[u8_Food_num][2]] |= L_u8Temp ;
	
	// Create custom character for the current food position and display it
	HLCD_vWrightCustomChar(3,snake[u8_Food[u8_Food_num][0]][u8_Food[u8_Food_num][1]]);
	HLCD_vSetCursorAtx_y(u8_Food[u8_Food_num][0],u8_Food[u8_Food_num][1]);
	HLCD_vSendChar(3);

	// Clear the previous food position by displaying empty character
	HLCD_vSetCursorAtx_y(u8_Food[u8_Food_num-1][0],u8_Food[u8_Food_num-1][1]);
	HLCD_vSendChar(0);
	
	/* Alternative method to clear previous food (commented out)
	CLR_BIT(L_u8Temp2,4-u8_Food[u8_Food_num-1][3]);
	snake[u8_Food[u8_Food_num-1][0]][u8_Food[u8_Food_num-1][1]][u8_Food[u8_Food_num-1][2]] &= L_u8Temp2 ;
	HLCD_vWrightCustomChar(3,snake[u8_Food[u8_Food_num-1][0]][u8_Food[u8_Food_num-1][1]]);
	HLCD_vSetCursorAtx_y(u8_Food[u8_Food_num-1][0],u8_Food[u8_Food_num-1][1]);
	HLCD_vSendChar(3);*/
}

/**
 * Function: vSet_new_Pos
 * Description: Calculates the new position of the snake head based on current direction
 *              Handles collision detection with food and snake body
 * Parameters: None
 * Returns: None
 */
void vSet_new_Pos(void){
	u8 L_u8Temp=0;        // Temporary variable for bit operations
	u8 L_u8Posx=0;        // LCD column position (0-15)
	u8 L_u8Posy=0;        // LCD row position (0-1)
	u8 L_u8Pos_in_x=0;    // Bit position within character (0-4)
	u8 L_u8Pos_in_y=0;    // Line position within character (0-7)
	
	// Update snake head coordinates based on current direction
	if(u8_Direction==1){          // Moving right
		x=(x+1)%80;               // Wrap around at right edge
	}else if(u8_Direction==2){    // Moving down
		y=(y+1)%14;               // Wrap around at bottom edge
	}else if(u8_Direction==3){    // Moving left
		x--;
		if(x==-1){                // Handle wrap around at left edge
			x=79;
		}
		x=x%80;
	}else if(u8_Direction==4){    // Moving up
		y--;
		if(y == -1){              // Handle wrap around at top edge
			y=13;
		}
		y=y%14;
	}

	// Convert absolute coordinates to LCD position and offset
	L_u8Posx=x/5;          // Each LCD character represents 5 horizontal pixels
	L_u8Posy=y/7;          // Each LCD character represents 7 vertical pixels
	L_u8Pos_in_x=x%5;      // Position within the character horizontally
	L_u8Pos_in_y=y%7;      // Position within the character vertically

	// Update snake head position in the position array
	u8_SnakePos[0][0] = L_u8Posy;     // Row
	u8_SnakePos[0][1] = L_u8Posx;     // Column
	u8_SnakePos[0][2] = L_u8Pos_in_y; // Y offset
	u8_SnakePos[0][3] = L_u8Pos_in_x; // X offset
	u8_SnakePos[0][4] = 1;            // Active flag
	
	// Shift all snake segments one position back (snake movement)
	for(u8 i=5;i>0;i--){
		u8_SnakePos[i][0] = u8_SnakePos[i-1][0];
		u8_SnakePos[i][1] = u8_SnakePos[i-1][1];
		u8_SnakePos[i][2] = u8_SnakePos[i-1][2];
		u8_SnakePos[i][3] = u8_SnakePos[i-1][3];
		u8_SnakePos[i][4] = u8_SnakePos[i-1][4];
	}

	// Deactivate the tail segment
	u8_SnakePos[5][4] = 0;
	
	// Create bit mask for the new head position
	SET_BIT(L_u8Temp,4-L_u8Pos_in_x);
	
	// Check if snake head collides with food
	if(u8_Food[u8_Food_num][0]==L_u8Posy && u8_Food[u8_Food_num][1]==L_u8Posx && 
	   u8_Food[u8_Food_num][2]==L_u8Pos_in_y && u8_Food[u8_Food_num][3]== L_u8Pos_in_x){
		u8_Food_num=(u8_Food_num+1)%10;  // Move to next food position
		return;  // Exit without checking collision (food eaten)
	}

	// Check if snake head collides with its own body
	if((L_u8Temp & snake[L_u8Posy][L_u8Posx][L_u8Pos_in_y]) != 0 ){
		u8_die=1;  // Set death flag
		return;
	}else{
		u8_die=0;  // Continue game
	}
}
/**
 * Function: vDrawSnake
 * Description: Renders the snake on the LCD display by updating custom characters
 *              Handles both active and inactive snake segments
 * Parameters: None
 * Returns: None
 */
void vDrawSnake(void){
	u8 L_u8Temp=0;     // Temporary variable for setting bits
	u8 L_u8Temp2=0x1f; // Temporary variable for clearing bits (0x1f = 00011111)
	
	// Loop through all snake segments
	for(u8 i=0;i<=5;i++){
		L_u8Temp=0;     // Reset temp variables
		L_u8Temp2=0x1f;
		
		// Handle inactive segments (tail that needs to be erased)
		if(u8_SnakePos[i][4] == 0){
			// Clear the bit for this segment position
			CLR_BIT(L_u8Temp2,4-u8_SnakePos[i][3]);
			snake[u8_SnakePos[i][0]][u8_SnakePos[i][1]][u8_SnakePos[i][2]] &= L_u8Temp2;
			
			// Check if this segment shares the same LCD character as the previous segment
			if(u8_SnakePos[i][0]==u8_SnakePos[i-1][0] && u8_SnakePos[i][1]==u8_SnakePos[i-1][1]){
				// Update the custom character and display it
				HLCD_vWrightCustomChar(1,snake[u8_SnakePos[i][0]][u8_SnakePos[i][1]]);
				HLCD_vSetCursorAtx_y(u8_SnakePos[i][0],u8_SnakePos[i][1]);
				HLCD_vSendChar(1);
			}else {
				// Display empty character if no other segments in this position
				HLCD_vSetCursorAtx_y(u8_SnakePos[i][0],u8_SnakePos[i][1]);
				HLCD_vSendChar(0);
			}
			break; // Exit loop after handling inactive segment
		}else{
			// Handle active segments (snake body and head)
			
			// Set the bit for this segment position
			SET_BIT(L_u8Temp,4-u8_SnakePos[i][3]);
			snake[u8_SnakePos[i][0]][u8_SnakePos[i][1]][u8_SnakePos[i][2]] |= L_u8Temp;
			
			// Check if this segment shares the same LCD character as the next segment
			if(u8_SnakePos[i][0]==u8_SnakePos[i+1][0] && u8_SnakePos[i][1]==u8_SnakePos[i+1][1]){
				// Use character type 1 for segments sharing the same LCD position
				HLCD_vWrightCustomChar(1,snake[u8_SnakePos[i][0]][u8_SnakePos[i][1]]);
				HLCD_vSetCursorAtx_y(u8_SnakePos[i][0],u8_SnakePos[i][1]);
				HLCD_vSendChar(1);
			}else {
				// Use character type 2 for isolated segments
				HLCD_vWrightCustomChar(2,snake[u8_SnakePos[i][0]][u8_SnakePos[i][1]]);
				HLCD_vSetCursorAtx_y(u8_SnakePos[i][0],u8_SnakePos[i][1]);
				HLCD_vSendChar(2);
			}
		}
	}

	/* Alternative collision detection method (commented out)
	if((L_u8Temp & snake[L_u8Posy][L_u8Posx][L_u8Pos_in_y]) != 0 ){
		u8_die=1;
		break;
	}else{
		snake[L_u8Posy][L_u8Posx][L_u8Pos_in_y] |= L_u8Temp ;
	}*/
}
/**
 * Function: main
 * Description: Main function - initializes hardware and runs the game loop
 * Parameters: None
 * Returns: int (never returns in embedded systems)
 */
int main(void){

	// Hardware Initialization
	HLCD_vinit();                                    // Initialize LCD display
	DIO_vSetPortDir(DIO_PORTB,0b11110000);          // Set PORTB pins 0-3 as inputs (buttons), 4-7 as outputs
	DIO_vSetPortVal(DIO_PORTB,0b00001111);          // Enable pull-up resistors for input pins

	/* Alternative custom character definitions (commented out)
		HLCD_vWrightCustomChar(0,snake1);
		HLCD_vWrightCustomChar(1,snake2);
		HLCD_vWrightCustomChar(2,snake3);
		HLCD_vWrightCustomChar(3,snake4);
		HLCD_vWrightCustomChar(4,snake26);
		HLCD_vWrightCustomChar(5,snake20);
		HLCD_vWrightCustomChar(6,snake22);
		HLCD_vWrightCustomChar(7,snake25);*/

	// Initialize LCD with border/empty character pattern
	HLCD_vWrightCustomChar(0,snake_use);
	
	// Fill entire LCD screen with empty/border characters
	for(u8 j=0;j<2;j++){                            // Loop through LCD rows (0-1)
		for(u8 i=0;i<=15;i++){                      // Loop through LCD columns (0-15)
			HLCD_vSetCursorAtx_y(j,i);              // Set cursor position
			HLCD_vSendChar(0);                      // Display empty character
		}
	}

	/* Additional LCD setup code (commented out)
		HLCD_vWrightCustomChar(1,snake[1][4]);
		HLCD_vSetCursorAtx_y(0,0);
		HLCD_vSendChar(1);
		
		HLCD_vSetCursorAtx_y(0,15);
		HLCD_vSendChar(5);
		HLCD_vSetCursorAtx_y(1,15);
		HLCD_vSendChar(6);
		HLCD_vSetCursorAtx_y(0,0);
		HLCD_vSendChar(5);
		HLCD_vSetCursorAtx_y(1,0);
		HLCD_vSendChar(6);*/

	// Game loop variables
	u16 counter=0;        // Game timing counter (controls game speed)
	u8 u8_LcdFlag=0;      // Flag to prevent multiple game over screen updates
	
	// Main game loop
	while(1){
		
		// Input handling - Check for button presses with debouncing
		if(DIO_u8GetPinVal(DIO_PORTB,DIO_PIN0)==0){         // Right button pressed
			_delay_ms(10);                                   // Debounce delay
			if(DIO_u8GetPinVal(DIO_PORTB,DIO_PIN0)==0){     // Confirm button still pressed
				 u8_Direction=1;                             // Set direction to right
			}
		}else if(DIO_u8GetPinVal(DIO_PORTB,DIO_PIN1)==0){   // Left button pressed
			_delay_ms(10);                                   // Debounce delay
			if(DIO_u8GetPinVal(DIO_PORTB,DIO_PIN1)==0){     // Confirm button still pressed
				 u8_Direction=3;                             // Set direction to left
			}
		}else if(DIO_u8GetPinVal(DIO_PORTB,DIO_PIN2)==0){   // Down button pressed
			_delay_ms(10);                                   // Debounce delay
			if(DIO_u8GetPinVal(DIO_PORTB,DIO_PIN2)==0){     // Confirm button still pressed
				 u8_Direction=2;                             // Set direction to down
			}
		}else if(DIO_u8GetPinVal(DIO_PORTB,DIO_PIN3)==0){   // Up button pressed
			_delay_ms(10);                                   // Debounce delay
			if(DIO_u8GetPinVal(DIO_PORTB,DIO_PIN3)==0){     // Confirm button still pressed
				 u8_Direction=4;                             // Set direction to up
			}
		}

		// Game logic execution (runs every 200ms)
		if(counter==200){
			if(u8_die == 0){                                 // If snake is alive
				vSet_new_Pos();                              // Calculate new snake position
				vSetFood();                                  // Update food position
				vDrawSnake();                                // Render snake on LCD
			}else{                                           // If snake is dead
				if(u8_LcdFlag==0){                           // Display game over screen only once
					HLCD_vSetCursorAtx_y(0,0);               // Clear first row
					HLCD_vPrintString("                ");
					HLCD_vSetCursorAtx_y(1,0);               // Clear second row
					HLCD_vPrintString("                ");
					HLCD_vSetCursorAtx_y(0,0);               // Display game over message
					HLCD_vPrintString("screenshot:");
					HLCD_vSendChar(1);
					HLCD_vSetCursorAtx_y(1,0);               // Display final score
					HLCD_vPrintString("score:");
					HLCD_vPrintNum(u8_Food_num);
					u8_LcdFlag=1;                            // Set flag to prevent repeated updates
				}
			}
			counter=0;                                       // Reset counter
		}
		counter++;                                           // Increment counter
		_delay_ms(1);                                        // 1ms delay for timing

		/* Alternative snake animation code (commented out)
		 * This section contains code for a different type of snake animation
		 * that moves the snake in a predefined pattern around the screen edges
		 * 
		 * The code implements:
		 * - Moving right across the top row (j<=14 && flag==0)
		 * - Moving left across the bottom row (j2>=1 && flag==1)
		 * - Handling corner transitions with special characters
		 * - Using different custom characters for different parts of the animation
		 */
		/*
		if(j<=14 && flag==0){
			// Move snake to the right across top row
			HLCD_vSetCursorAtx_y(0,j);
			HLCD_vSendChar(0);
			for(u8 i=0;i<=2;i++){
				if(j+1+i<15){
				HLCD_vSetCursorAtx_y(0,j+1+i);
				HLCD_vSendChar(2);
				}else if(j+1+i==15){
					HLCD_vSetCursorAtx_y(0,j+1+i);
				    HLCD_vSendChar(7);
				}else if(j+1+i==16){
					HLCD_vSetCursorAtx_y(1,j+i);
				    HLCD_vSendChar(4);
				}else if(j+1+i==17){
					HLCD_vSetCursorAtx_y(1,j);
				    HLCD_vSendChar(2);
				}
			}
			_delay_ms(200);

			// Handle transition at right edge
			if(j==14){
				flag=1;
				j=0;
				HLCD_vSetCursorAtx_y(0,15);
				HLCD_vSendChar(5);
				_delay_ms(200);
				HLCD_vSetCursorAtx_y(1,15);
				HLCD_vSendChar(6);
				HLCD_vWrightCustomChar(4,snake27);
				HLCD_vWrightCustomChar(7,snake28);
			}
		}

		if(j2>=1 && flag==1){
			// Move snake to the left across bottom row
			HLCD_vSetCursorAtx_y(1,j2);
			HLCD_vSendChar(0);
			for(s8 i=0;i<=2;i++){
				if(j2-1-i>=1){
				HLCD_vSetCursorAtx_y(1,j2-1-i);
				HLCD_vSendChar(2);
				}else if(j2-1-i==0){
					HLCD_vSetCursorAtx_y(1,j2-1-i);
				    HLCD_vSendChar(7);
				}else if(j2-1-i==-1){
					HLCD_vSetCursorAtx_y(0,j2-i);
				    HLCD_vSendChar(4);
				}else if(j2-1-i==-2){
					HLCD_vSetCursorAtx_y(0,j2);
				    HLCD_vSendChar(2);
				}
			}
			_delay_ms(200);

			// Handle transition at left edge
			if(j2==1){
				j2=14;
				flag=0;
				HLCD_vSetCursorAtx_y(1,0);
				HLCD_vSendChar(6);
				_delay_ms(200);
				HLCD_vSetCursorAtx_y(0,0);
				HLCD_vSendChar(5);
				HLCD_vWrightCustomChar(4,snake26);
				HLCD_vWrightCustomChar(7,snake25);
			}
		}

		// Update position counters for animation
		if(flag==0){
			j++;      // Move right
		}
		if(flag==1){
			j2--;     // Move left
		}
		*/

	} // End of main game loop
} // End of main function
