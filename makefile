CC=gcc
all:server clientu clientt
.PHONY:all

server:echo_server.o
	$(CC) -o server echo_server.o
clientu:echo_UDP_client.o
	$(CC) -o clientu echo_UDP_client.o
clientt:echo_TCP_client.o
	$(CC) -o clientt echo_TCP_client.o
echo_server.o:echo_server.c
	$(CC) -c echo_server.c
echo_UDP_client.o:echo_UDP_client.c
	$(CC) -c echo_UDP_client.c
echo_TCP_client.o:echo_TCP_client.c
	$(CC) -c echo_TCP_client.c

.PHONY:clean
clean:
	rm *.o server clientu clientt	
