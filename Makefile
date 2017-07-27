CFLAGS=-W -Wall -Werror -std=c11

all: clean tcpclient tcpserver

tcpserver:
	$(CC) $@.c -o $@

tcpclient:
	$(CC) $@.c -o $@

clean: 
	$(RM) tcpclient tcpserver

