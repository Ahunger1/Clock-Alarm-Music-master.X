#include "KEY.h"
#include "DELAY.h"
#include "LCD.h"
#include "DS1302.h"
#include "LED.h"
#include "SONG.h"

//0，1，2分别为时钟模式，时间调整模式，闹钟模式
unsigned char Mode;
extern unsigned char LcdTimeAddr[13];
extern unsigned int MusicPosition;
bit clear_flag;
signed char cursor_addr; //光标位置
bit isMusicMode; //进入还是退出
//--------------------按键标志-------------------------------------------------------//
unsigned char key2pushflag, key3pushflag;

//---按键初始化---------//

void KEY_init(void) {
    ANSEL = 0;
    ANSELH = 0; //B口要关模拟
    OPTION_REGbits.nRBPU = 0; //B口上拉使能
    TRISB |= 0b11111101; //
    Mode = 0; //先显示时钟模式
    cursor_addr = 0; //光标位置零
    clear_flag = 1; //最开始运行时进入时钟模块，需要加载框架
    isMusicMode = 0;
}
//进入各个Mode是清屏

void ClearScreenOnce(void) {
    if (clear_flag) {
        CLEARSCREEN;
        cursor_addr = 0; //光标位置清零
        if (isMusicMode) {
            //----------显示框架-------------//
            LCD_write_string(3, LINE1, "MUSIC");
            LCD_write_string(0, LINE2, "NUM:");
            MusicPosition = 0; //切歌清位置
        } else {
            if (Mode == 0 || Mode == 1) {
                FrameworkDisplay(); //时钟模块框架显示
            } else {
                aFrameworkDisplay(); //闹钟模块框架显示
                LCD_write_string(1, LINE1, "SETTING--ALARM");
            }
        }
        clear_flag = 0;
    }
}

char check_K1(void) {
    if (!key1) {
        delay_ms(10); //防抖
        if (!key1) {
            while (!key1); //等待按键抬起
            clear_flag = 1; //countkey改变，模式改变，每次模式改变要清屏。
            Mode++; //次数加
            if (Mode > 2)//测试可用
                Mode = 0; //0为走时模式，1为设置时钟模式，2为设置闹钟模式
            return 1;
        }
    }
    return 0;
}

char check_K2(void) {
    if (!key2) {
        delay_ms(10); //防抖
        if (!key2) {
            while (!key2); //等待按键抬起
            key2pushflag = 1;
            return 1;
        }
    }
    return 0;
}

char check_K3(void) {
    if (!key3) {
        delay_ms(10); //防抖
        if (!key3) {
            while (!key3); //等待按键抬起
            key3pushflag = 1;
            return 1;
        }
    }
    return 0;
}

char check_K4(void) {
    if (!key4) {
        delay_ms(10); //防抖
        if (!key4) {
            while (!key4); //等待按键抬起
            if (isMusicMode) {
                SongNum--;
                if (SongNum < 0) {
                    SongNum = 3;
                }
                clear_flag = 1; //每次切歌要清屏。
            } else {
                cursor_addr--;
                if (Mode == 1) {
                    if (cursor_addr < 0)
                        cursor_addr = 12;
                } else if (Mode == 2) {
                    if (cursor_addr < 0)
                        cursor_addr = 5;
                }
            }
            return 1;
        }
    }
    return 0;
}

char check_K5(void) {
    if (!key5) {
        delay_ms(10); //防抖
        if (!key5) {
            while (!key5); //等待按键抬起
            if (isMusicMode) {
                SongNum++;
                if (SongNum > 3) {
                    SongNum = 0;
                }
                clear_flag = 1; //每次切歌要清屏。
            } else {
                cursor_addr++;
                if (Mode == 1) {
                    if (cursor_addr > 12)
                        cursor_addr = 0;
                } else if (Mode == 2) {
                    if (cursor_addr > 5)
                        cursor_addr = 0;
                }
            }
            return 1;
        }
    }
    return 0;
}

char check_K6(void) {
    if (!key6) {
        delay_ms(10); //防抖
        if (!key6) {
            while (!key6); //等待按键抬起
            isMusicPlaying = ~isMusicPlaying;
            return 1;
        }
    }
    return 0;
}


//------按下哪个键-------//  

