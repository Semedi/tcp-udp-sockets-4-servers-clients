#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>

#include <stdio.h>
#include <iostream>

int main(int argc, char **argv){
	
	struct addrinfo hints;
	struct addrinfo *result, *rp;
	memset(&hints, 0, sizeof(struct addrinfo));
	char host[NI_MAXHOST];
	int s;
	std::string family;
	std::string socktype;
	
	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_UNSPEC;
	
	s = getaddrinfo(argv[1], NULL, &hints, &result);
	if(s != 0){
		printf("Error %s\n", gai_strerror(s));
		exit(EXIT_FAILURE);
	}
	
	for(rp =result; rp != NULL; rp = rp->ai_next){
	
		if(getnameinfo(rp->ai_addr, rp->ai_addrlen, host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST) == 0){
			
		
			
			if (rp->ai_family == 10)
				family="AF_INET6";
			else
				family="AF_INET";
				
			if (rp->ai_socktype == 0)
				socktype="SOCK_STREAM";
			else if (rp->ai_socktype==1)
				socktype="SOCK_DGRAM";
			else
				socktype="SOCK_RAW";
				
			
			std::cout << host<<" "<<family<< " "<< socktype<<std::endl;
			
			
		}
		
	}
	
	
	freeaddrinfo(result);

	return 0;

}
