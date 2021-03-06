/*
 * main_menu.h
 *
 *  Created on: 11/02/2016
 *      Author: Mariano
 */

#ifndef LCD_UTILS_H_
#define LCD_UTILS_H_


//respuestas de las funciones
#define LCD_RESP_CONTINUE		0
#define LCD_RESP_SELECTED		1
#define LCD_RESP_CHANGE_DWN		2
#define LCD_RESP_CHANGE_UP		3
#define LCD_RESP_CHANGE_ALL_UP	4
#define LCD_RESP_WORKING		5

#define LCD_RESP_FINISH			10
#define LCD_RESP_YES			11
#define LCD_RESP_NO				12
#define LCD_RESP_NO_CHANGE		13
#define LCD_RESP_UPDATED		14

//estados de la funcion BLINKING
#define BLINKING_INIT       0
#define BLINKING_MARK_D     1
#define BLINKING_SPACE_D    2
#define BLINKING_MARK_C     3
#define BLINKING_SPACE_C    4
#define BLINKING_MARK_N     5
#define BLINKING_SPACE_N    6

//modos de blinking
#define BLINK_DIRECT    0
#define BLINK_CROSS     1
#define BLINK_NO        2


//estados de la funcion SHOW SELECT
#define SHOW_SELECT_INIT				0
#define SHOW_SELECT_1					1
#define SHOW_SELECT_2					2
#define SHOW_SELECT_3					3
#define SHOW_SELECT_SELECTED			4
#define SHOW_SELECT_SELECTED_1			5
#define SHOW_SELECT_CHANGE_DWN			6
#define SHOW_SELECT_CHANGE_DWN_1		7
#define SHOW_SELECT_CHANGE_UP			8
#define SHOW_SELECT_CHANGE_UP_1			9

//estados de la funcion OPTIONS
#define OPTIONS_INIT					0
#define OPTIONS_WAIT_SELECT			1
#define OPTIONS_WAIT_SELECT_1			2
#define OPTIONS_WAIT_SELECT_2			3
#define OPTIONS_WAIT_SELECT_3			4

#define OPTIONS_CHANGE_SELECT_UP	10
#define OPTIONS_CHANGE_SELECT_DWN	11

//estados de la funcion SCROLL
#define SCROLL_INIT				0
#define SCROLL_SENDING			1
#define SCROLL_FINISH			2

//estados de la funcion CHANGE
#define CHANGE_INIT					0
#define CHANGE_WAIT_SELECT			1


//modos de change
#define CHANGE_PERCENT    0
#define CHANGE_SECS		  1
#define CHANGE_CHANNELS	  2
#define CHANGE_ADDRESS	  3
#define CHANGE_COLORS	  4
#define CHANGE_RESET	  0x80

//wrapers de la funcion FuncChange
#define FuncChangePercent(X)		FuncChange(X, CHANGE_PERCENT, 0, 100)
#define FuncChangeSecs(X)			FuncChange(X, CHANGE_SECS, 0, 10)
#define FuncChangeChannels(X)		FuncChangeThreeButtonsCHAR(X, CHANGE_CHANNELS, 1, 2)
#define FuncChangeColors(X)			FuncChangeThreeButtonsCHAR(X, CHANGE_COLORS, 0, 2)
#define FuncChangeAddress(X)		FuncChangeThreeButtons(X, CHANGE_ADDRESS, 1, 512)
#define FuncChangePercentReset()	FuncChangeReset()
#define FuncChangeSecsReset()		FuncChangeReset()
#define FuncChangeChannelsReset()	FuncChangeReset()
#define FuncChangeAddressReset()	FuncChangeReset()
#define FuncChangeColorsReset()		FuncChangeReset()

//-------- Functions -------------
unsigned char FuncMainMenu (void);
unsigned char FuncShowSelect (const char *);
unsigned char FuncShowSelectv2 (const char *);
unsigned char FuncShowSelectv3 (const char *);
unsigned char FuncOptions (const char *, const char *, unsigned char *, unsigned char, unsigned char);
unsigned char FuncScroll1 (const char *);
unsigned char FuncScroll2 (const char *);
unsigned char FuncShowBlink (const char * , const char * , unsigned char, unsigned char);
//unsigned char FuncChange (unsigned char *);
unsigned char FuncChange (unsigned short *, unsigned char , unsigned short , unsigned short );
unsigned char FuncChangeThreeButtons (unsigned short * , unsigned char , unsigned short , unsigned short );
unsigned char FuncChangeThreeButtonsCHAR (unsigned char * , unsigned char , unsigned char , unsigned char );

void FuncOptionsReset (void);
void FuncShowSelectv2Reset (void);
void FuncChangeReset (void);

#endif /* MAIN_MENU_H_ */
