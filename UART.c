#include "UART.h"
#include "LED.h"
#include <string.h>
//波特率计算
#define _XTAL_FREQ 4000000//4MHz
#define Baud_rate 9600
#define Baud_val (((_XTAL_FREQ/16)/Baud_rate) - 1)

void Initialize_UART(void) {
    //设置端口方向
    TRISCbits.TRISC6 = 0; // TX输出
    TRISCbits.TRISC7 = 1; // RX输入
    //设置波特率位数
    BAUDCTLbits.BRG16 = 0; //波特率位数8
    //设置发送位数
    TXSTAbits.TX9 = 0; //八位发送
    TXSTAbits.TXEN = 1; // 发送使能
    TXSTAbits.SYNC = 0; // 异步
    TXSTAbits.BRGH = 1; // 高速模式
    //设置波特率因子
    SPBRG = Baud_val;
    //设置使能串口
    RCSTAbits.SPEN = 1; // 串口使能
    RCSTAbits.RX9 = 0; //八位接收
    RCSTAbits.CREN = 1; // 连续接收
    RCSTAbits.ADDEN = 0; //禁止地址检测
    UART_send_string("UART Module Initialized and active \r");
}
//-----------发送一字节-------------//

void UART_send_char(unsigned char data) {
    TXREG = data;
    while (!TXIF);
    while (TXSTAbits.TRMT == 0); //TRMT==0时，TSR正在发送
}
//--------发送字符串-------------//

void UART_send_string(unsigned char *Str) {
    while (*Str != '\0')
        UART_send_char(*Str++);
}