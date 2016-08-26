修改日期：2016.05.25 ------- 吴玉波

wifi连接--
	用户名：656
	密码：wuwuwuwei
	--已经写死在程序中，
	  要调试请自行更改（在spi_wifi.c)
	  或用手机设置对应的用户名和密码开热点使用


工程文件在  .\HT32\ESK-300\0_Start\Example_SK\MDK_ARM

代码文件在  .\HT32\ESK-300\0_Start\Example_SK\
	    .\HT32\ESK-300\0_Start\Example_SK\module


工程目录

主要用到的有：

--user
  -- main.c 
  -- tasks.c   --- 所有线程
  -- sem.c     --- 信号量的封装
  -- bed_client.c  --- 与服务器通信的线程的代码   ****重要*****
  -- system.c   --- read() 接收socket信息的封装
  ...

--modules
  ...	各模块代码	

重要头文件：

main.h ------ 关联所有模块的宏

