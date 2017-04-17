CC=gcc
all:server client
.PHONY:all

server:echo_server.o
	$(CC) -o server echo_server.o
client:echo_client.o
	$(CC) -o client echo_client.o
echo_server.o:echo_server.c
	$(CC) -c echo_server.c
echo_client.o:echo_client.c
	$(CC) -c echo_client.c

.PHONY:clean
clean:
	rm *.o server client	
