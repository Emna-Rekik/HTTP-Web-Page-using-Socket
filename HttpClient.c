#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>
#include <string.h>

#define PORT 8080
#define REQUEST "GET / HTTP/1.1\r\nHost: 127.0.0.1\r\n\r\n"


int main(int argc, char** argv){

   char buffer[1024] = {0};
   int sockfd;
   int val;
   
   if(argc!=3)
   {
   	printf("Usage: <IP_ADRESS> <PORT>\n"); 
   	exit(1);
   }
   
   
   // Create a socket
   if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
   
   // Set socket address
   struct sockaddr_in dest_addr;
   dest_addr.sin_family = AF_INET;    
   dest_addr.sin_port = htons(atoi(argv[2]));
   inet_aton(argv[1], &dest_addr.sin_addr);
   memset(&(dest_addr.sin_zero), '\0', sizeof dest_addr.sin_zero); 
   
   // Connect this socket to the server's socket
   if (connect(sockfd, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) < 0) {
       printf("\nConnection Failed \n");
       return -1;
   }

   // Send HTTP request to server
   if (send(sockfd, REQUEST, strlen(REQUEST), 0) < 0) {
       printf("\nError sending data over socket \n");
       return -1;
   }
   printf("HTTP request sent\n");

   // Read HTTP response from server
   val = read(sockfd, buffer, 1024);
   if (val < 0) {
       printf("Error reading from socket");
       return -1;
   }
   printf("%s\n", buffer);

   // Close socket
   close(sockfd);
   
   exit(0);
}
