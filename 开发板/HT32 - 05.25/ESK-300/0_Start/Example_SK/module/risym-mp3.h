#ifndef _RISYM_MP3_H_
#define _RISYM_MP3_H_

#include "main.h"

#define	MUSIC_GPIO_PX	  _MUSIC_GPIO_PX
#define	MUSIC_HT_GPX    _MUSIC_HT_GPX

#define MUSIC_NEXT_PIN 	_MUSIC_NEXT_PIN
#define MUSIC_PREV_PIN 	_MUSIC_PREV_PIN
#define MUSIC_OPEN_PIN 	_MUSIC_OPEN_PIN
#define MUSIC_PAUSE_PIN _MUSIC_PAUSE_PIN


#define MUSIC_BT_UP 	RESET
#define MUSIC_BT_DOWN 	SET

#define MUSIC_STATUS_ON  1
#define MUSIC_STATUS_OFF 0

#define MUSIC_CONTR(contr_io) CPIO_OUT_BITS(MUSIC_HT_GPX, contr_io, MUSIC_BT_DOWN);\
															OSTimeDly(500);\
															CPIO_OUT_BITS(MUSIC_HT_GPX, contr_io, MUSIC_BT_UP)
															
#define VOLUME_CONTR(contr_io) CPIO_OUT_BITS(MUSIC_HT_GPX, contr_io, MUSIC_BT_DOWN);\
															OSTimeDly(800);\
															CPIO_OUT_BITS(MUSIC_HT_GPX, contr_io, MUSIC_BT_UP)
															
#define CHANGE_MODE()         CPIO_OUT_BITS(MUSIC_HT_GPX, MUSIC_PAUSE_PIN, MUSIC_BT_DOWN);\
															OSTimeDly(1000);\
															CPIO_OUT_BITS(MUSIC_HT_GPX, MUSIC_PAUSE_PIN, MUSIC_BT_UP)
															
#define MUSIC_CLOSE						CPIO_OUT_BITS(MUSIC_HT_GPX, MUSIC_OPEN_PIN, MUSIC_BT_UP)
#define MUSIC_OPEN						CPIO_OUT_BITS(MUSIC_HT_GPX, MUSIC_OPEN_PIN, MUSIC_BT_DOWN)

typedef enum music
{
	CLOSE = 0,		/*'0' 关闭*/
	OPEN,						/*'1' 打开*/
	NEXT,						/*'2' 下一曲*/
	PREV,						/*'3' 上一曲*/
	MODE,						/*'4' 切换模式SD/USB*/
	PAUSE,					/*'5' 暂停*/
}MUSIC_CONTR;

void Music_Init(void);
void Mp3_Init_Set(void);
u8 Music_Control(u8 ctrl);
u8 Check_Music(void);

#endif
