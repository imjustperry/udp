#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

#define MAXLINE 1024 

int main(int argc, char **argv) {
	int sockfd;
	int n = 0;
	int portNumber;
	char buffer[MAXLINE];
	char from_ip[1024] = "";
	struct sockaddr_in servaddr, cliaddr; 
	
	if (argc != 3) {
		perror("Incorrect usage");
		exit(0);
	}

	portNumber = atoi(argv[2]);

	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
		perror("socket creation failed"); 
		exit(0); 
	} 

	memset((char *)&servaddr, 0, sizeof(servaddr));
	memset(&cliaddr, 0, sizeof(cliaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(portNumber);

	if(bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
	{
		perror("bind failed");
		exit(0);
	}

	int addrlen = sizeof(struct sockaddr);
	int keepGoing = 0;

	while (keepGoing < 10) {
		addrlen = sizeof(struct sockaddr);
		n = recvfrom(sockfd, (char *)buffer, MAXLINE, 0, (struct sockaddr *)&cliaddr, &addrlen);
			buffer[n] = '\0';
			printf("Received from client: %s, n:%d, addrlen:%d\n", buffer, n, addrlen);
		

		sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&cliaddr, addrlen);

		keepGoing += 1;
	}
	
	return 0;
}
			
