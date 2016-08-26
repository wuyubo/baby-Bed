/*
 * bed_types.h
 *
 *  Created on: 2016-4-5
 *      Author: Lin Yixing
 */

#ifndef BED_TYPES_H_
#define BED_TYPES_H_

#include <stdint.h>
/**
 *  Illustrate: Data structures of the baby bed, which are use for socket transmitting.
 *
 * */

#define TASK_CMD 0
#define TASK_DATA 1
#define BED_PORT 	8890
#define PHONE_PORT  8899
/*database config*/
#define SERVER 		"localhost"
#define USER 		"root"
#define PASSWARD  	"123456"
#define DATABASE  	"Baby_Bed"
/*database*/

/*CONTROL COMMAND*/
#define NONE					0x00000000
#define WARM_CTRL_OFF 		0x00000001
#define WARM_CTRL_ON  		0x00000002
#define PHOTO_CTR_ON  		0x00000003     //拍照
#define CRADLE_CTRL_OFF     0x00000004     // 关闭摇床
#define CRADLE_CTRL_ON      0x00000005     //打开摇床
#define MUSIC_CTRL_NEXT     0x00000006     //控制下一曲
#define MUSIC_CTRL_PRE      0x00000007     //控制上一曲
#define MUSIC_CTRL_STOP     0x00000008     //控制暂停
#define MUSIC_CTRL_ON       0x00000009     //控制播放
#define GET_BED_DATA			0x0000000A		 //请求获取婴儿床数据
/**
 * NOTE: Header of data packet(length: 32bit)
 */
/*type*/
#define COMMAND   0
#define DATA		1
struct HEADER {
	uint16_t ck1:8;
	uint16_t ck2:7;
	uint16_t type:1;

	uint16_t length;
};

/**
 * NOTE:MODULES CONTRL
 */
enum MODULES_CTRL
{
	HEAT=1,
	SWING,
	PLAY,
	FAN,
};

/**
 * NOTE: Device's MAC and type. (length: 17bit).
 */
/*type*/
#define TYPE_PHONE   0
#define TYPE_BED	   1

struct DEVICE {
	uint8_t mac[6];
	uint8_t type;
};

struct BED_STATUS {
	uint8_t kick:1;
	uint8_t heat:1;
	uint8_t onbed:1;
	uint8_t cry:1;
	uint8_t swing:1;
	uint8_t pee:1;
	uint8_t play:1; 	//music is playing
	uint8_t fan:1; 
	uint8_t reserve:8;  //reserve for other modules
};

struct BED_DATA {
	int room_temperature;//8 byte
	int baby_temperature;
	int time;			//4 byte
	struct BED_STATUS status;//1
};

/**
 * NOTE: Data packet.
 */
struct DATA_PACKET {
	struct HEADER header;
	struct BED_DATA data;
};
/**
 * NOTE: Command
 */
struct CMD{
	uint8_t ctr;
	uint8_t cmd;
};

/**
 * NOTE: Command packet.
 */
struct COMMAND_PACKET {
	struct HEADER header;
	struct CMD cmd;
};

typedef struct _TARG{
	int sockfd;
	int16_t port;
}Targ;


#endif /* BED_TYPES_H_ */
