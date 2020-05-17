#ifndef __LED_H_
#define	__LED_H_
#include <xc.h> 

#define LED1 PORTCbits.RC0
#define LED2 PORTCbits.RC1

void LED_Init(void);

#endif

