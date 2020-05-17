// PIC16F887 Configuration Bit Settings
// 'C' source line config statements
// CONFIG1
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator: High-speed crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown Out Reset Selection bits (BOR enabled)
#pragma config IESO = ON        // Internal External Switchover bit (Internal/External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP = ON         // Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming enabled)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

#include <xc.h>
//------头文件-------//
#include "LCD.h"
#include "DS1302.h"
#include "KEY.h"
#include "DELAY.h"
#include "LED.h"
#include "INT.h"
#include "UART.h"
#include "SONG.h"
//----------------//
extern bit isMusicMode;
extern unsigned char Mode;
extern unsigned char key2pushflag,key3pushflag;
extern bit clear_flag;
extern signed char cursor_addr;//光标位置;
const long int SongName[4]={"LittleStar","LittleLamb","JingleBell","Sky City"};

void MusicDisplay(void)
{
    LCD_write_char(4,LINE2,SongNum+'1');
    LCD_write_string(6,LINE2,SongName[SongNum]);
        if(isMusicPlaying)
        {
            LCD_write_string(10,LINE1,"PLAY");
        }
        else
        {
            LCD_write_string(10,LINE1,"STOP");
        }
}
void main(void) 
{
    //-------初始化--------//
    Initialize_UART();   
    Initialize_INT();
    //DS1302
    Ds1302_Init();
    Ds1302Init();
    //LCD
    LCD_MPU_init();
    LCD_init();
    InitScrn();
    //LED
    LED_Init();
    //
    MUSIC_init();
    KEY_init();
    PwmInit();
    //------------------------------//
    while(1)
    {
        //按键扫描-----------------------//
        keyscan();
        //----非音乐模式----------------//
        if(!isMusicMode)
        {
            //检测闹钟------------------//
            check_Alarm();
            //实时时钟 时间设置 闹钟设置----//
            ModeSwitch();
        }//-----------------------------------//
        else
        {//------音乐模式-----------------------//
            ClearScreenOnce();
            //----------歌曲信息-----------------//
            MusicDisplay();
            //----------播放/暂停----------------------//
            if(isMusicPlaying)
                MusicPlay();
        }
    }
}

void interrupt ISR()  
{
    if(INTF)
    {
        INTF=0;
        LED1=!LED1;
        isMusicMode=~isMusicMode;
        clear_flag=1;
    }
}
