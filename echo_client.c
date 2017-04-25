#include<sys/types.h>
#include<sys/socket.h>
#include<time.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>
#define SERV_PORT 8888
#define MAXLINE 1024
void str_cli(FILE*fp, int sockfd,const struct sockaddr *cliaddr,socklen_t servlen);
int main(int argc, char **argv) {
	int sockfd;
	struct sockaddr_in servaddr,clientaddr;
	socklen_t clilen;
	if(argc!=2){
		printf("usefjds");
		exit(0);
	}

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

	str_cli(stdin, sockfd,( struct sockaddr *)&servaddr,sizeof(servaddr));
	exit(0);
}



void str_cli(FILE*fp, int sockfd,const struct sockaddr *cliaddr,socklen_t servlen) {
	char sendline[MAXLINE], recvline[MAXLINE+1];
	int n;
	struct sockaddr_in caddr;
	socklen_t clilen=sizeof(caddr);
	while (fgets(sendline, MAXLINE, fp) != NULL) {
		sendto(sockfd,sendline,strlen(sendline),0,cliaddr,servlen);
		n=recvfrom(sockfd,recvline,MAXLINE,0,(struct sockaddr*)&caddr,&clilen);
		recvline[n]=0;
		fputs(recvline, stdout);
		n=recvfrom(sockfd,recvline,MAXLINE,0,(struct sockaddr*)&caddr,&clilen);
		recvline[n]=0;
		fputs(recvline, stdout);
	}
}
