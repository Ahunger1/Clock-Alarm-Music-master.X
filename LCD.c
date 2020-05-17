#include"LCD.h"
#include"DELAY.h"


//--------------LCD写指令函数--------------------//
void LCD_en_command(unsigned char command)//参数为 数据总线的地址
{
    LCD1602_EN=0;
    LCD1602_RS=0;//RS,RW 同时为低电平，可以写入指令
    LCD1602_RW=0;
    LCDIO=command;
    delay100us;
    delay100us;
    delay100us;
    delay100us;
    delay100us;
    delay100us;
    LCD1602_EN=1;//E置低电平
    delay_ms(1);//等待指令写完 40e-6s
    LCD1602_EN=0;//写指令，RS=0，R/w=0，E下降沿
}

//------------------LCD初始化----------------------//
void LCD_init(void)
{
    CLEARSCREEN;//清屏 0x01
    LCD_en_command(DATA_MODE);//设置8位数据传输模式
    LCD_en_command(OPEN_SCREEN);//显示开启，不显示光标
    LCD_en_command(DISPLAY_ADDRESS);//设置LCD初次显示地址
    CLEARSCREEN;
}

//------------------LCD写数据函数-------------------//
void LCD_en_dat(char dat)
{
    LCD1602_EN=0;//清零
    LCD1602_RS=1;//RS为高电平，RW为低电平，可以写入数据
    LCD1602_RW=0;

    LCDIO=dat;//写入

    LCD1602_EN=1;
    delay100us;//保持时间
    LCD1602_EN=0;
}

//-------LCD读数据/标志------//
char LCD_Read_dat(void)
{
    char dat;
    LCDIO|=0xff;//D口输入
    LCD1602_RS=0;
    LCD1602_RW=1;NOP();
    LCD1602_EN=1;
    dat=LCDIO;//读
    
    LCD1602_EN=0;NOP();//结束
    LCD1602_RW=0;
    LCDIO=0;//D口输出
    return dat;

}

//-------设置地址----------------//
void LCD_set_xy(unsigned char x, unsigned char y)
{
    unsigned char address;
    if(y==LINE1)//LINE1=1 LINE2=2
    {address = (unsigned char)LINE1_HEAD+x;}//LINE1_HEAD=0x80=0b10000000 DDRAM地址设置 显示数据RAM
    //第一行地址为00H到27H，0b00100111=32+7=39，第二行为40H到67H
    else
    {address = LINE2_HEAD+x;}//0xc0=0x80+0x40
    LCD_en_command(DISPLAY_ADDRESS|address);//DISPLAY_ADDRESS=0x80
}

//-----------------写字符-------------------//
void LCD_write_char(signed char x, signed char y, signed char dat)
{
    LCD_set_xy(x,y);
    LCD_en_dat(dat);
}

//-----------------写字符串-------------------//
void LCD_write_string(unsigned char X, unsigned char Y, unsigned char *s)
{
    LCD_Busy();//查忙
    LCD_set_xy(X,Y);
    while(*s)
    {
        LCDIO=*s;
        LCD_en_dat(*s);
        s++;
    }
}

//--------分行显示-------年月日上行，时分秒下行//
unsigned char Y_position(signed char addr) 
{
    //cursor_addr 0~6 为LINE1  7~12 LINE2
    if((addr >=0)&&(addr<=6))
    {
        return LINE1;
    }
    else if((addr>=7)&&(addr<=12))
    {
        return LINE2;
    }
}

//-------LCD初始化配置-------//
void LCD_MPU_init(void)
{
    TRISE&=0b11111000;
    ANSEL=0;
    ANSELH=0;//B口要关模拟
    TRISD=0;//D口输出

}

//-----孔雀开屏------//不记得自己写了这个
void InitScrn(void)
{
    CLEARSCREEN;
    delay_ms(10);
    
}

//------查忙---------//
void LCD_Busy(void)
{
    char busyFlag;
    while(1)
    {
        busyFlag=LCD_Read_dat();
        if((busyFlag&0x80)==0)
        {
            break;
        }
    }
}

