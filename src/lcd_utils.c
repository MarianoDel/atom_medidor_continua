/*
 * main_menu.c
 *
 *  Created on: 11/02/2016
 *      Author: Mariano
 */

#include "lcd_utils.h"
#include "hard.h"
#include "lcd.h"
#include <stdio.h>
#include <string.h>

//#include "funcs_manual.h"

/* Externals variables ---------------------------------------------------------*/
//extern volatile unsigned short mainmenu_timer;
extern volatile unsigned short show_select_timer;

extern volatile unsigned short scroll1_timer;
extern volatile unsigned short scroll2_timer;

extern const char s_blank_line [];

extern volatile unsigned short lcd_backlight_timer;

/* Global variables ------------------------------------------------------------*/
unsigned char mainmenu_repeat = 0;
unsigned char show_select_state = 0;
unsigned char options_state = 0;
unsigned char options_curr_sel = 0;

unsigned char scroll1_state = 0;
unsigned char scroll1_last_window = 0;
unsigned char scroll1_current_window = 0;

unsigned char scroll2_state = 0;
unsigned char scroll2_last_window = 0;
unsigned char scroll2_current_window = 0;

unsigned char blinking_state = 0;
unsigned char blinking_how_many = 0;

unsigned char change_state = 0;
unsigned short change_current_val = 0;
unsigned char change_last_option = 0;

const unsigned char s_sel_up_down [] = { 0x02, 0x08, 0x0f };
//-------- Functions -------------


//funcion que muestra los string de renglon 1 y renglon 2
//recibe puntero a primer renglon
//puntero a segundo renglon
//how many cantidad de pantallas en blinking
//modo BLINK_DIRECT o BLINK_CROSS
//si modo es BLINK_NO en how_many me pasan los segundos antes de terminar
unsigned char FuncShowBlink (const char * p_text1, const char * p_text2, unsigned char how_many, unsigned char mode)
{
    unsigned char resp = LCD_RESP_CONTINUE;

    switch (blinking_state)
    {
        case BLINKING_INIT:
            if (mode == BLINK_DIRECT)
                blinking_state = BLINKING_MARK_D;
            else if (mode == BLINK_CROSS)
                blinking_state = BLINKING_MARK_C;
            else
                blinking_state = BLINKING_MARK_N;

//            if (!how_many)
//                how_many = 1;

            blinking_how_many = how_many;
            show_select_timer = 0;
            break;

        case BLINKING_MARK_D:
            if (!show_select_timer)
            {
                if (blinking_how_many)
                {
                	blinking_how_many--;
                    LCD_1ER_RENGLON;
                    LCDTransmitStr(p_text1);
                    LCD_2DO_RENGLON;
                    LCDTransmitStr(p_text2);
                    show_select_timer = 1000;
                    blinking_state = BLINKING_SPACE_D;
                }
                else
                {
                	blinking_state = BLINKING_INIT;
                	resp = LCD_RESP_FINISH;
                }
            }
            break;

        case BLINKING_SPACE_D:
            if (!show_select_timer)
            {
                LCD_1ER_RENGLON;
                LCDTransmitStr((const char *)s_blank_line);
                LCD_2DO_RENGLON;
                LCDTransmitStr((const char *)s_blank_line);
                show_select_timer = 600;
                blinking_state = BLINKING_MARK_D;
            }
            break;

        case BLINKING_MARK_C:
            if (!show_select_timer)
            {
                if (blinking_how_many)
                {
                	blinking_how_many--;
                    LCD_1ER_RENGLON;
                    LCDTransmitStr(p_text1);
                    LCD_2DO_RENGLON;
                    LCDTransmitStr((const char *)s_blank_line);
                    show_select_timer = 1000;
                    blinking_state = BLINKING_SPACE_C;
                }
                else
                {
                    LCD_1ER_RENGLON;
                    LCDTransmitStr((const char *) s_blank_line);
                    blinking_state = BLINKING_INIT;
                    resp = LCD_RESP_FINISH;
                }
            }
            break;

        case BLINKING_SPACE_C:
            if (!show_select_timer)
            {
                LCD_1ER_RENGLON;
                LCDTransmitStr((const char *) s_blank_line);
                LCD_2DO_RENGLON;
                LCDTransmitStr(p_text2);
                show_select_timer = 1000;
                blinking_state = BLINKING_MARK_C;
            }
            break;

        case BLINKING_MARK_N:
            LCD_1ER_RENGLON;
            LCDTransmitStr(p_text1);
            LCD_2DO_RENGLON;
            LCDTransmitStr(p_text2);
            show_select_timer = 1000 * how_many;
            blinking_state = BLINKING_SPACE_N;
            break;

        case BLINKING_SPACE_N:
            if (!show_select_timer)
            {
            	blinking_state = BLINKING_INIT;
                resp = LCD_RESP_FINISH;
            }
            break;

        default:
        	blinking_state = BLINKING_INIT;
            resp = LCD_RESP_FINISH;
            break;
    }
    return resp;
}



