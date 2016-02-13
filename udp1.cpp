#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>
#include <string.h>
#include <iostream>
#include <time.h>

/*
 * direccion y puerto son el primer y segundo argumento del programa.
 * 
 * en recvfrom sockaddr_Storage para ipv4 y 6 * */
int main(int argc, char **argv){
	
	struct addrinfo hint;
	struct addrinfo *result;
	struct sockaddr_storage peer_addr;
	char host[NI_MAXHOST];
	char service[NI_MAXSERV];
	int sfd, s;
	char message[1];
	ssize_t nread;
	socklen_t peer_addr_len;

	
	if (argc != 3) {
		fprintf(stderr, "Usage: %s ip port\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	
	memset(&hint, 0, sizeof(struct addrinfo)); /* flush*/
	
	/*criterios busqueda*/
	hint.ai_flags = AI_PASSIVE;
	hint.ai_socktype = SOCK_DGRAM;
	
	
	
	/* getaddrinfo() returns a list of address structures.
              Try each address until we successfully bind(2).
              If socket(2) (or bind(2)) fails, we (close the socket
              and) try the next address. */
	s = getaddrinfo(argv[1], argv[2], &hint, &result);
	if (s != 0){
		printf("Error en getaddinfo: %s\n", gai_strerror(s));
		exit(EXIT_FAILURE);
	}
	
	/* conexion */
	sfd = socket(result->ai_family, result->ai_socktype, 0);
	if (sfd == -1){
		std::cout << "socket()" << std::endl;
		exit(EXIT_FAILURE);
	}
	
	/* binding my addr*/
	if (bind(sfd, result->ai_addr, result->ai_addrlen) != 0){
		std::cout <<"bind()" << std::endl;
		exit(EXIT_FAILURE);
	}
	
	while(message[0] != 'q'){/* servicio*/
		peer_addr_len = sizeof(struct sockaddr_storage);
		nread = recvfrom(sfd, message, 1, 0, (struct sockaddr*) &peer_addr, &peer_addr_len);
		
		if (nread == -1)
			continue;
		
		s = getnameinfo((struct sockaddr *) &peer_addr, peer_addr_len, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICSERV | NI_NUMERICHOST);
		if (s == 0)
			printf("Received %zd bytes from %s:%s\n", nread, host, service);
		 else
			fprintf(stderr, "getnameinfo: %s\n", gai_strerror(s));
			
		if (message[0] == 't'){
			time_t t=time(NULL);
			struct tm *tlocal=localtime(&t);
			char output_time[128];
			strftime(output_time,128,"%d/%m/%y",tlocal);
			
			if (sendto(sfd, output_time, strlen(output_time)+1, 0, (struct sockaddr *) &peer_addr, peer_addr_len) == -1)
				fprintf(stderr, "Error sending response\n");
			
		}
		else if(message[0] == 'd'){
			
			time_t t=time(NULL);
			struct tm *tlocal=localtime(&t);
			char output_time[128];
			strftime(output_time,128,"%r",tlocal);
			
			if (sendto(sfd, output_time, strlen(output_time)+1, 0, (struct sockaddr *) &peer_addr, peer_addr_len) == -1)
				fprintf(stderr, "Error sending response\n");
			
		}
		else if(message[0] == 'q')
			std::cout << "saliendo" << std::endl;
		else
			std::cout << "comando no soportado " << message[0] << std::endl;
		


	}
	
	freeaddrinfo(result);
	
}
