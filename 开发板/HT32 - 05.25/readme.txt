修改日期：2016.05.03 ------- 吴玉波


工程文件在  .\HT32\ESK-300\0_Start\Example_SK\MDK_ARM

代码文件在  .\HT32\ESK-300\0_Start\Example_SK\
	    .\HT32\ESK-300\0_Start\Example_SK\module


工程目录

主要用到的有：

--user
  -- main.c 
  -- tasks.c   --- 所有线程
  -- sem.c     --- 信号量的封装
  -- bed_client.c  --- 与服务器通信的线程的代码
  -- system.c   --- read() 接收socket信息的封装
  ...

--modules
  ...	各模块代码	

重要头文件：

main.h ------ 关联所有模块的宏