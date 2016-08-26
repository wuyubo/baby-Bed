#ifndef _MAIN_H_
#define _MAIN_H_
#define DEBUG
#define DATANOCHANGE 2
/********** normal headers *************/
#include "ht32.h"
#include "ht32_board.h"
#include "ucos_ii.h"
#include "app_cfg.h"
#include "tasks.h"
#include "led.h"
#include "uart1.h"
#include "sem.h"
#include "delay.h"
#include "milk.h"
#include "bed_types.h"
#include "system.h"
/********normal**********/

/********** moudules headers *************/
#include "gy-mcu90615.h"
#include "risym-mp3.h"
#include "cry-voice.h"
#include "fan.h"
#include "piss.h"
/*wifi module*/
#include "spi-wifi.h"
/****/
//#include "ov7670.h"
#include "rock.h"
#include "quilt.h"
#include "onbed.h"
/********modules**********/

/********GPIO CONFIG**********/
#define GPIO_PORTA  GPIO_PA
#define GPIO_PORTB  GPIO_PB
#define GPIO_PORTC  GPIO_PC
#define GPIO_PORTD  GPIO_PD
#define GPIO_PORTE  GPIO_PE

#define HT_GPIO_PORTA  HT_GPIOA
#define HT_GPIO_PORTB  HT_GPIOB
#define HT_GPIO_PORTC  HT_GPIOC
#define HT_GPIO_PORTD  HT_GPIOD
#define HT_GPIO_PORTE  HT_GPIOE

#define GPIO_PIN0	 GPIO_PIN_0
#define GPIO_PIN1  GPIO_PIN_1
#define GPIO_PIN2  GPIO_PIN_2
#define GPIO_PIN3  GPIO_PIN_3
#define GPIO_PIN4  GPIO_PIN_4
#define GPIO_PIN5  GPIO_PIN_5
#define GPIO_PIN6  GPIO_PIN_6
#define GPIO_PIN7  GPIO_PIN_7
#define GPIO_PIN8  GPIO_PIN_8
#define GPIO_PIN9  GPIO_PIN_9
#define GPIO_PIN10 GPIO_PIN_10
#define GPIO_PIN11 GPIO_PIN_11
#define GPIO_PIN12 GPIO_PIN_12
#define GPIO_PIN13 GPIO_PIN_13
#define GPIO_PIN14 GPIO_PIN_14
#define GPIO_PIN15 GPIO_PIN_15


#define GPIO_OUT_CONFIG(gpio_port_out, ht_gpio_port_out, gpio_pin_out) 	AFIO_GPxConfig(gpio_port_out,           gpio_pin_out, AFIO_MODE_DEFAULT);\
																		GPIO_DirectionConfig(ht_gpio_port_out,  gpio_pin_out, GPIO_DIR_OUT)
															
#define GPIO_IN_CONFIG(gpio_port_in, ht_gpio_port_in, gpio_pin_in) 		AFIO_GPxConfig(gpio_port_in,           gpio_pin_in, AFIO_MODE_DEFAULT);\
																		GPIO_DirectionConfig(ht_gpio_port_in,  gpio_pin_in, GPIO_DIR_IN);\
																		GPIO_InputConfig(ht_gpio_port_in,      gpio_pin_in, ENABLE)
																		
#define CPIO_OUT_BITS(ht_gpio_port_out, gpio_pin_out, value)   	 GPIO_WriteOutBits(ht_gpio_port_out, gpio_pin_out, value)
#define CPIO_IN_BITS(ht_gpio_port_in, gpio_pin_in)   					   GPIO_ReadInBit(ht_gpio_port_in, gpio_pin_in)
/********GPIO CONFIG**********/
/********MODULES GPIO CONFIG**********/
/********tempalte CONFIG**********/
#define _TEM_PORT 			GPIO_PORTA
#define _TEM_HT_PORT 	HT_GPIO_PORTA
#define _TEM_TRIG			GPIO_PIN6
#define _TEM_ECHO			GPIO_PIN7
/********tempalte CONFIG**********/
/********SWING CONFIG**********/
#define	_ROCK_GPIO_PX	 GPIO_PORTA
#define	_ROCK_HT_GPIOX HT_GPIO_PORTA
#define _ROCK_PIN_SWI  GPIO_PIN8			/*开关IO*/
#define _ROCK_PIN_L0 	 GPIO_PIN9      /*位置0IO*/
#define _ROCK_PIN_L1 	 GPIO_PIN10     /*位置1IO*/
/********SWING CONFIG**********/
/********ONBED CONFIG**********/
#define	_ONBED_GPIO_PX	 GPIO_PORTA
#define	_ONBED_HT_GPIOX  HT_GPIO_PORTA

#define _ONBED_PIN 	     GPIO_PIN11

/*************************************************/
/********WIFI CONFIG**********/
#define _WIFI_CPIO_PORT1    GPIO_PORTB
#define _WIFI_CPIO_PORT2    GPIO_PORTB
#define _WIFI_HT_CPIO_PORT1  HT_GPIO_PORTB
#define _WIFI_HT_CPIO_PORT2  HT_GPIO_PORTB
#define _RESET_PIN			    GPIO_PIN0
#define _SCK_PIN 			      GPIO_PIN1
#define _CS_PIN			        GPIO_PIN2
#define _MOSI_PIN 			    GPIO_PIN3
#define _MISO_PIN			      GPIO_PIN4
//#define _INT_PIN			      GPIO_PIN4

/********WIFI CONFIG**********/

/********MUSIC CONFIG**********/
#define	_MUSIC_GPIO_PX	  GPIO_PORTB
#define	_MUSIC_HT_GPX     HT_GPIO_PORTB
#define _MUSIC_NEXT_PIN 	GPIO_PIN6
#define _MUSIC_PREV_PIN 	GPIO_PIN5
#define _MUSIC_OPEN_PIN 	GPIO_PIN8
#define _MUSIC_PAUSE_PIN  GPIO_PIN9
/********MUSIC CONFIG**********/
/********VOICE CONFIG**********/
#define	_VOICE_GPIO_PX	  GPIO_PORTB
#define	_VOICE_HT_GPIOX   HT_GPIO_PORTB
#define _CRY_VOICE_PIN 	  GPIO_PIN10
/********VOICE CONFIG**********/
/********FAN CONFIG**********/
#define	_FAN_GPIO_PX		GPIO_PORTB
#define _FAN_HT_GPIOX   HT_GPIO_PORTB
#define _FAN_PIN 	      GPIO_PIN11
/********FAN CONFIG**********/
/********PISS CONFIG**********/
#define	_PISS_GPIO_PX	 GPIO_PORTB
#define	_PISS_HT_GPIOX HT_GPIO_PORTB
#define _PISS_PIN 	   GPIO_PIN12
/********PISS CONFIG**********/
/********KICK CONFIG**********/
#define	_QUILT_GPIO_PX	 GPIO_PORTB
#define	_QUILT_HT_GPIOX  HT_GPIO_PORTB
#define _QUILT_PIN 	     GPIO_PIN13
/********KICK CONFIG**********/
/********HEAT CONFIG**********/
#define	_MILK_GPIO_PX		 GPIO_PORTB
#define	_MILK_HT_GPIOX   HT_GPIO_PORTB
#define _MILK_PIN 			 GPIO_PIN15
/********HEAT CONFIG**********/
/********MODULES GPIO CONFIG**********/

void system_Reset(void);
#endif






