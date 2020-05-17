#include "SONG.h"
#include "LCD.h"
#include "UART.h"
#include "LED.h"
#include <string.h>
#include "DELAY.h"


//低音
#define Cl 238
#define Dl 212
#define El 188
#define Fl 178
#define Flh 167//hash 井号键 升高半音
#define Gl 158
#define Glh 149
#define Al 141
#define Bl 125
//中音
#define Cm 118
#define Dm 105
#define Em 94
#define Fm 88
#define Gm 79
#define Am 70
#define Bm 62
//高音
#define Ch 59
#define Dh 52
#define Eh 46
#define Fh 44
#define Gh 39
#define Ah 34
#define Bh 31

#define Mu 0//Mute 休止
#define END 255


//某些歌放不下ROM
const unsigned char LittleStar[] = {Mu, 2,
    Cm, 2, Cm, 2, Gm, 2, Gm, 2, Am, 2, Am, 2, Gm, 4, Fm, 2, Fm, 2, Em, 2, Em, 2, Dm, 2, Dm, 2, Cm, 4,
    END}; 
//Mary had a little lamb
const unsigned char LittleLamb [] = {Mu, 2,
    Em, 3, Dm, 1, Cm, 2, Dm, 2, Em, 2, Em, 2, Em, 2, Em, 2, Dm, 2, Dm, 2, Dm, 2, Dm, 2, Em, 2, Gm, 2, Gm, 2, Gm, 2,
    Em, 3, Dm, 1, Cm, 2, Dm, 2, Em, 2, Em, 2, Em, 2, Cm, 2, Dm, 2, Dm, 2, Em, 2, Dm, 2, Cm, 2, Cm, 2, Cm, 2, Cm, 2,
    Bm, 3, Am, 1, Gm, 2, Am, 2, Bm, 2, Bm, 2, Bm, 2, Bm, 2, Gm, 2, Gm, 2, Gm, 2, Gm, 2, Bm, 2, Dh, 2, Bm, 2, Bm, 2,
    Bm, 3, Am, 1, Gm, 2, Am, 2, Bm, 2, Bm, 2, Bm, 2, Gm, 2, Am, 2, Am, 2, Bm, 2, Am, 2, Gm, 2, Gm, 2, Gm, 2, Gm, 2,
    END};
const unsigned char JingleBell [] = {Mu, 2,
    Gl, 2, Em, 2, Dm, 2, Cm, 2, Gl, 4, Mu, 2, Gl, 1, Gl, 1, Gl, 2, Em, 2, Dm, 2, Cm, 2, Al, 4, Mu, 4,
    Al, 2, Fm, 2, Em, 2, Dm, 2, Bl, 4, Mu, 4, Gm, 2, Gm, 2, Fm, 2, Dm, 2, Em, 4, Cm, 2, Mu, 2,
    Gl, 2, Em, 2, Dm, 2, Cm, 2, Gl, 4, Mu, 4, Gl, 2, Em, 2, Dm, 2, Cm, 2, Gl, 4, Mu, 2, Gl, 2,
    Al, 2, Fm, 2, Em, 2, Dm, 2, Gm, 2, Gm, 2, Gm, 2, Gm, 2, Am, 2, Gm, 2, Fm, 2, Dm, 2, Cm, 6, Mu, 2,
    Em, 2, Em, 2, Em, 4, Em, 2, Em, 2, Em, 4, Em, 2, Gm, 2, Cm, 3, Dm, 1, Em, 6, Mu, 2,
    Fm, 2, Fm, 2, Fm, 3, Fm, 1, Fm, 2, Em, 2, Em, 2, Em, 1, Em, 1, Em, 2, Dm, 2, Dm, 2, Cm, 2, Dm, 4, Gm, 4,
    Em, 2, Em, 2, Em, 4, Em, 2, Em, 2, Em, 4, Em, 2, Gm, 2, Cm, 3, Dm, 1, Em, 6, Mu, 2,
    Fm, 2, Fm, 2, Fm, 3, Fm, 1, Fm, 2, Em, 2, Em, 2, Em, 1, Em, 1, Gm, 2, Gm, 2, Fm, 2, Dm, 2, Cm, 6, Mu, 2,
    END};
