����--2016.05.25 -- ����

cmysql -- �������ݿ�
	-cmysql.c
	-cmysql.h

�����Ѹ�Ϊ�����

main.c -- 
	--server_run()
		if(fork() == 0)
		{
			printf("start phone\n");
				/*get socket descriptor.*/
			int sockfd = socket_initialize(PHONE_PORT);
			Targ arg;
			arg.sockfd = sockfd;
			arg.port = PHONE_PORT;
			_server_start_thread((void *)&arg);
		}
		else
		{
			printf("start bed\n");
				/*get socket descriptor.*/
			int sockfd2 = socket_initialize(BED_PORT);
			Targ arg;
			arg.sockfd = sockfd2;
			arg.port = BED_PORT;
			_server_start_thread((void *)&arg);
		}


���������е��ڰ����Ƶ�ubuntu

���Ӱ����ƣ���xshell/SecureCRT Զ�����ӣ�SSH2��
	ip:121.42.190.117
	�û�����wuyubo
	���룺3113001629
	�����ڣ� /home/wuyubo 
	����Ĭ���Ѿ�����
	
�ϴ����밢���ƣ� ��WinSCP ftp����
	ip:121.42.190.117
	�û�����wuyubo
	���룺3113001629
	Ŀ¼Ϊ /home/wuyubo 