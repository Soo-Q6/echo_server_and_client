#include<sys/socket.h>
#include<strings.h>
#include<signal.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<errno.h>
#define serv_port 8888
#define LISTENQ 1024
#define MAXLINE 1024
void str_echo(int sockfd,struct sockaddr_in *cliaddr,socklen_t clilen);
// void sig_chid(int signo);
int main()
{
	int listenid, connfd;
	struct sockaddr_in cliaddr, servaddr;

	if ((listenid = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		printf("socket errro");
		exit(0);
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(serv_port);
	if (bind(listenid, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) {
		printf("bind:%s(errno:%d)\n", strerror(errno), errno);
		exit(0);
	}


	str_echo(listenid,&cliaddr,sizeof(cliaddr));

}


void str_echo(int sockfd,struct sockaddr_in * cliaddr,socklen_t clilen) {
	ssize_t len;
	int n;
	char buf[MAXLINE];
	while (1) {
		len=clilen;
		n=recvfrom(sockfd,buf,MAXLINE,0,(struct sockaddr *)cliaddr,&len);
		printf("connection from:%s  port:%d\n",inet_ntoa(cliaddr->sin_addr),ntohs(cliaddr->sin_port));
		sendto(sockfd,buf,n,0,cliaddr,len);
	}
}

// void sig_chid(int signo) {
// 	pid_t pid;
// 	int stat;
// 	while ((pid = wait(&stat)) > 0)
// 		printf("child:%d terminated", pid);
// 	return;
// }