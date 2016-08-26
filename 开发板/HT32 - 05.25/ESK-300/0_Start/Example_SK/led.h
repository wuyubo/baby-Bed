#ifndef _LED_H_
#define _LED_H_
#include <stdint.h>
#define LED_0 0
#define LED_1 1
#define LED_2 2
#define LED_3 3
//typedef unsigned long uint32_t;

void led_init(void); 
void led_on(uint32_t n);
void led_off(uint32_t n);
void led_all_off(void);

#endif
