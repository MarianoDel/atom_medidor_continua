/**
  ******************************************************************************
  * @file    Template_2/main.c
  * @author  Nahuel
  * @version V1.0
  * @date    22-August-2014
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * Use this template for new projects with stm32f0xx family.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "hard.h"
#include "core_cm0.h"
#include "gpio.h"
#include "tim.h"
#include "stm32f0xx_it.h"
#include "dsp.h"

#include "lcd.h"
#include "lcd_utils.h"

#include "flash_program.h"

//#include <stdio.h>
//#include <string.h>


/* Externals variables ---------------------------------------------------------*/
parameters_typedef param_struct;


// ------- Externals de los timers -------
volatile unsigned char timer_1seg = 0;
volatile unsigned short wait_ms_var = 0;
volatile unsigned short timer_tick = 0;

//----- para menues (main_menu.c) ----------
volatile unsigned short function_timer;
volatile unsigned short show_select_timer = 0;
volatile unsigned short scroll1_timer;
volatile unsigned short scroll2_timer;
volatile unsigned short lcd_backlight_timer;

// ------- del display LCD -------
const char s_blank_line [] = {"                "};






//--- VARIABLES GLOBALES ---//

// ------- de los timers -------
volatile unsigned short timer_standby;

// ------- de los switches -------

#define TIM_BIP_SHORT	300
#define TIM_BIP_LONG	800
#define TT_TO_FREE_ERROR	5000

#define TT_RXCODE			6000

#define SAVE_S1		1
#define SAVE_S2		2






//--- FUNCIONES DEL MODULO ---//
void TimingDelay_Decrement(void);



//-------------------------------------------//
// @brief  Main program.
// @param  None
// @retval None
//------------------------------------------//
int main(void)
{
	unsigned char i;
	RspMessages resp = RESP_CONTINUE;
	MainStates main_state = SET_CURRENT_ZERO;



	//!< At this stage the microcontroller clock setting is already configured,
    //   this is done through SystemInit() function which is called from startup
    //   file (startup_stm32f0xx.s) before to branch to application main.
    //   To reconfigure the default setting of SystemInit() function, refer to
    //   system_stm32f0xx.c file

	//GPIO Configuration.
	GPIO_Config();
	EXTIOff();

	//ACTIVAR SYSTICK TIMER
	if (SysTick_Config(48000))		//del core_cm0.h
	//if (SysTick_Config(8000))		//del core_cm0.h
	{
		while (1)	/* Capture error */
		{
			if (LED)
				LED_OFF;
			else
				LED_ON;

			for (i = 0; i < 255; i++)
			{
				asm (	"nop \n\t"
						"nop \n\t"
						"nop \n\t" );
			}
		}
	}


	//--- PRUEBA DISPLAY LCD ---
	EXTIOff ();
	LCDInit();
	LED_ON;

	//--- Welcome code ---//
	Lcd_Command(CLEAR);
	Wait_ms(100);
	Lcd_Command(CURSOR_OFF);
	Wait_ms(100);
	Lcd_Command(BLINK_OFF);
	Wait_ms(100);

	// LCDTransmitStr("KIRNO");
	// while (1);

	while (FuncShowBlink ((const char *) "Kirno Technology", (const char *) "   -DC Meas-   ", 2, BLINK_NO) != LCD_RESP_FINISH);


	//TIM Configuration.
//	TIM_16_Init();

	//--- COMIENZO PROGRAMA DE PRODUCCION

	//--- Main loop ---//
	while(1)
	{
		switch (main_state)
		{
			case SET_CURRENT_ZERO:
				resp = FuncShowBlink ((const char *) "Wait for setting", (const char *) "Current to zero ", 5, BLINK_DIRECT);

				//setear 0 en corriente!!
				if (resp == LCD_RESP_FINISH)
					main_state++;
				break;

			case WAIT_CURRENT_ZERO:
				break;


			default:
				main_state = SET_CURRENT_ZERO;
				break;
		}

	//Funciones que no tienen que ver con el main loop


	}	//termina while(1)

	return 0;
}


//--- End of Main ---//



void TimingDelay_Decrement(void)
{
	if (wait_ms_var)
		wait_ms_var--;

	if (timer_standby)
		timer_standby--;


	//-------- Timers para funciones y sus menues ---------//
	if (function_timer)
		function_timer--;

	if (show_select_timer)
		show_select_timer--;


}
