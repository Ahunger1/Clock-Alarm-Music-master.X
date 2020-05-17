#ifndef __UART_H_
#define __UART_H_
#include <xc.h>
#define UART_REC UART_send_string("Received ");
//串口初始化
void Initialize_UART(void);
//发送一字节
void UART_send_char(unsigned char data);
//发送字符串
void UART_send_string(unsigned char* Str);

#endif