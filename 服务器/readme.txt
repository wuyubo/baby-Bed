日期--2016.05.25 -- 吴玉波

cmysql -- 连接数据库
	-cmysql.c
	-cmysql.h

程序已改为多进程

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


服务器运行到在阿里云的ubuntu

连接阿里云：用xshell/SecureCRT 远程连接（SSH2）
	ip:121.42.190.117
	用户名：wuyubo
	密码：3113001629
	代码在： /home/wuyubo 
	程序默认已经开启
	
上传代码阿里云： 用WinSCP ftp连接
	ip:121.42.190.117
	用户名：wuyubo
	密码：3113001629
	目录为 /home/wuyubo 