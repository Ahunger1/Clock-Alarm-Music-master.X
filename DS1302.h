#ifndef __DS1302_H_
#define	__DS1302_H_
#include <xc.h> // include processor files - each processor file is guarded.  
//---定义ds1302使用的IO口---//
#define RST PORTCbits.RC3
#define SCLK PORTCbits.RC4
#define DSIO PORTCbits.RC5
//---------时钟显示位置--------//
#define YEAR_THOUSAND 1
#define YEAR_TEN 3// 年 LINE1 3 4位
#define YEAR_UNIT 4 
#define MONTH_TEN 6//月 LINE1 7 2位
#define MONTH_UNIT 7
#define DAY_TEN 9//日 LINE1 10 2位
#define DAY_UNIT 10
#define WEEK_TEN 13
#define WEEK_UNIT 14//周 LINE1 13 1位
#define HOUR_TEN 6//时 LINE2 6 2位
#define HOUR_UNIT 7
#define MINUTE_TEN 9//分 LINE2 9 2位
#define MINUTE_UNIT 10
#define SECOND_TEN 12
#define SECOND_UNIT 13//秒 LINE2 12 2位
#define BEEP 22 //闹钟PR值

extern signed char YEAR_ten, YEAR_unit, MONTH_ten, MONTH_unit, DAY_ten, DAY_unit, WEEK_unit, HOUR_ten, HOUR_unit, MINUTE_ten, MINUTE_unit, SECOND_ten, SECOND_unit;
extern signed char aHOUR_ten, aHOUR_unit, aMINUTE_ten, aMINUTE_unit, aSECOND_ten, aSECOND_unit;
//------------闹钟初始时间----------//
signed char Alarm_TIME[6] = {0, 1, 0, 0, 0, 0};
//                  秒   分   时
void Ds1302Write(unsigned char addr, unsigned char dat);
unsigned char Ds1302Read(unsigned char addr);
void Ds1302Init(void);
void Ds1302WriteTime(unsigned char n, unsigned char dat);
void Ds1302ReadTimeBCD();
void Ds1302_Init(void);
void Trans_Time_bit(void);
void TimeDataDisplay(void);
void FrameworkDisplay(void);
//Alarm
void Trans_Alarm_bit(void);
void AlarmDataDisplay(void);
void check_Alarm(void);
void aFrameworkDisplay(void);
void beep_ms(unsigned int n);

#endif	

