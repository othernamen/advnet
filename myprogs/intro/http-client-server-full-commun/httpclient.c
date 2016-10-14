#include "unp.h"
#include <libgen.h>

void connection(int sockfd, char *page_name){

char html_request[MAXLINE];
char buf[2048]="basic";
int n;

		sprintf(html_request, "GET ");
		sprintf(html_request + strlen(html_request), "%s ", page_name);
		sprintf(html_request + strlen(html_request), "HTTP/1.1\r\n" );
		printf("Sending request\n");
		if(0>(n = write(sockfd, html_request, strlen(html_request))))
			printf("Error writing to server\n");
		else {
		while(read(sockfd, buf, strlen(buf)))
			printf("%s",buf);
		}
		
	
}

int main(int argc, char **argv) {
int 	sockfd, n, counter = 0;
char	recvline[MAXLINE+1];
struct	sockaddr_in servaddr;
char	request[32];
char	*page_name;
char	*server_address;
char	*tmp;

	if (argc != 3) 
		err_quit("usage: httpclient <http://address> <port_no>");
	else {
		page_name = strdup(argv[1]);
//		page_name = basename(argv[1]);

		server_address = strdup(argv[1]);
		if(strstr(argv[1],"http://")) {
	        	sscanf(argv[1], "http://%99[^/]%s", server_address, page_name);
		}
		else
	        	sscanf(argv[1], "%99[^/]", server_address);

		printf("server_address:%s\n", server_address);
		fprintf(stdout, "page_name: %s\n", page_name);

		}

	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
		err_sys("socket error");

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(atoi(argv[2]));		// Assign Port
		
	
	if (inet_pton(AF_INET, server_address, &servaddr.sin_addr) <= 0) // convert dotted decimal IP to network byte order
		err_sys("connect1 error");

	if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr))<0) {
		err_sys("connect2 error");
	}
	connection(sockfd, page_name);

	exit(0);

}
