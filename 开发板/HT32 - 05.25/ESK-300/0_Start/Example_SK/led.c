#include "ht32.h"
#include "ht32_board.h"
#include "led.h"

void led_init(void)
{
	HT32F_DVB_LEDInit(HT_LED1);
  HT32F_DVB_LEDInit(HT_LED2);
}

void led_on(uint32_t n)
{
	switch(n)
	{
		case LED_1:
			HT32F_DVB_LEDOn(HT_LED1);
		  break;
		case LED_2:
			HT32F_DVB_LEDOn(HT_LED2);
		default:
			break;
	}

}

void led_off(uint32_t n)
{
	switch(n)
	{
		case LED_1:
			HT32F_DVB_LEDOff(HT_LED1);
		  break;
		case LED_2:
			HT32F_DVB_LEDOff(HT_LED2);
		default:
			break;
	}
		
}
void led_all_off(void)
{
	led_off(LED_1);
	led_off(LED_2);
}





