#ifndef __SONG_H_
#define	__SONG_H_

#include <xc.h> 
extern signed char SongNum;
extern bit isMusicPlaying;//播放还是暂停



#define Fosc 4000000//Hz
#define BUZZER PORTCbits.RC2



char backup_pos;
unsigned char TempTone;
unsigned char TempMetre;


void PwmInit(void);
void PwmTone(unsigned char n);
 void MusicPlay(void);
//void isMUSIC_MODE(void);
void MUSIC_init(void);

#endif

