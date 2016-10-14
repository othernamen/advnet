#include "unp.h"
#include  <sys/types.h>

// Function which returns 1 if request is GET
int request_type(char *request) {
int n;
int request_code = 0;								// 1 is equal to GET request
char* string;
char http_version[32]="";
char request_t[32]="";
char page_address[MAXLINE]="";

		string = strdup(request);
		printf("--------------------------------------------------------------\
		%s\n--------------------------------------------------------------\n",request);
		sscanf(string, "%s %s %s\r\n", request_t, page_address, http_version);
		printf("request_t:%s\npage_address:%s\nhttp_version:%s\n", request_t, page_address, http_version);
		if(!strcmp("GET",request_t) && !strcmp("HTTP/1.1", http_version))
			request_code = 1;						// Its a GET!

	return request_code;

			
}

// Function which tells which page was requested. Returns string. example /index.html
char* page_requested(char *request) {

char* string;
char* token;
char page_name[MAXLINE]; 

	string = strdup(request);
		if(string!=NULL) {
			token = strsep(&string, " ");	// From string "GET /index.html HTTP/1.1" take "GET"
			token = strsep(&string, " ");   // From string "GET /index.html HTTP/1.1" take "/index.html"	
			strcpy(page_name, token);
	}
	return strdup(page_name);
}

int service_get_request(int clientfd, char *page_name) {
FILE *fp;
char *p = strdup(page_name);
char *buf = malloc(2048);				// Can service only small files!
int  s=0;
char c;
	
        if(p!=NULL)
	if(p[0]=='/')					// Remove trailing / from filename if exists
		p++;

	printf("opening file:%s\n", p);
        if(fp = fopen(p, "r")) {
                printf("file opened\n");
		while((c=getc(fp))!=EOF){
			buf[s]=c;
			s++;
		}			
		printf("%s\n",buf);
	}
	else {
		printf("Couldn't open requested file\n");
		strcpy(buf,"NO Such page 404 error\n");
	}
	// Write to client
	s=0;
	if(0>(s=write(clientfd,buf,strlen(buf))))
		printf("failed to write to client\n");
	else
		printf("Wrote %d characters\n",s);
	

	return 1;


}

// This function called when cliet connects
void connection(int clientfd) {
	int n;
	char readbuff[MAXLINE];
	char *query = readbuff;
	const char EOL = '\\0';
	char *page_name;
	printf("Client connected\n");
		
		// Read HTTP request
		if (read(clientfd,readbuff, MAXLINE)) {
				if(request_type(readbuff)==1) {
					page_name=page_requested(query);			
					fprintf(stdout,"Requested page: %s\n",page_name);
					service_get_request(clientfd, page_name);		// Service client connection, return a page
				}
				else
					fprintf(stdout, "Request type unknown\n");
		}

		fprintf(stdout,"Disconnecting client\n");
		close(clientfd);
		readbuff[n]=0;

}

int main(int argc, char **argv) {

int listenfd, connfd;
socklen_t conlen;
struct sockaddr_in servaddr;

	if(argc!=2)
		err_quit("usage: ./httpserver <port_no>");

	listenfd = Socket (AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family =	AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);			
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

		if(fork() == 0) 
			connection(connfd);	
		close(connfd);

	}

}
