/*
 * hard.h
 *
 *  Created on: 28/11/2013
 *      Author: Mariano
 */

#ifndef HARD_H_
#define HARD_H_

//-------- Board Configuration -----------------------//
#define VER_1_0


//--- Pruebas de hardware, consumo sin int
//#define WITHOUT_INT
#define WITH_INT

//#define WITH_1_TO_10
//#define WITHOUT_POTE

//#ifdef VER_1_0
//#define BOOST_CONVENCIONAL	//placa anterior del tama�o de la F12V5A ultimo prog 13-07-16
//#endif

#ifdef VER_1_0
//GPIOA pin0
//GPIOA pin1
//GPIOA pin2
//GPIOA pin3		interface LCD

//GPIOA pin4
#define LCD_E ((GPIOA->ODR & 0x0010) != 0)
#define LCD_E_ON	GPIOA->BSRR = 0x00000010
#define LCD_E_OFF GPIOA->BSRR = 0x00100000

//GPIOA pin5
#define LCD_RS ((GPIOA->ODR & 0x0020) != 0)
#define LCD_RS_ON	GPIOA->BSRR = 0x00000020
#define LCD_RS_OFF GPIOA->BSRR = 0x00200000





//lo viejo
#define S1 ((GPIOA->IDR & 0x0001) == 0)
#define LED ((GPIOA->ODR & 0x0002) != 0)
#define LED_ON	GPIOA->BSRR = 0x00000002
#define LED_OFF GPIOA->BSRR = 0x00020000

#define TX_CODE ((GPIOA->ODR & 0x0008) != 0)
#define TX_CODE_ON	GPIOA->BSRR = 0x00000008
#define TX_CODE_OFF GPIOA->BSRR = 0x00080000

//GPIOA pin4
#define S2 ((GPIOA->IDR & 0x0010) == 0)

//GPIOA pin5
#define RX_CODE ((GPIOA->IDR & 0x0020) != 0)

//GPIOA pin6
#define RX_EN 		((GPIOA->ODR & 0x0040) != 0)
#define RX_EN_ON	GPIOA->BSRR = 0x00000040
#define RX_EN_OFF 	GPIOA->BSRR = 0x00400000

//GPIOA pin7

//GPIOA pin9
//GPIOA pin10
//GPIOA pin13
//GPIOA pin14

//GPIOB pin1
#endif

//-------- Respuestas de Funciones -------------------------//
enum
{
	RESP_CONTINUE = 0,
	RESP_SELECTED,
	RESP_CHANGE,
	RESP_CHANGE_ALL_UP,
	RESP_WORKING,
	RESP_FINISH,
	RESP_YES,
	RESP_NO,
	RESP_NO_CHANGE,
	RESP_OK,
	RESP_NOK,
	RESP_NO_ANSWER,
	RESP_TIMEOUT,
	RESP_READY
} typedef RspMessages;

//ESTADOS DEL PROGRAMA PRINCIPAL
enum {
	SET_CURRENT_ZERO = 0,
	WAIT_CURRENT_ZERO,
	START_MEAS,
	SHOW_V,
	SHOW_I,
	SHOW_R

} typedef MainStates;

#define REPEAT_CODES	3
#define SWITCHES_TIMER_RELOAD	10

#define SWITCHES_ROOF			400
#define SWITCHES_THRESHOLD_FULL	300		//3 segundos
#define SWITCHES_THRESHOLD_HALF	10		//1 segundo
#define SWITCHES_THRESHOLD_MIN	5		//50 ms

#define TIMER_SLEEP			200		//200ms
#define TIMER_FOR_SAVE		4000	//4 segundos

#define S_FULL		10
#define S_HALF		3
#define S_MIN		1
#define S_NO		0

#endif /* HARD_H_ */
