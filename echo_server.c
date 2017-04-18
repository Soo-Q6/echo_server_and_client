#include<sys/socket.h>
#include<strings.h>
#include<signal.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<stdlib.h>
#include<errno.h>
#define serv_port 9877
#define LISTENQ 1024
#define MAXLINE 1024
void str_echo(int sockfd);
void sig_chid(int signo);
int main()
{
	int listenid, connfd;
	pid_t childpid;
	socklen_t clilen;
	int keepalive=1;
	//int reuse=1;
	struct linger LINGER;
	struct sockaddr_in cliaddr, servaddr;
	LINGER.l_onoff=1;
	LINGER.l_linger=0;

	if ((listenid = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		printf("socket errro");
		exit(0);
	}
	setsockopt(listenid,SOL_SOCKET,SO_KEEPALIVE,&keepalive,sizeof(keepalive));
	// if(setsockopt(listenid,SOL_SOCKET,SO_REUSEPORT,&reuse,sizeof(reuse))==-1){
	// 	printf("%s\n",strerror(errno));
	// 	exit(0);
	// }else{
	// 	printf("set socket successfully\n");
	// }

	// if(setsockopt(listenid,SOL_SOCKET,SO_LINGER,&LINGER,sizeof(LINGER))==-1){
	// 	printf("%s\n",strerror(errno));
	// 	exit(0);
	// }else{
	// 	printf("set socket successfully\n");
	// }

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(serv_port);
	if (bind(listenid, (const struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) {
		printf("bind:%s(errno:%d)\n", strerror(errno), errno);
		exit(0);
	}

	if (listen(listenid, LISTENQ) == -1) {
		printf("listen:%s(errno:%d)\n", strerror(errno), errno);
		exit(0);
	}
	else {
		printf("waiting for new connection\n");
	}
	if (signal(SIGCHLD, sig_chid) < 0) {
		printf("%s,%d", strerror(errno), errno);
	}

	while (1) {
		clilen = sizeof(cliaddr);
		connfd = accept(listenid, (struct sockaddr*)&cliaddr, &clilen);
		if (connfd < 0) {
			if (errno == EINTR)
				continue;
			else
				printf("accept error:%s (errno:%d)\n", strerror(errno), errno);

		}
		if ((childpid = fork()) == 0) {
			close(listenid);
			str_echo(connfd);
			exit(0);
		}			
		close(connfd);
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
