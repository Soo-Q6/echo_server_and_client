#include<sys/socket.h>
#include<strings.h>
#include<signal.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<stdio.h>
#include<errno.h>
#define SERV_PORT 8888
#define LISTENQ 1024
#define MAXLINE 1024
void str_echo(int sockfd);
void sig_chid(int signo);
int max(int a,int b);
int main(int argc, char **argv)
{
    int
	listenfd,
	connfd, udpfd, nready, maxfdp1;
    char
	mesg[MAXLINE];
    pid_t
	childpid;
    fd_set
	rset;
    ssize_t
	n;
    socklen_t
	len;
    const int
	on = 1;
    struct sockaddr_in
	cliaddr,
	servaddr;
    void
    sig_chid(int);
    /* for create listening TCP socket */
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    listen(listenfd, LISTENQ);
    /* for create UDP socket */
    udpfd = socket(AF_INET, SOCK_DGRAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);
    bind(udpfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    signal(SIGCHLD, sig_chid); /* must call waitpid() */
    FD_ZERO(&rset);
    maxfdp1 = max(listenfd, udpfd) + 1;
    for (;;)
    {
	FD_SET(listenfd, &rset);
	FD_SET(udpfd, &rset);
	if ((nready = select(maxfdp1, &rset, NULL, NULL, NULL)) < 0)
	{
	    if (errno == EINTR)
		continue;
	    /* back to for() */
	    else
		printf("select error");
	}
	if (FD_ISSET(listenfd, &rset))
	{
	    len = sizeof(cliaddr);
	    connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &len);
	    if ((childpid = fork()) == 0)
	    {
		/* child process */
		close(listenfd);
		/* close listening socket */
		str_echo(connfd);
		/* process the request */
		exit(0);
	    }
	    close(connfd);
	    /* parent closes connected socket */
	}
	if (FD_ISSET(udpfd, &rset))
	{
	    len = sizeof(cliaddr);
	    n = recvfrom(udpfd, mesg, MAXLINE, 0, (struct sockaddr *)&cliaddr, &len);
	    sendto(udpfd, mesg, n, 0, (struct sockaddr *)&cliaddr, len);
	}
    }
}


void str_echo(int sockfd) {
	ssize_t n;
	char buf[MAXLINE];
	struct sockaddr_in clientaddr;
	socklen_t clientLen = sizeof(clientaddr);
	getpeername(sockfd, (struct sockaddr*)&clientaddr, &clientLen);
	printf("connection form:%s  port:%d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
again:
	while ((n = read(sockfd, buf, MAXLINE)) > 0) {
		buf[n] = '\0';
		write(sockfd, buf, n);

	}
	if (n < 0 && errno == EINTR)
		goto again;
	else if (n < 0)
		printf("str_echo:read error");
}

void sig_chid(int signo) {
	pid_t pid;
	int stat;
	while ((pid = wait(&stat)) > 0)
		printf("child:%d terminated", pid);
	return;
}
int max(int a,int b){
	return a>b?a:b;
}
