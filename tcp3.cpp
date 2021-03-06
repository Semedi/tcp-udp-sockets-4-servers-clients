#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>

#include <iostream>

#define BUFFER_LENGTH 128


static void sigchld_hdl (int sig)
{
	/* Wait for all dead processes.
	 * We use a non-blocking call to be sure this signal handler will not
	 * block if a child was cleaned up in another part of the program. */
	while (waitpid(-1, NULL, WNOHANG) > 0) {
	}
}
 
/* para el ejercicio 4 usar la llamada al sistema wait*/

/*
struct sigaction act;
memset (&act, 0, sizeof(act));
act.sa_handler = sigchld_hdl;
if (sigaction(SIGCHLD, &act, 0))
	perror ("sigaction");
	return 1;
*/

int main(int argc, char **argv)
{
	
	struct addrinfo hints;
    struct addrinfo *result, *rp;
    int sfd, s, demon, childs = 0;
    struct sockaddr_storage peer_addr;
    socklen_t peer_addr_len;
    ssize_t nread;
    char host[NI_MAXHOST];
    char service[NI_MAXSERV];
    char buf[BUFFER_LENGTH];
    
   
    
    if (argc != 2) {
        fprintf(stderr, "Usage: %s port\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_protocol = 0;          /* Any protocol */
	
	
	s = getaddrinfo("::1", argv[1], &hints, &result);
	/* Poner Null para que vaya el nc
	 * pero al ponerlo no resuelve direcciones ipv6 = wtf
	 * */
    if (s != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
    }
    /*
    sfd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    bind(sfd, result->ai_addr, result->ai_addrlen);
    */
    for (rp = result; rp != NULL; rp = rp->ai_next) {
       sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sfd == -1)
            continue;

       if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
            break;                  

       close(sfd);
    }
    
    if (rp == NULL) {               
        fprintf(stderr, "Could not bind\n");
        exit(EXIT_FAILURE);
    }
  

   freeaddrinfo(result);           /* No longer needed */
   
   
   if(listen(sfd, 2) != 0){
		printf("listen()");
		close(sfd);
		exit(EXIT_FAILURE);
	}
	
	
	
	
	demon = getpid();
	while(demon){
			
			peer_addr_len = sizeof(struct sockaddr_storage);
			int success = accept(sfd, (struct sockaddr*)&peer_addr, &peer_addr_len);
			
			s = getnameinfo((struct sockaddr *) &peer_addr, peer_addr_len, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICSERV);
			
			if (s== 0)
				printf("Conectado %s:%s \n", host, service);
				
			/* Se han conectado */
			
			childs++;
			demon = fork(); //clone
			
			
			if (demon == 0){ /* Esto solo lo ejecutara eñ hijo*/
				printf ("%i proceso(s) activos\n", childs);
			
			
				while (true) {
				
					nread = recv(success, buf, BUFFER_LENGTH, 0);
					if (nread == 0){
						printf("conexion cerrada \n");
						break; 
					}
					if (nread == -1){
						printf("me cago en dioos\n");
						break;
						
					}
					
					buf[nread] = '\0';
					
					if (send(success,buf, ++nread, 0) == -1){
						printf("error en la conexion(envio), intentandolo otra vez...\n");
						break;		
					}
				
				}
			}
			close(success);
		
		
	}
	close(sfd);
	
	
	return 0;
}
