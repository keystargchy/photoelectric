
CC:=arm-none-linux-gnueabi-gcc

all:
	$(CC) -o e18-d80nk e18-d80nk.c gpio.c -lpthread

clean:
	rm e18-d80nk
cp:
	cp e18-d80nk /home/keystar/samba_share/tftpboot