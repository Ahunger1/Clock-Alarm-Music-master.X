#include"DS1302.h"
#include"LCD.h"
#include "LED.h"
#include "DELAY.h"
#include "SONG.h"

//----------------------显示星期------------------------//
const int *week[7] = {"Mon.", "Tue.", "Wed.", "Thu.", "Fri.", "Sat.", "Sun."}; //int 四字节
//-----------------------DS1302读写位------------------------//
const unsigned char ReadTimeAddr[7] = {0x81, 0x83, 0x85, 0x87, 0x89, 0x8b, 0x8d};
const unsigned char WriteTimeAddr[7] = {0x80, 0x82, 0x84, 0x86, 0x88, 0x8a, 0x8c};
//                          秒    分    时    日    月    周   年
//-------------------------时钟初始时间-----------------------------------------//
unsigned char SET_TIME[7] = {0x55, 0x59, 0x23, 0x15, 0x03, 0x07, 0x20}; //默认2020年 3.15 7
//-------------------------------闹钟变量----------------------------------//
signed char aHOUR_ten, aHOUR_unit, aMINUTE_ten, aMINUTE_unit, aSECOND_ten, aSECOND_unit;
//----------中间变量-------//
unsigned char tempTime_Ds1302[7];
//------------------------自定义从大到小的LCD显示地址数组------------------------------//
unsigned char LcdTimeAddr[13] = {YEAR_TEN, YEAR_UNIT, MONTH_TEN, MONTH_UNIT, DAY_TEN, DAY_UNIT, WEEK_UNIT, HOUR_TEN, HOUR_UNIT, MINUTE_TEN, MINUTE_UNIT, SECOND_TEN, SECOND_UNIT};
//---------------------------------------时钟变量-------------------------------------------------------------------------------------------------//
signed char YEAR_ten, YEAR_unit, MONTH_ten, MONTH_unit, DAY_ten, DAY_unit, WEEK_unit, HOUR_ten, HOUR_unit, MINUTE_ten, MINUTE_unit, SECOND_ten, SECOND_unit;


//---------------写数据--------------//

void Ds1302Write(unsigned char addr, unsigned char dat) {
    unsigned char n;
    RST = 0;
    NOP();

    SCLK = 0; //先将SCLK置低电平。
    NOP();
    RST = 1; //然后将RST(CE)置高电平。
    NOP();

    for (n = 0; n < 8; n++)//开始传送八位地址命令
    {
        DSIO = addr & 0x01; //数据从低位开始传送
        addr >>= 1;
        SCLK = 1; //数据在上升沿时，DS1302读取数据
        NOP();
        SCLK = 0;
        NOP();
    }
    for (n = 0; n < 8; n++)//写入8位数据
    {
        DSIO = dat & 0x01;
        dat >>= 1;
        SCLK = 1; //数据在上升沿时，DS1302读取数据
        NOP();
        SCLK = 0;
        NOP();
    }

    RST = 0; //传送数据结束
    NOP();
}

//------------读数据------------//

unsigned char Ds1302Read(unsigned char addr) {
    unsigned char n, dat, dat1;
    RST = 0;
    NOP();

    SCLK = 0; //先将SCLK置低电平。
    NOP();
    RST = 1; //然后将RST(CE)置高电平。
    NOP();

    for (n = 0; n < 8; n++)//开始传送八位地址命令
    {
        DSIO = addr & 0x01; //数据从低位开始传送
        addr >>= 1;
        SCLK = 1; //数据在上升沿时，DS1302读取数据
        NOP();
        SCLK = 0; //DS1302下降沿时，放置数据
        NOP();
    }
    NOP();
    TRISCbits.TRISC5 = 1;
    for (n = 0; n < 8; n++)//读取8位数据
    {
        dat1 = DSIO; //从最低位开始接收
        dat = (dat >> 1) | (dat1 << 7);
        SCLK = 1;
        NOP();
        SCLK = 0; //DS1302下降沿时，放置数据
        NOP();
    }

    RST = 0;
    /*NOP();	//以下为DS1302复位的稳定时间,必须的。吗？？？？
    SCLK = 1;
    NOP();
    DSIO = 0;
    NOP();
    DSIO = 1;*/
    NOP();
    TRISCbits.TRISC5 = 0;
    return dat;
}

//-----------写初始化时间-----------//

void Ds1302Init(void) {
    unsigned char n, dat;
    RST = 0;
    SCLK = 0;
    //dat=Ds1302Read(0x81);//仿真时总是会掉电，所以注释掉
    //if((dat&0x80)!=0)//reg1 bit7 为0时正常供电=为1时数据丢失，需要重新设置默认时间
    //{
    Ds1302Write(0x8E, 0); //禁止写保护，就是关闭写保护功能
    for (n = 0; n < 7; n++)//写入7个字节的时钟信号：分秒时日月周年
    {
        Ds1302Write(WriteTimeAddr[n], SET_TIME[n]);
    }
    Ds1302Write(0x8E, 0x80); //打开写保护功能
    Trans_Alarm_bit();
}

//--------------写入时间--------------------//

void Ds1302WriteTime(unsigned char n, unsigned char dat) {
    RST = 0;
    SCLK = 0;
    Ds1302Write(0x8E, 0);
    Ds1302Write(WriteTimeAddr[n], dat);
    Ds1302Write(0x8E, 0x80);
}

//--------------初始化配置--------------------//

void Ds1302_Init(void) {
    TRISC &= 0b11000111; //RC3~RC5输出
}


//--------------读取时间-----------------//

