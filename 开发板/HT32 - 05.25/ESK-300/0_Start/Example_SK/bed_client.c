#include "bed_types.h"

//******************************************************************
//º¯ÊýÃû£º  set_data_packet
//¹¦ÄÜ£º    
//ÊäÈë²ÎÊý£º
//·µ»ØÖµ£º 
//ÈÕÆÚ£º  2016.04.19
//ÐÕÃû£º	ÎâÓñ²¨
//******************************************************************
struct DATA_PACKET 		data_packet_buff;
struct COMMAND_PACKET command_packet_buff;
struct COMMAND_PACKET Send_cmd;
static u8 grobal_play_ctr ;
static u8 grobal_swing_ctr;
u8 task;

/*init*/
void client_Init(void)
{
	grobal_play_ctr  = CLOSE;
	grobal_swing_ctr = ROCK_OFF_CTR;
	task = TASK_CMD;
	Send_cmd.header.ck1 = 0xFB;
	Send_cmd.header.ck2 = 0x0A;
	Send_cmd.cmd.ctr = 0;
	
	data_packet_buff.header.ck1 = 0xFB;
	data_packet_buff.header.ck2 = 0x0A;
	data_packet_buff.header.type = 0;
	data_packet_buff.header.length = sizeof(struct BED_DATA);
	data_packet_buff.data.time = 0xffff;
}

/*client thread*/
//ÎâÓñ²¨ 2016.05.03
int bed_client(void)
{
	int sockfd;
	u8 ret;
	while(1) {
		/*get bed data*/
		ret = get_data_packet(&data_packet_buff);
		if(ret !=0) goto client_Notconnet_err;
		/*wait to connect again*/
		delay_ms(800);	
		/*Create socket.*/
		sockfd = socket_create("121.42.190.117", 8890);
		printf("socekfd=%d\r\n", sockfd);
		if(sockfd == CLIENT_ERR) goto client_dealing_err;
		/*Send device MAC to server  */
		if(send_device_mac_type(sockfd)!= CLIENT_ERR) {
			printf("check MAC success\r\n");
			//·¢ËÍÃüÁî
			send_command_to_server(sockfd, task);
			//½ÓÊÕÃüÁî
			/*get cmd*/
			printf("get comand\r\n");
			ret = get_command_from_server(sockfd, &command_packet_buff);
			if(ret == CLIENT_ERR) goto client_dealing_err;
			task = TASK_DATA;
			delay_ms(100);
		//·¢ËÍ×´Ì¬
			printf("send data\r\n");
			ret = send_bed_data(sockfd, &data_packet_buff);
			if(ret == CLIENT_ERR) goto client_dealing_err;
			task = TASK_CMD;
		}
		else {
			goto client_dealing_err;
		}
		printf("disconnect...\r\n");
		Wifi_CloseSocket();//¹Ø±Õsocket
		clear_grobalBuff();
		//´¦Àí½ÓÊÕµ½µÄÃüÁ
		Deal_Command(&command_packet_buff);
	}
	return 0;
	
client_dealing_err:
	delay_ms(1000);
	clear_grobalBuff();
	printf("err::disconnect...\r\n");
	Wifi_CloseSocket();
client_Notconnet_err:
	delay_ms(500);
	return CLIENT_ERR;
}


