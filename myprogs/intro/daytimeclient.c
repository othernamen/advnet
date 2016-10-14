#include "unp.h"

int main(int argc, char **argv) {
	int 	sockfd, n, counter = 0;
	char	recvline[MAXLINE+1];
	struct	sockaddr_in servaddr;

	if (argc != 3) 
		err_quit("usage: daytimeclient <IPADDRESS>");

	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
		err_sys("socket error");

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(atoi(argv[2]));		// Assign Port
		
	if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) // convert dotted decimal IP to network byte order
		err_sys("connect error");

	if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr))<0) {
		err_sys("connect error");
	}

	while ( (n = read (sockfd, recvline, MAXLINE)) > 0 ) {
		counter++;
		recvline[n] = 0;
		if (fputs(recvline, stdout) == EOF) 		// print data time and date returned from server
			err_sys("fputs error");

	}

	if (n < 0) {
		err_sys("read error11");
		printf("%d\n", n);
	}

//	printf("counter = %d\n", counter);
	 
	exit(0);

}
