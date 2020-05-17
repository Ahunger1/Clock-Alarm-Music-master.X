#include "LED.h"

//------LED初始化配置----//

void LED_Init(void) {
    TRISC &= 0b11111100;
    LED1 = 0;
    LED2 = 0;
}