void Ds1302ReadTimeBCD(void) {
    unsigned char n;
    for (n = 0; n < 7; n++)//读取7个字节的时钟信号：分秒时日月周年
    {
        tempTime_Ds1302[n] = Ds1302Read(ReadTimeAddr[n]);
    }
}

//------------把读取的时间写入变量--------//

void Trans_Time_bit(void) {
    YEAR_ten = tempTime_Ds1302[6] >> 4;
    YEAR_unit = tempTime_Ds1302[6]&0x0f;
    WEEK_unit = tempTime_Ds1302[5]&0x0f;
    //WEEK_ten=Real_Time_BCD[3]>>4;
    MONTH_ten = tempTime_Ds1302[4] >> 4;
    MONTH_unit = tempTime_Ds1302[4]&0x0f;
    DAY_ten = tempTime_Ds1302[3] >> 4;
    DAY_unit = tempTime_Ds1302[3]&0x0f;
    HOUR_ten = tempTime_Ds1302[2] >> 4;
    HOUR_unit = tempTime_Ds1302[2]&0x0f;
    MINUTE_ten = tempTime_Ds1302[1] >> 4;
    MINUTE_unit = tempTime_Ds1302[1]&0x0f;
    SECOND_ten = tempTime_Ds1302[0] >> 4;
    SECOND_unit = tempTime_Ds1302[0]&0x0f;
}

//-------------将时钟变量写入LCD----------------//

void TimeDataDisplay(void) {
    //---------------LINE1   年 月 日 周
    LCD_write_string(YEAR_THOUSAND, LINE1, "20");
    LCD_write_char(YEAR_TEN, LINE1, YEAR_ten + '0');
    LCD_write_char(YEAR_UNIT, LINE1, YEAR_unit + '0');
    LCD_write_char(MONTH_TEN, LINE1, MONTH_ten + '0');
    LCD_write_char(MONTH_UNIT, LINE1, MONTH_unit + '0');
    LCD_write_char(DAY_TEN, LINE1, DAY_ten + '0');
    LCD_write_char(DAY_UNIT, LINE1, DAY_unit + '0');
    LCD_write_string(WEEK_TEN - 1, LINE1, week[WEEK_unit - 1]);
    //LCD_write_char(WEEK_TEN,LINE1,'W');
    //LCD_write_char(WEEK_UNIT,LINE1,WEEK_unit+'0');
    //------------LINE2---------时  分 秒
    LCD_write_char(HOUR_TEN, LINE2, HOUR_ten + '0');
    LCD_write_char(HOUR_UNIT, LINE2, HOUR_unit + '0');
    LCD_write_char(MINUTE_TEN, LINE2, MINUTE_ten + '0');
    LCD_write_char(MINUTE_UNIT, LINE2, MINUTE_unit + '0');
    LCD_write_char(SECOND_TEN, LINE2, SECOND_ten + '0');
    LCD_write_char(SECOND_UNIT, LINE2, SECOND_unit + '0');
}

//--------------LCD时钟的框架------------//

void FrameworkDisplay(void) {
    LCD_write_char(YEAR_UNIT + 1, LINE1, '-');
    LCD_write_char(MONTH_UNIT + 1, LINE1, '-');
    LCD_write_char(HOUR_UNIT + 1, LINE2, ':');
    LCD_write_char(MINUTE_UNIT + 1, LINE2, ':');
}

//--------------LCD闹钟的框架-------------//

void aFrameworkDisplay(void) {
    LCD_write_char(HOUR_UNIT + 1, LINE2, ':');
    LCD_write_char(MINUTE_UNIT + 1, LINE2, ':');
}

//-------将读取的闹钟时间写入变量-------------//

void Trans_Alarm_bit(void) {
    aHOUR_ten = Alarm_TIME[5];
    aHOUR_unit = Alarm_TIME[4];
    aMINUTE_ten = Alarm_TIME[3];
    aMINUTE_unit = Alarm_TIME[2];
    aSECOND_ten = Alarm_TIME[1];
    aSECOND_unit = Alarm_TIME[0];
}

//---------------将闹钟变量写入LCD--------------//

void AlarmDataDisplay(void) {
    LCD_write_char(HOUR_TEN, LINE2, aHOUR_ten + '0');
    LCD_write_char(HOUR_UNIT, LINE2, aHOUR_unit + '0');
    LCD_write_char(MINUTE_TEN, LINE2, aMINUTE_ten + '0');
    LCD_write_char(MINUTE_UNIT, LINE2, aMINUTE_unit + '0');
    LCD_write_char(SECOND_TEN, LINE2, aSECOND_ten + '0');
    LCD_write_char(SECOND_UNIT, LINE2, aSECOND_unit + '0');
}

void beep_ms(unsigned int n) {
    TRISCbits.TRISC2 = 0;
    LED2 = 1;
    PwmTone(BEEP);
    delay_ms(n);
    LED2 = 0;
    TRISCbits.TRISC2 = 1;

}
//--------------闹钟是否到点------------------//不要用嵌套if正逻辑，要花1ms来判断

void check_Alarm(void) {
    if (SECOND_unit != aSECOND_unit) {
        return;
    } else {
        if (SECOND_ten != aSECOND_ten) {
            return;
        } else {
            if (MINUTE_unit != aMINUTE_unit) {
                return;
            } else {
                if (MINUTE_ten != aMINUTE_ten) {
                    return;
                } else {
                    if (HOUR_unit != aHOUR_unit) {
                        return;
                    } else {
                        if (HOUR_ten != aHOUR_ten) {
                            return;
                        } else {
                            beep_ms(50);
                            //
                        }
                    }
                }
            }
        }
    }
}

