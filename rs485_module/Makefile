
obj-m += rs485.o

KERNEL = $(HOME)/kernel/

all:
	make -C $(KERNEL) M=`pwd` modules
	cp   ./rs485.ko /tftpboot
clean:
	make -C $(KERNEL) M=`pwd` clean