//******************************************************************
//º¯ÊýÃû£º  auto_Contrl
//¹¦ÄÜ£º    
//ÊäÈë²ÎÊý£º
//·µ»ØÖµ£º 
//ÈÕÆÚ£º  2016.04.20
//ÐÕÃû£º	ÎâÓñ²¨
//******************************************************************
void auto_Contrl(struct BED_STATUS *data_status)
{
	static u32 i = 0;
	static u32 j = 0;
	if(data_status->cry)
	{
		j++;
		if(j<3) return;
		Modules_Contrl(SWING, ROCK_ON_CTR);
		Modules_Contrl(PLAY,  OPEN);
		j=0;
	}
	else
	{
		i++;
		if(i < 15) return;
		if(grobal_swing_ctr == ROCK_OFF_CTR)
					Modules_Contrl(SWING, ROCK_OFF_CTR);
		if(grobal_play_ctr == CLOSE)
					Modules_Contrl(PLAY,  CLOSE);
		i = 0;
	}
}
//******************************************************************
//º¯ÊýÃû£º  set_data_packet
//¹¦ÄÜ£º    
//ÊäÈë²ÎÊý£º
//·µ»ØÖµ£º 
//ÈÕÆÚ£º  2016.04.20
//ÐÕÃû£º	ÎâÓñ²¨
//******************************************************************
INT8U set_data_packet(struct DATA_PACKET *dpacket)
{
	struct BED_STATUS status;
	struct TEMPERATURE tempe;
	/*check temperature*/
	tempe = Check_Temperature();
	dpacket->data.baby_temperature = tempe.baby_temperature;
	dpacket->data.room_temperature = tempe.room_temperature;

	
	status.cry = Check_Cry();	
	status.heat = Check_Milk(); 
	status.kick = Check_Quilt(); 
//	status.onbed = Check_Onbed(); 
	status.onbed = is_onbed(&tempe);
	status.pee = Check_Piss();
	status.play= Check_Music();;
	status.swing = Check_Rock();
	status.fan = Check_Fan();
	dpacket->data.status = status;
	printf("[r%d,b%d] %d \r\n", dpacket->data.room_temperature, dpacket->data.baby_temperature, dpacket->header.length);
	auto_Contrl(&dpacket->data.status);
	return 0;
}


/*Device collects data.*/
INT8U get_data_packet(struct DATA_PACKET * data_packet)
{
	return set_data_packet(data_packet);
}

int wait_respone(int sockfd)
{
	int ret;
	struct COMMAND_PACKET cpacket;
	ret = read(sockfd, (u8 *)&cpacket, sizeof(struct COMMAND_PACKET), 0);
	if(ret == -1) return -1;
	return 0;
}

/*Send bed data to server*/
u8 send_bed_data(int sockfd, struct DATA_PACKET *dpacket)
{
	 if(send(sockfd, (u8 *)dpacket, sizeof(struct DATA_PACKET), 0) == -1) return CLIENT_ERR;
	 if(wait_respone(sockfd)== -1) return CLIENT_ERR;
	 return 0;
}

/*create a socket connection.*/
/*Return socket ID.*/
int socket_create(char *ip, int port)
{
	int sockfd = 0;
	sockfd = Wifi_ConnectSocket(ip, port);
	if(sockfd == SOCKET_ERR) return CLIENT_ERR;
	return sockfd;
}

/*Get MAC from WIFI module*/
int get_mac(char *buf)
{
	char mac[6] = {0x5E, 0x60, 0x7F, 0x9A, 0xCC, 0x91};
	memcpy(buf, mac, 6);
	return 1;
}

/*Check MAC*/
u8 check_mac(struct DEVICE *device, struct DEVICE *recdev)
{
	int i;
	if(recdev->type != device->type)
	{		
		printf("connect unsuccessfully.\n");
		return CLIENT_ERR;
	}
	for(i=0; i<6; i++) {
		if(device->mac[i] != recdev->mac[i]) {
				printf("connect unsuccessfully.\n");
				return CLIENT_ERR;
		}
	}
//	printf("connect successfully.\n");
	return 0;
}

