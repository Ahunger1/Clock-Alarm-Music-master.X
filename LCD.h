#ifndef __LCD_H_
#define	__LCD_H_
#include <xc.h>

//驱动LCD的相关定义
#define LINE1 1
#define LINE2 2
#define LINE1_HEAD 0x80 //设置数据存储器地址0b1xxxxxxx  0x80=0b10000000
#define LINE2_HEAD 0xC0
//-----指令----//
#define DATA_MODE 0x38//0b00111000 功能设置命令： 设置8位数据传输模式 双行设置 5*10点阵
#define OPEN_SCREEN 0x0C//0b00001100 显示开，光标关，光标闪烁关
#define DISPLAY_ADDRESS 0x80
#define CLEARSCREEN LCD_en_command(0x01)//0b00000001清屏指令
//----------IO口定义------------------------//
#define LCDIO PORTD  //数据总线为D端口
#define LCD1602_RS PORTEbits.RE0//数据、指令选择线
#define LCD1602_EN PORTEbits.RE1//使能信号线
#define LCD1602_RW PORTEbits.RE2//读、写选择线

void LCD_en_command(unsigned char command);
void LCD_init(void);
void LCD_en_dat(unsigned char dat);
void LCD_set_xy(unsigned char x, unsigned char y);
void LCD_write_char(signed char x, signed char y, signed char dat);
void LCD_write_string(unsigned char X, unsigned char Y, unsigned char *s);
void LCD_MPU_init(void);
void InitScrn(void);
char LCD_Read_dat(void);
void LCD_Busy(void);
unsigned char Y_position(signed char addr);

#endif	/* XC_HEADER_TEMPLATE_H */

