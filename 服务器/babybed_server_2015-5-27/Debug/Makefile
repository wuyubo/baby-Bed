EXAMPLES ?=  baby_server 

CC = gcc
ROOTDIR = ../
SRC= $(ROOTDIR)main.c $(ROOTDIR)bed_server.c \
	$(ROOTDIR)bed_thread.c $(ROOTDIR)phone_thread.c \
    $(ROOTDIR)cmysql/mutex_lock.c $(ROOTDIR)cmysql/cmysql.c
	 
LIBS = -lmysqlclient -pthread
OBJS = -Wall
RM := rm -rf

all: $(EXAMPLES)




	
	 
baby_server:  $(SRC) 
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	
	$(CC)  $(SRC) -o $@  $(OBJS) $(LIBS) 
	
	@echo 'Finished building target: $@'
	@echo 'run: $@ LoalPort(8899) '
	

clean: 
	$(RM) $(EXAMPLES)
