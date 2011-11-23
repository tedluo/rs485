/*
 * functions for rs485 
 * */


#include "rs485.h"

static int rs485_fd;

static int open_rs485(void)
{
	if((rs485_fd = open("/dev/rs485",O_RDWR)) < 0)
	{
		fprintf(stderr,"Open device /dev/rs485 failed: %s\n",strerror(errno));
		return -1;
	}
	return 0;
}

static int close_rs485(void)
{
	if(close(rs485_fd) != 0)
	{
		fprintf(stderr,"Close device /dev/rs485 failed:%s\n",strerror(errno));
	}
	return 0;
}

/************************************************************/
/*  set rs485 work model 0 is receive,and 1 is translate    */
/********************************************************** */
static int rs485_set_model(unsigned int model)
{
	unsigned int buff[2];
	buff[0] = model;
	open_rs485();
	if(ioctl(rs485_fd,RS485_GPIO_SET_CMD,&buff) < 0)
	{
		fprintf(stderr,"Set RS485 model failed \n");
		return -1;
	}
	close_rs485();
	return 0;
}

/************************************************************/
/*  get rs485 work model 0 is receive,and 1 is translate    */
/********************************************************** */
static int rs485_get_stats(void)
{
	unsigned int buff[2] = {0};

	open_rs485();

	if(ioctl(rs485_fd,RS485_GPIO_READ_CMD,&buff) < 0)
	{
		fprintf(stderr,"Get RS485 stats failed \n");
		return -1;
	}

	if((buff[1]&0x1) == 0)
	{
		fprintf(stdout,"Now rs485 work int receive model\n");
	}
	else
	{
		fprintf(stdout,"Now rs485 work in translate model\n");
	}
	close_rs485();

	return buff[1]&0x1;
}
