#include "INT.h"
//------中断初始---------//
void Initialize_INT(void)
{
    INTCONbits.GIE = 1;   //全局中断开
    INTCONbits.PEIE = 1;  //外部中断开
    INTCONbits.INTE=1;
    OPTION_REGbits.INTEDG=0;//下降沿触发
}

