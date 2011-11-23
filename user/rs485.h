#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/*for function open*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/*for function ioctl*/
#include <sys/ioctl.h>

/*for function readi,write*/
#include <unistd.h>

#ifndef _RS485_H_
#define _RS485_H_
/*cmds translate to ioctl*/
#define RS485_GPIO_SET_CMD 	0x1
#define RS485_GPIO_READ_CMD	0x2

#define RS485_RECEIVE_MODEL 	0x0
#define RS485_TRANS_MODEL	0x1

#endif
