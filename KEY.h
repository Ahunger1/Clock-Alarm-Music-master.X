#ifndef __KEY_H_
#define	__KEY_H_
#include <xc.h> // include processor files - each processor file is guarded.  

//------------按钮--------//
#define key1 PORTBbits.RB2
#define key2 PORTBbits.RB3
#define key3 PORTBbits.RB4
#define key4 PORTBbits.RB5
#define key5 PORTBbits.RB6
#define key6 PORTBbits.RB7

void ClearScreenOnce(void);
void KEY_init(void);
void keyscan(void);
void ModeSwitch(void);
char check_K1(void);
char check_K2(void);
char check_K3(void);
char check_K4(void);
char check_K5(void);
char check_K6(void);

#endif	/* XC_HEADER_TEMPLATE_H */

