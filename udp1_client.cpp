#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <iostream>


#define BUFFER_LENGTH 500

int main(int argc, char **argv)
{
	
	struct addrinfo hints;
	struct addrinfo *result, *rp;
	int sfd, s;
	int len;
	ssize_t nread;
	char buf[BUFFER_LENGTH];
	
	if (argc < 3){
		fprintf(stderr, "Usage: %s host port msg... \n", argv[0]);
		exit(EXIT_FAILURE);
	}
	/* Obtain address(es) matching host/port */
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
	hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */


		 /* getaddrinfo() returns a list of address structures.
              Try each address until we successfully connect(2).
              If socket(2) (or connect(2)) fails, we (close the socket
              and) try the next address. *
              */
	s = getaddrinfo(argv[1], argv[2], &hints, &result);
	
	if (s != 0){
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
		exit(EXIT_FAILURE);
	}


              
              

	
	/* 
	 * intentamos establecer conexion (opcional)
	 * */
	 /*
	for (rp = result; rp != NULL; rp = rp->ai_next) {
		sfd = socket(rp->ai_family, rp->ai_socktype,rp->ai_protocol);
		if (sfd == -1)
			continue;
		if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1)
			break;  
		close(sfd);
	}
	
	
	if (rp == NULL) {
		fprintf(stderr, "Could not connect\n");
		exit(EXIT_FAILURE);
	} */
		
	/* conexion directa*/
	sfd = socket(result->ai_family, result->ai_socktype, 0);
	
	len = strlen(argv[3])+1; /*+terminating null byte*/
			
	if (len+1 > BUFFER_LENGTH){
		fprintf(stderr, "fail\n");
		exit(EXIT_FAILURE);
	}
	
	
	if(sendto(sfd, argv[3], strlen(argv[3])+1, 0, result->ai_addr, result->ai_addrlen) != len){
		fprintf(stderr, "fail on send!\n");
		exit(EXIT_FAILURE);
	}
		
		
	nread = recvfrom(sfd, buf, BUFFER_LENGTH, 0, result->ai_addr, &result->ai_addrlen);
	
			
	if (nread == -1){
		perror("read");
		exit(EXIT_FAILURE);
	}
	buf[nread]='\0';
	std::cout << buf << std::endl;
			

	freeaddrinfo(result);
	return 0;
}
