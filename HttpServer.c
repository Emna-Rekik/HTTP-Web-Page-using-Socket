#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

//how many pending connections queue  will hold
#define BACKLOG 10  

#define PORT 8080
#define RESPONSE "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\nHello, World!\n"

void reaper(int sig){
   waitpid(-1, NULL, 0);
}

int main(){
   signal(SIGCHLD, reaper);
   //socket on which server process will listen for incoming communications
   int server_sockfd;
   //socket on which the server will be communicate with the client
   int client_sockfd; 
   int val;
   char buffer[1024] = {0};
   struct sockaddr_in server_addr;
   struct sockaddr_in client_addr;

   // Create passive socket for the server
   server_sockfd = socket (AF_INET, SOCK_STREAM, 0);

   // Set socket address
   server_addr.sin_family = AF_INET;
   server_addr.sin_port = htons(8080);
   inet_aton("127.0.0.1", &server_addr.sin_addr);
   
   memset(&(server_addr.sin_zero), '\0', sizeof server_addr.sin_zero);
   bind(server_sockfd, (struct sockaddr*)&server_addr, sizeof server_addr);

   // Create a connection queue and wait for clients
   listen(server_sockfd, BACKLOG); 
   while(1){
   
      fprintf(stderr,"\nServer waiting for client connection on port %d...", PORT);
      
      int client_len = sizeof client_addr;
      client_sockfd=accept(server_sockfd,(struct sockaddr*)&client_addr,&(client_len)); 
      
      switch(fork()){
          case 0: 
            fprintf(stderr, "\n********* CLIENT CONNECTION ESTABLISHED ********\n");
            close(server_sockfd);
	         char buf[100];
            
            // Read HTTP request from client
	    val = read(client_sockfd, buffer, 1024);
	    if (val < 0) {
	        printf("Error reading from socket");
	        return -1;
	    }
	    printf("%s\n", buffer);

	    // Send HTTP response to client
	    if (send(client_sockfd, RESPONSE, strlen(RESPONSE), 0) < 0) {
	        printf("\nError sending data over socket \n");
	        return -1;
	    }
	    printf("HTTP response sent\n");
            
            
            close(client_sockfd);
            fprintf(stderr, "\n********* CLIENT CONNECTION TERMINATED ********\n"); 
            fprintf(stderr,"\n\nServer waiting for client connection on port %d...", PORT);     
            exit(0);
            
          default:
            close(client_sockfd);
            break;
            
          case -1:
            fprintf(stderr,"Error in fork\n");
            exit(1);
      }
    }
   return 0;
 }
