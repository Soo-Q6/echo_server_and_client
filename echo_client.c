#include<sys/types.h>
#include<sys/socket.h>
#include<time.h>
#include<netinet/in.h>
#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>
#include<stdlib.h>
#include<stdio.h>
//#include<stdbool.h>
#define SERV_PORT 9877
#define MAXLINE 1024
void str_cli(FILE*fp, int sockfd);
int main(int argc, char **argv) {
	int sockfd;
	struct sockaddr_in servaddr;
	int keepalive=1;
	if(argc!=2){
		printf("usefjds");
		exit(0);
	}

	char *ipaddr = "127.0.0.1";
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	inet_pton(AF_INET, ipaddr, &servaddr.sin_addr);
	setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &keepalive, sizeof(keepalive));

	if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
	{
	    printf("connect error!\n");
	    exit(0);
	}
	else
	{
	    printf("connect successfully!\n");
	}

	str_cli(stdin, sockfd);
	exit(0);
}



void str_cli(FILE*fp, int sockfd) {
	char sendline[MAXLINE], recvline[MAXLINE];
	while (fgets(sendline, MAXLINE, fp) != NULL) {
		write(sockfd, sendline, strlen(sendline));
		if (read(sockfd, recvline, strlen(sendline)) == 0) {
			printf("sre_cli:error");
			exit(0);

		}
		fputs(recvline, stdout);
	}
}
