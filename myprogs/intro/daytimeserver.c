#include "unp.h"
#include <time.h>


int main(int argc, char **argv) {

	int listenfd, connfd;
	socklen_t conlen;
	struct sockaddr_in servaddr;
	char   buff[MAXLINE];
	time_t ticks;
	


	if(argc!=2)
		err_quit("usage: ./daytimeserver <port_no>");

	listenfd = Socket (AF_INET, SOCK_STREAM, 0);


	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family =	AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);			//connect to any local address
	servaddr.sin_port	 = htons(atoi(argv[1]));

	


	if( bind(listenfd, (SA *) &servaddr, sizeof(servaddr)))
		err_sys("Error binding");
	
	listen(listenfd, LISTENQ);

	for (; ;) {

	conlen = sizeof(servaddr);
	connfd = accept(listenfd, (SA *) &servaddr, &conlen);
	if(connfd<0){
		err_sys("connect accept error");	
	}


	ticks = time(NULL);
	snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
	Write(connfd, buff, strlen(buff));

	close(connfd);

	}

}
