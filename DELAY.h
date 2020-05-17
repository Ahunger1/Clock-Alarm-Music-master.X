#ifndef __DELAY_H_
#define	__DELAY_H_
#include <xc.h>
#define delay100us delay_us(8)
#define delay20us delay_us(0)
void delay_ms(unsigned int n);
void delay_us(unsigned char n); //n*10us +20us
void SixteenNoteDelay(unsigned char n);
#endif	