int send_device_mac_type(int sockfd)
{
	int ret;
	struct DEVICE device, recdev;
	memset((char *)&device, 0, sizeof(device));
	
	get_mac((char *)device.mac);
	device.type = TYPE_BED;

	/*Send*/
	printf("send MAC\r\n");
	 ret = send(sockfd, (u8 *)&device, sizeof(struct DEVICE), 0);
	 memset((char *)&recdev, 0, sizeof(recdev));
	delay_ms(300);
	printf("read mac\r\n");
	/*Receive response from server. */
	ret = read(sockfd, (u8 *)&recdev, sizeof(recdev), 0);
	if(ret == SEM_ERR) return CLIENT_ERR;
	
	/*Check MAC*/
  if(check_mac(&device, &recdev) == CLIENT_ERR) return CLIENT_ERR;
//	printf("connect successfully.\n");
	return 0;
}

/*send command to server*/
int send_command_to_server(int sockfd, u8 task)
{
	int ret;
	Send_cmd.cmd.cmd = task;
	ret = send(sockfd, (u8 *)&Send_cmd, sizeof(struct COMMAND_PACKET), 0);
	return ret;
}

/*get command from server*/
INT8U get_command_from_server(int sockfd, struct COMMAND_PACKET *command_packet)
{
	int ret;
	memset((u8 *)command_packet, 0, sizeof(struct COMMAND_PACKET));
	ret = read(sockfd,(u8 *)command_packet, sizeof(struct COMMAND_PACKET), 0);
	if(ret == -1) return CLIENT_ERR;
	return 0;
}


/*check header*/
u8 Check_header(struct HEADER *header, u32 size)
{
	if(header->ck1 != 0xFB) return CLIENT_ERR;
	if(header->ck2 != 0x0A) return CLIENT_ERR;
//	if(header->length != (size - sizeof(struct HEADER))) return CLIENT_ERR;
	if(header->type != TYPE_PHONE) return CLIENT_ERR;
	return 0;
}

//******************************************************************
//º¯ÊýÃû£º  Modoules_Contrl
//¹¦ÄÜ£º    ´¦ÀíÃüÁî
//ÊäÈë²ÎÊý£ºÎÞ
//·µ»ØÖµ£º  
//ÈÕÆÚ£º  2016.04.20
//ÐÕÃû£º	ÎâÓñ²¨
//******************************************************************
INT8U Modules_Contrl(u8 ctr, u8 cmd)
{
//	printf("ctr=%d cmd=%d", ctr, cmd);
//	run_led(LED_2, 20);
	switch(ctr)
	{
		case HEAT:
			printf("HEAT contrl\r\n");
			Fan_Contorl(cmd);
			Milk_Contorl(cmd);
			break;
		case SWING:
			printf("SWING contrl\r\n");
			Rock_Control(cmd);
			break;  
		case PLAY:
			printf("play contrl\r\n");
			Music_Control(cmd);
			break;
		case FAN:
			printf("FAN contrl\r\n");
			Fan_Contorl(cmd);
			break;
	}
	return 0;
}

//******************************************************************
//º¯ÊýÃû£º  Deal_Command
//¹¦ÄÜ£º    ´¦ÀíÃüÁî
//ÊäÈë²ÎÊý£ºÎÞ
//·µ»ØÖµ£º  
//ÈÕÆÚ£º  2016.04.20
//ÐÕÃû£º	ÎâÓñ²¨
//******************************************************************
INT8U Deal_Command(struct COMMAND_PACKET *command_packet)
{
//	if(Check_header(&command_packet->header, sizeof(command_packet->cmd)) == CLIENT_ERR) 
//				return CLIENT_ERR;
//	printf("H 0x%.2x 0x%.2x", command_packet->header.ck1, command_packet->header.length);
	if(command_packet->cmd.ctr == PLAY)
		 grobal_play_ctr = (command_packet->cmd.cmd == ROCK_OFF_CTR? ROCK_OFF_CTR:ROCK_ON_CTR);
	if(command_packet->cmd.ctr == SWING)
		 grobal_swing_ctr= (command_packet->cmd.cmd == CLOSE? CLOSE:OPEN);
	return Modules_Contrl(command_packet->cmd.ctr, command_packet->cmd.cmd);
}