const unsigned char SkyCity[] = {Mu, 2,
    Al, 2, Bl, 2,
    Cm, 6, Bl, 2, Cm, 4, Em, 4, Bl, 4, Bl, 4, Bl, 4, El, 4, Al, 6, Gl, 2, Al, 4, Cm, 4, Gl, 4, Gl, 4, Gl, 4, El, 4, Fl, 6, El, 2, Fl, 2, Cm, 6,
    El, 4, El, 4, El, 4, Cm, 4, Bl, 6, Flh, 2, Flh, 4, Bl, 4, Bl, 4, Bl, 4, Mu, 4, Al, 2, Bl, 2, Cm, 6, Bl, 2, Cm, 4, Em, 4, Bl, 4, Bl, 4, Bl, 4, El, 2, El, 2,
    Al, 6, Gl, 2, Al, 4, Cm, 4, Gl, 4, Gl, 4, Gl, 4, El, 4, Fl, 4, Cm, 2, Bl, 2, Bl, 4, Cm, 4, Dm, 4, Em, 2, Cm, 4, Cm, 4, Mu, 2, Cm, 2, Bl, 2, Al, 4, Bl, 4, Glh, 4,
    Al, 4, Al, 4, Mu, 4, Cm, 2, Dm, 2, Em, 5, Dm, 3, Em, 3, Fm, 3, Gm, 2, Dm, 4, Dm, 4, Dm, 4, Gl, 4, Dm, 1, Cm, 1, Bl, 1, Cm, 1, Cm, 4, Cm, 2, Dm, 4, Em, 2,
    Em, 4, Em, 4, Em, 4, Em, 4, Al, 2, Bl, 2, Cm, 4, Bl, 2, Cm, 2, Dm, 4, Cm, 6, Gl, 2, Gl, 4, Gl, 4, Fm, 4, Em, 4, Dm, 4, Cm, 4, Em, 4, Em, 4, Em, 4, Al, 2, Bl, 2, Cm, 6, Bl, 2, Cm, 4, Em, 4,
    Bl, 4, Bl, 4, Bl, 4, El, 4, Al, 6, Gl, 2, Al, 4, Cm, 4, Gl, 4, Gl, 4, Gl, 4, El, 4, Fl, 4, Cm, 2, Bl, 2, Bl, 4, Cm, 4, Dm, 4, Em, 2, Cm, 2, Cm, 4, Cm, 4,
    Cm, 2, Bl, 2, Al, 4, Bl, 4, Glh, 4,
    Al, 4, Al, 4, Al, 4, Al, 4,
    END};



//-----------------歌曲名---------------------------//
unsigned int MusicPosition;
//extern unsigned char Str[10];
signed char SongNum; //第几首歌
bit isMusicPlaying; //播放还是暂停



//-----初始化配置------//

void MUSIC_init(void) {

    isMusicPlaying = 0; //默认暂停
    SongNum = 0; //默认第一首开始
}

void PwmInit(void) {
    //unsigned int X;
    //分频
    T2CON = 0b00000111; //TMR2ON 1：16
    // Fosc=4MHz  Tosc=0.25us
    //输出1kHz 占空比50%
    //T=1ms=1000us 
    //CCP1    
    TRISCbits.TRISC2 = 0; //BUZZER
}

void PwmTone(unsigned char n) {
    signed int X;
    //PR2
    PR2 = n; //周期为1ms
    //X=(1+PR2)*4/D=(1+PR2)*2  根据占空比
    X = (1 + PR2) << 1;
    //CCPR1L
    CCPR1L = X >> 2;
    //CCP1CON<5:4>
    CCP1CONbits.CCP1X = X >> 1;
    CCP1CONbits.CCP1Y = X;
    //PWM模式
    CCP1CONbits.CCP1M3 = 1;
    CCP1CONbits.CCP1M2 = 1;
}

void MusicPlay(void) {
    if (isMusicPlaying == 0)
        return;
    if (SongNum == 0) //改成switch
    {
        TempTone = LittleStar[MusicPosition];
        TempMetre = LittleStar[++MusicPosition];
    } else if (SongNum == 1) {
        TempTone = LittleLamb[MusicPosition];
        TempMetre = LittleLamb[++MusicPosition];
    } else if (SongNum == 2) {
        TempTone = JingleBell[MusicPosition];
        TempMetre = JingleBell[++MusicPosition];
    } else if (SongNum == 3) {
        TempTone = SkyCity[MusicPosition];
        TempMetre = SkyCity[++MusicPosition];
    }
    if (TempTone == Mu)//休止符
    {
        TRISCbits.TRISC2 = 1;
        SixteenNoteDelay(TempMetre);
        TRISCbits.TRISC2 = 0;
        MusicPosition++;
        return;
    }
    if (TempTone == END) {
        SongNum++;
        if (SongNum > 3) {
            SongNum = 0;
        }
        MusicPosition = 0;
        return;
    }
    //播放
    TRISCbits.TRISC2 = 0; //中间7ms
    PwmTone(TempTone);
    SixteenNoteDelay(TempMetre); //一拍250ms
    MusicPosition++;
    //在音符之间创建简短的静音,利用循环的时间
    TRISCbits.TRISC2 = 1;
}