//hace un scroll en el primer renglon del lcd
//recibe un puntero al string
//devuelve LCD_RESP_CONTINUE o LCD_RESP_FINISH
unsigned char FuncScroll1 (const char * p_text)
{
	unsigned char resp = LCD_RESP_CONTINUE;
	unsigned char last_window;
	unsigned char i;

	switch (scroll1_state)
	{
		case SCROLL_INIT:
			scroll1_last_window = strlen(p_text) + 32;
			scroll1_current_window = 1;
			scroll1_state++;
			break;

		case SCROLL_SENDING:
			if (!scroll1_timer)
			{
				last_window = scroll1_current_window + 16;
				LCD_1ER_RENGLON;

				for (i = scroll1_current_window; i < last_window; i++)
				{
					if (i < 16)
						LCDStartTransmit(' ');
					else if (i < (scroll1_last_window - 16))
						LCDStartTransmit(*(p_text + (i - 16)));
					else if (i < scroll1_last_window)
						LCDStartTransmit(' ');
					else
					{
						//termine de enviar
						i = last_window;
						scroll1_state++;
					}
				}
				scroll1_current_window++;
				scroll1_timer = 550;
			}
			break;

		case SCROLL_FINISH:
			resp = LCD_RESP_FINISH;
			scroll1_state = SCROLL_INIT;
			break;

		default:
			resp = LCD_RESP_FINISH;
			scroll1_state = SCROLL_INIT;
			break;
	}

	return resp;
}

//hace un scroll en el segundo renglon del lcd
//recibe un puntero al string
//devuelve LCD_RESP_CONTINUE o LCD_RESP_FINISH
unsigned char FuncScroll2 (const char * p_text)
{
	unsigned char resp = LCD_RESP_CONTINUE;
	unsigned char last_window;
	unsigned char i;

	switch (scroll2_state)
	{
		case SCROLL_INIT:
			scroll2_last_window = strlen(p_text) + 32;
			scroll2_current_window = 1;
			scroll2_state++;
			break;

		case SCROLL_SENDING:
			if (!scroll2_timer)
			{
				last_window = scroll2_current_window + 16;
				LCD_2DO_RENGLON;

				for (i = scroll2_current_window; i < last_window; i++)
				{
					if (i < 16)
						LCDStartTransmit(' ');
					else if (i < (scroll2_last_window - 16))
						LCDStartTransmit(*(p_text + (i - 16)));
					else if (i < scroll2_last_window)
						LCDStartTransmit(' ');
					else
					{
						//termine de enviar
						i = last_window;
						scroll2_state++;
					}
				}
				scroll2_current_window++;
				scroll2_timer = 550;
			}
			break;

		case SCROLL_FINISH:
			resp = LCD_RESP_FINISH;
			scroll2_state = SCROLL_INIT;
			break;

		default:
			resp = LCD_RESP_FINISH;
			scroll2_state = SCROLL_INIT;
			break;
	}

	return resp;
}
