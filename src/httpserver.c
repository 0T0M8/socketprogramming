#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

const int PORT = 8080;

int main(void) {
 /* create a socket */
 int sockfd = socket(AF_INET, SOCK_STREAM, 0); 
 
/* declare the sockaddr_in struct */
 struct sockaddr_in address;

 /* populate the sockaddr_in struct */
 address.sin_family = AF_INET; 
 address.sin_port = htons(PORT);
 address.sin_addr.s_addr = INADDR_ANY;

 /* bind the address to the socket */
 bind(sockfd, (struct sockaddr_in *)&address, sizeof(address));

 /* listen on the port assigned */
 listen(sockfd, 10);

 /* welcome the clients to server!! */
 printf("Server is running on port 8080\n");

 /* accept the client connections */
 int clientfd = accept(sockfd, NULL, NULL);

 /* Read a request from client */
 char buf[4096];

 read(clientfd, buf, sizeof(buf)-1);

 /* create a response */
 const char* response = 
     "HTTP/1.1 200 OK\r\n"
     "Content-Type: text/html\r\n"
     "\r\n"
     "<h1>Hello, from C Server!!</h1>";

 /* Send the response to client */
 write(clientfd, response, strlen(response));

 /* close the client socket */
 close(clientfd);

 /* close the server socket */
 close(sockfd);

 return 0;

}