void keyscan(void) {
    check_K1();
    check_K2();
    check_K3();
    check_K4();
    check_K5();
    check_K6();
}

//---------处理三个模式--------//

void ModeSwitch(void) {
    switch (Mode)//功能键按键次数
    {
            //-------------------------时钟模块----------------//
        case 0:
        {
            //-------进入时钟模块清屏--------//
            ClearScreenOnce();
            //从DS1302中读取每一位数据，写入LCD1602中//
            Ds1302ReadTimeBCD();
            Trans_Time_bit();
            TimeDataDisplay();
            break;
        }
            //-------------------调整时间模块------------------//
        case 1:
        {
            ClearScreenOnce();
            delay_ms(73);
            //-----------------指针跟随-------------------------------//
            LCD_write_string(LcdTimeAddr[cursor_addr], Y_position(cursor_addr), "_"); //空格时间3.7ms   数字时间 26.8ms
            delay_ms(96);
            switch (cursor_addr) {
                case 0://YEAR_ten
                {
                    if (key2pushflag == 1) {
                        YEAR_ten++; //
                        if (YEAR_ten > 9) {
                            YEAR_ten = 0;
                        }
                        key2pushflag = 0; //防止按1次该键，数值一直增加
                    } else if (key3pushflag == 1) {
                        YEAR_ten--;
                        if (YEAR_ten < 0) {
                            YEAR_ten = 9;
                        }
                        key3pushflag = 0;
                    }
                    Ds1302WriteTime(6, YEAR_ten << 4 | YEAR_unit); //不能用加法
                    break;
                }
                case 1://YEAR_unit
                {
                    if (key2pushflag == 1) {
                        YEAR_unit++; //
                        if (YEAR_unit > 9) {
                            YEAR_unit = 0;
                        }
                        key2pushflag = 0;
                    } else if (key3pushflag == 1) {
                        YEAR_unit--;
                        if (YEAR_unit < 0) {
                            YEAR_unit = 9;
                        }
                        key3pushflag = 0;
                    }
                    Ds1302WriteTime(6, YEAR_ten << 4 | YEAR_unit);
                    break;
                }
                case 2://MONTH_ten
                {
                    if (key2pushflag == 1) {
                        MONTH_ten++; //
                        if (MONTH_unit > 0) {
                            if (MONTH_ten > 1) {
                                MONTH_ten = 0;
                            }
                        } else {
                            if (MONTH_ten > 2) {
                                MONTH_ten = 0;
                            }
                        }
                        key2pushflag = 0;
                    } else if (key3pushflag == 1) {
                        MONTH_ten--;
                        if (MONTH_unit > 0) {
                            if (MONTH_ten < 0) {
                                MONTH_ten = 1;
                            }
                        } else {
                            if (MONTH_ten < 0) {
                                MONTH_ten = 2;
                            }
                        }
                        key3pushflag = 0;
                    }
                    Ds1302WriteTime(4, MONTH_ten << 4 | MONTH_unit);
                    break;
                }
                case 3://MONTH_unit
                {
                    if (key2pushflag == 1) {
                        MONTH_unit++; //
                        if (MONTH_ten == 1) {
                            if (MONTH_unit > 2) {
                                MONTH_unit = 0;
                            }
                        } else {
                            if (MONTH_unit > 9) {
                                MONTH_unit = 0;
                            }
                        }
                        key2pushflag = 0;
                    } else if (key3pushflag == 1) {
                        MONTH_unit--;
                        if (MONTH_ten == 1) {
                            if (MONTH_unit < 0) {
                                MONTH_unit = 2;
                            }
                        } else {
                            if (MONTH_unit < 0) {
                                MONTH_unit = 9;
                            }
                        }
                        key3pushflag = 0;
                    }
                    Ds1302WriteTime(4, MONTH_ten << 4 | MONTH_unit);
                    break;
                }
                case 4://DAY_ten
                {
                    if (key2pushflag == 1) {
                        DAY_ten++; //
                        if (DAY_unit > 0) {
                            if (DAY_ten > 2) {
                                DAY_ten = 0;
                            }
                        } else {
                            if (DAY_ten > 3) {
                                DAY_ten = 0;
                            }
                        }
                        key2pushflag = 0;
                    } else if (key3pushflag == 1) {
                        DAY_ten--;
                        if (DAY_unit > 0) {
                            if (DAY_ten < 0) {
                                DAY_ten = 2;
                            }
                        } else {
                            if (DAY_ten < 0) {
                                DAY_ten = 3;
                            }
                        }
                        key3pushflag = 0;
                    }
                    Ds1302WriteTime(3, DAY_ten << 4 | DAY_unit);
                    break;
                }
                case 5://DAY_unit
                {
                    if (key2pushflag == 1) {
                        DAY_unit++; //
                        if (DAY_ten == 3) {
                            if (DAY_unit > 2) {
                                DAY_unit = 0;
                            }
                        } else {
                            if (DAY_unit > 9) {
                                DAY_unit = 0;
                            }
                        }
                        key2pushflag = 0;
                    } else if (key3pushflag == 1) {
                        DAY_unit--;
                        if (DAY_ten == 3) {
                            if (DAY_unit < 0) {
                                DAY_unit = 1;
                            }
                        } else {
                            if (DAY_unit < 0) {
                                DAY_unit = 9;
                            }
                        }
                        key3pushflag = 0;
                    }
                    Ds1302WriteTime(3, DAY_ten << 4 | DAY_unit);
                    break;
                }
                case 6://WEEK_unit
                {
                    LCD_write_string(WEEK_TEN - 1, LINE1, "___");
                    if (key2pushflag == 1) {
                        WEEK_unit++; //WEEK_unit 1~7
                        if (WEEK_unit > 7) {
                            WEEK_unit = 1;
                        }
                        key2pushflag = 0;
                    } else if (key3pushflag == 1) {
                        WEEK_unit--;
                        if (WEEK_unit < 0) {
                            WEEK_unit = 7;
                        }
                        key3pushflag = 0;
                    }
                    Ds1302WriteTime(5, WEEK_unit);
                    break;
                }
                    //    LCD_write_string(WEEK_TEN-1,LINE1,week[WEEK_unit-1]);
                    //int week[7]={"Mon.","Tue.","Wed.","Thu.","Fri.","Sat.","Sun."};//int 四字节
                case 7://HOUR_ten
                {
                    if (key2pushflag == 1) {
                        HOUR_ten++; //
                        if (HOUR_unit > 4) {
                            if (HOUR_ten > 1) {
                                HOUR_ten = 0;
                            }
                        } else {
                            if (HOUR_ten > 2) {
                                HOUR_ten = 0;
                            }
                        }
                        key2pushflag = 0;
                    } else if (key3pushflag == 1) {
                        HOUR_ten--;
                        if (HOUR_unit > 4) {
                            if (HOUR_ten < 0) {
                                HOUR_ten = 1;
                            }
                        } else {
                            if (HOUR_ten < 0) {
                                HOUR_ten = 2;
                            }
                        }

                        key3pushflag = 0;
                    }
                    Ds1302WriteTime(3, HOUR_ten << 4 | HOUR_unit);
                    break;
                }
                case 8://HOUR_unit
                {
                    if (key2pushflag == 1) {
                        HOUR_unit++; //
                        if (HOUR_ten == 2) {
                            if (HOUR_unit > 4) {
                                HOUR_unit = 0;
                            }
                        } else {
                            if (HOUR_unit > 9) {
                                HOUR_unit = 0;
                            }
                        }
                        key2pushflag = 0;
                    } else if (key3pushflag == 1) {
                        HOUR_unit--;
                        if (HOUR_ten == 2) {
                            if (HOUR_unit < 0) {
                                HOUR_unit = 4;
                            }
                        } else {
                            if (HOUR_unit < 0) {
                                HOUR_unit = 9;
                            }
                        }
                        key3pushflag = 0;
                    }
                    Ds1302WriteTime(2, HOUR_ten << 4 | HOUR_unit);
                    break;
                }
                case 9://MINUTE_ten
                {
                    if (key2pushflag == 1) {
                        MINUTE_ten++; //
                        if (MINUTE_unit > 0) {
                            if (MINUTE_ten > 5) {
                                MINUTE_ten = 0;
                            }
                        } else {
                            if (MINUTE_ten > 6) {
                                MINUTE_ten = 0;
                            }
                        }
                        key2pushflag = 0;
                    } else if (key3pushflag == 1) {
                        MINUTE_ten--;
                        if (MINUTE_unit > 0) {
                            if (MINUTE_ten < 0) {
                                MINUTE_ten = 5;
                            }
                        } else {
                            if (MINUTE_ten < 0) {
                                MINUTE_ten = 6;
                            }
                        }
                        key3pushflag = 0;
                    }
                    Ds1302WriteTime(1, MINUTE_ten << 4 | MINUTE_unit);
                    break;
                }
                case 10://MINUTE_unit
                {
                    if (key2pushflag == 1) {
                        MINUTE_unit++; //
                        if (MINUTE_ten == 6) {
                            MINUTE_unit = 0;
                        } else {
                            if (MINUTE_unit > 9) {
                                MINUTE_unit = 0;
                            }
                        }
                        key2pushflag = 0;
                    } else if (key3pushflag == 1) {
                        MINUTE_unit--;
                        if (MINUTE_ten == 6) {
                            MINUTE_unit = 0;
                        } else {
                            if (MINUTE_unit < 0) {
                                MINUTE_unit = 9;
                            }
                        }

                        key3pushflag = 0;
                    }
                    Ds1302WriteTime(1, MINUTE_ten << 4 | MINUTE_unit);
                    break;
                }
                case 11://SECOND_ten
                {
                    if (key2pushflag == 1) {
                        SECOND_ten++; //
                        if (SECOND_unit > 0) {
                            if (SECOND_ten > 5) {
                                SECOND_ten = 0;
                            }
                        } else {
                            if (SECOND_ten > 6) {
                                SECOND_ten = 0;
                            }
                        }
                        key2pushflag = 0;
                    } else if (key3pushflag == 1) {
                        SECOND_ten--;
                        if (SECOND_unit > 0) {
                            if (SECOND_ten < 0) {
                                SECOND_ten = 5;
                            }
                        } else {
                            if (SECOND_ten < 0) {
                                SECOND_ten = 6;
                            }
                        }
                        key3pushflag = 0;
                    }
                    Ds1302WriteTime(0, SECOND_ten << 4 | SECOND_unit);
                    break;
                }
                case 12://SECOND_unit
                {
                    if (key2pushflag == 1) {
                        SECOND_unit++; //
                        if (SECOND_ten == 6) {
                            SECOND_unit = 0;
                        } else {
                            if (SECOND_unit > 9) {
                                SECOND_unit = 0;
                            }
                        }
                        key2pushflag = 0;
                    } else if (key3pushflag == 1) {
                        SECOND_unit--;
                        if (SECOND_ten == 6) {
                            SECOND_unit = 0;
                        } else {
                            if (SECOND_unit < 0) {
                                SECOND_unit = 9;
                            }
                        }
                        key3pushflag = 0;
                    }
                    Ds1302WriteTime(0, SECOND_ten << 4 | SECOND_unit);
                    break;
                }
            }
            TimeDataDisplay();
            break;
        }
            //----------------------闹钟模块-------------------------------//
        case 2:
        {
            ClearScreenOnce();
            Trans_Alarm_bit(); //-------将读取的闹钟时间写入变量-------------//
            AlarmDataDisplay(); //---------------将闹钟变量写入LCD--------------//
            delay_ms(78);
            LCD_write_string(LcdTimeAddr[cursor_addr + 7], LINE2, "_"); //空 2ms  数字22ms
            delay_ms(98);
            switch (cursor_addr + 7) {
                case 7://  aHOUR_ten
                    if (key2pushflag == 1) {
                        aHOUR_ten++; //
                        if (aHOUR_unit > 4) {
                            if (aHOUR_ten > 1) {
                                aHOUR_ten = 0;
                            }
                        } else {
                            if (aHOUR_ten > 2) {
                                aHOUR_ten = 0;
                            }
                        }
                        key2pushflag = 0;
                    } else if (key3pushflag == 1) {
                        aHOUR_ten--;
                        if (aHOUR_unit > 4) {
                            if (aHOUR_ten < 0) {
                                aHOUR_ten = 1;
                            }
                        } else {
                            if (aHOUR_ten < 0) {
                                aHOUR_ten = 2;
                            }
                        }

                        key3pushflag = 0;
                    }
                    Alarm_TIME[5] = aHOUR_ten;
                    break;
                case 8:// aHOUR_unit
                    if (key2pushflag == 1) {
                        aHOUR_unit++; //
                        if (aHOUR_ten == 2) {
                            if (aHOUR_unit > 4) {
                                aHOUR_unit = 0;
                            }
                        } else {
                            if (aHOUR_unit > 9) {
                                aHOUR_unit = 0;
                            }
                        }
                        key2pushflag = 0;
                    } else if (key3pushflag == 1) {
                        aHOUR_unit--;
                        if (aHOUR_ten == 2) {
                            if (aHOUR_unit < 0) {
                                aHOUR_unit = 4;
                            }
                        } else {
                            if (aHOUR_unit < 0) {
                                aHOUR_unit = 9;
                            }
                        }
                        key3pushflag = 0;
                    }
                    Alarm_TIME[4] = aHOUR_unit;
                    break;
                case 9:// aMINUTE_ten
                    if (key2pushflag == 1) {
                        aMINUTE_ten++; //
                        if (aMINUTE_unit > 0) {
                            if (aMINUTE_ten > 5) {
                                aMINUTE_ten = 0;
                            }
                        } else {
                            if (aMINUTE_ten > 6) {
                                aMINUTE_ten = 0;
                            }
                        }
                        key2pushflag = 0;
                    } else if (key3pushflag == 1) {
                        aMINUTE_ten--;
                        if (aMINUTE_unit > 0) {
                            if (aMINUTE_ten < 0) {
                                aMINUTE_ten = 5;
                            }
                        } else {
                            if (aMINUTE_ten < 0) {
                                aMINUTE_ten = 6;
                            }
                        }
                        key3pushflag = 0;
                    }
                    Alarm_TIME[3] = aMINUTE_ten;
                    break;

                case 10:// aMINUTE_unit
                    if (key2pushflag == 1) {
                        aMINUTE_unit++; //
                        if (aMINUTE_ten == 6) {
                            aMINUTE_unit = 0;
                        } else {
                            if (aMINUTE_unit > 9) {
                                aMINUTE_unit = 0;
                            }
                        }
                        key2pushflag = 0;
                    } else if (key3pushflag == 1) {
                        aMINUTE_unit--;
                        if (aMINUTE_ten == 6) {
                            aMINUTE_unit = 0;
                        } else {
                            if (aMINUTE_unit < 0) {
                                aMINUTE_unit = 9;
                            }
                        }

                        key3pushflag = 0;
                    }
                    Alarm_TIME[2] = aMINUTE_unit;
                    break;
                case 11:// aSECOND_ten
                    if (key2pushflag == 1) {
                        aSECOND_ten++; //
                        if (aSECOND_unit > 0) {
                            if (aSECOND_ten > 5) {
                                aSECOND_ten = 0;
                            }
                        } else {
                            if (aSECOND_ten > 6) {
                                aSECOND_ten = 0;
                            }
                        }
                        key2pushflag = 0;
                    } else if (key3pushflag == 1) {
                        aSECOND_ten--;
                        if (aMINUTE_unit > 0) {
                            if (aSECOND_ten < 0) {
                                aSECOND_ten = 5;
                            }
                        } else {
                            if (aSECOND_ten < 0) {
                                aSECOND_ten = 6;
                            }
                        }
                        key3pushflag = 0;
                    }
                    Alarm_TIME[1] = aSECOND_ten;
                    break;
                case 12:// aSECOND_unit
                    if (key2pushflag == 1) {
                        aSECOND_unit++; //
                        if (aSECOND_ten == 6) {
                            aSECOND_unit = 0;
                        } else {
                            if (aSECOND_unit > 9) {
                                aSECOND_unit = 0;
                            }
                        }
                        key2pushflag = 0;
                    } else if (key3pushflag == 1) {
                        aSECOND_unit--;
                        if (aSECOND_unit < 0) {
                            aSECOND_unit = 9;
                        }
                        key3pushflag = 0;
                    }
                    Alarm_TIME[0] = aSECOND_unit;
                    break;
            }
            Trans_Alarm_bit(); //-------将读取的闹钟时间写入变量-------------//
            AlarmDataDisplay(); //---------------将闹钟变量写入LCD--------------//
            break;
        }
    }
}