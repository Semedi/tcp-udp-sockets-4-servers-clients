#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <iostream>

#define BUF_LENGTH 256


int main(int argc, char **argv)
{
	struct addrinfo hints;
	struct addrinfo *result, *rp;
	int sfd, s;
	int len;
	ssize_t nwrite, nread;
	char message[BUF_LENGTH];
	char buf[BUF_LENGTH];
	
	if (argc < 2) {
		fprintf(stderr, "Usage: %s host port...\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	
	memset(&hints, 0, sizeof(struct addrinfo));
	
	hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
	hints.ai_socktype = SOCK_STREAM; /* Datagram socket */
	hints.ai_flags = AI_PASSIVE;
	
	s = getaddrinfo(argv[1], argv[2], &hints, &result);
	if (s != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
		exit(EXIT_FAILURE);
	}

	for (rp = result; rp != NULL; rp = rp->ai_next) {
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sfd == -1)
            continue;

        if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1)
            break;                  /* Success */

		close(sfd);
    }
    
    
    if (rp == NULL) {               /* No address succeeded */
        fprintf(stderr, "Could not connect\n");
        exit(EXIT_FAILURE);
    }

	freeaddrinfo(result);           /* No longer needed */

	len = read(0, message, BUF_LENGTH);
	
	if (len == -1){
		perror("error read()\n");
		close(sfd);
		exit(EXIT_FAILURE);
	}
	
	while (message[0] != 'q'){
		
		message[++len]='\0';
		nwrite = send(sfd, message, len, 0);
		
		if (nwrite != len)
			printf("error al enviar mensaje\n");
			
			
		nread =recv(sfd, buf, BUF_LENGTH, 0);
		buf[nread]='\0';
		if (nread == -1)
			perror("read");
			
		
		std::cout << buf << std::endl;
		len = read(0, message, BUF_LENGTH);
			if (len == -1){
				perror("error read()\n");
				close(sfd);
			exit(EXIT_FAILURE);
			}
		
		
	}
	
	close(sfd);



	
	return 0;
	
}
