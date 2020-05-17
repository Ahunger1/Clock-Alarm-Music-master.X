#include"DELAY.h"
//--------计数延时--------//

void delay_ms(unsigned int n)// n ms    
{
    unsigned int j;
    char k;
    for (j = 0; j < n; j++)
        for (k = 108; k > 0; k--)
            NOP();
}

void delay_us(unsigned char n)//n*10us +21us
{
    NOP();
    while (n > 0) {
        n--;
    }
}

void SixteenNoteDelay(unsigned char n) {
    unsigned char x;
    for (x = n; x > 0; x--)
        delay_ms(170);
}