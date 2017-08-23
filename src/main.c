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
#include "adc.h"

#include "lcd.h"
#include "lcd_utils.h"

#include "flash_program.h"

#include <stdio.h>
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


// ------- Externals del ADC ------------
unsigned short voltage_samples [SIZEOF_VOLTAGE_VECT];
unsigned short current_samples [SIZEOF_CURRENT_VECT];



//--- VARIABLES GLOBALES ---//

// ------- de los timers -------
volatile unsigned short timer_standby;
volatile unsigned short tt_current;


// ------- de los switches -------

#define TIM_BIP_SHORT	300
#define TIM_BIP_LONG	800
#define TT_TO_FREE_ERROR	5000

#define TT_RXCODE			6000

#define SAVE_S1		1
#define SAVE_S2		2


// //Para las mediciones	PLACA A
// #define KV	0.01046
// #define KI	0.00739

//Para las mediciones	PLACA B
#define KV	0.0103
#define KI	0.00739


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
	char str [32];
	unsigned short current = 0;
	unsigned short voltage = 0;
	unsigned short current_zero = 0;
	float fcalc;
	short volt_int, volt_dec;
	unsigned short maxR, minR;




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

	//ADC Configuration.
	AdcConfig();

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
	i = 0;

	while(1)
	{
		switch (main_state)
		{
			case SET_CURRENT_ZERO:
				resp = FuncShowBlink ((const char *) "Wait for setting", (const char *) "Current to zero ", 2, BLINK_DIRECT);

				if (i < 32)
				{
					if (!tt_current)
					{
						current_samples [i] = ReadADC1_SameSampleTime(ADC_CH7);
						tt_current = 8;
						i++;
					}
				}
				else if (i != 0xFF)
				{
					current_zero = MA32 (current_samples);
					i = 0xFF;
				}

				//setear 0 en corriente!!
				if (resp == LCD_RESP_FINISH)
					main_state++;
				break;

			case WAIT_CURRENT_ZERO:
			 	sprintf(str, "%4d            ", current_zero);
				while (FuncShowBlink ((const char *) "Current zero in:", str, 2, BLINK_NO) != LCD_RESP_FINISH);

				while (FuncShowBlink (s_blank_line, s_blank_line, 0, BLINK_NO) != LCD_RESP_FINISH);

				LCD_1ER_RENGLON;
				LCDTransmitStr("V:      V  R:");
				LCD_2DO_RENGLON;
				LCDTransmitStr("I:      A");

				main_state++;
				break;

			case START_MEAS:
				if (i < 32)
				{
					if (!tt_current)
					{
						current_samples [i] = ReadADC1_SameSampleTime(ADC_CH7);
						voltage_samples [i] = ReadADC1_SameSampleTime(ADC_CH6);
						tt_current = 8;
						i++;
					}
				}
				else
				{
					i = 0;
					main_state++;
				}
				break;

			case SHOW_V:
				voltage = MA32 (voltage_samples);
				//LCD_1ER_RENGLON;
				Lcd_SetDDRAM(0x00 + 3);

				fcalc = voltage;
				fcalc = fcalc * KV;
				volt_int = (short) fcalc;
				fcalc = fcalc - volt_int;
				fcalc = fcalc * 100;
				volt_dec = (short) fcalc;

				sprintf(str, "%2d.%02d", volt_int, volt_dec);

				//sprintf(str, "%4d        ", voltage);
				LCDTransmitStr(str);
				main_state++;

				break;

			case SHOW_I:
				current = MA32 (current_samples);
				//LCD_1ER_RENGLON;
				Lcd_SetDDRAM(0x40 + 3);

				// //tira el numero medido directamente y la resta
				// sprintf(str, "%4d %4d", current, current - current_zero);
				// LCDTransmitStr(str);

				if (current < current_zero)
					current = 0;
				else
					current = current - current_zero;

				fcalc = current;
				fcalc = fcalc * KI;
				volt_int = (short) fcalc;
				fcalc = fcalc - volt_int;
				fcalc = fcalc * 100;
				volt_dec = (short) fcalc;

				sprintf(str, "%2d.%02d", volt_int, volt_dec);
				LCDTransmitStr(str);

				main_state++;
				break;

			case SHOW_R:
				maxR = SeekMax32 (voltage_samples);
				minR = SeekMin32 (voltage_samples);

				Lcd_SetDDRAM(0x00 + 13);

				fcalc = maxR - minR;
				fcalc = fcalc * KV;
				volt_int = (short) fcalc;
				fcalc = fcalc - volt_int;
				fcalc = fcalc * 10;
				volt_dec = (short) fcalc;

				if (volt_int > 9)
					volt_int = 9;

				//LCD_1ER_RENGLON;
				sprintf(str, "%1d.%1d", volt_int, volt_dec);
				LCDTransmitStr(str);
				main_state = START_MEAS;
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

	if (tt_current)
		tt_current--;


	//-------- Timers para funciones y sus menues ---------//
	if (function_timer)
		function_timer--;

	if (show_select_timer)
		show_select_timer--;


}
