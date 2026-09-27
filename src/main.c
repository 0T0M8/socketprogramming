#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

int main(void) {

 /* create IPv4 TCP socket */  
  int serverfd = socket(
    AF_INET,
    SOCK_STREAM,
    0
  );

  if (serverfd == -1) {
    perror("socket() failed!!");
    return 1;
  }
  printf("serverfd: %d\n", serverfd);
  printf("\nSocket created successfully!!\n");

  /* configure the server address */
  struct sockaddr_in serveraddr = {0};
   
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_port   = htons(8080);
  serveraddr.sin_addr.s_addr = INADDR_ANY;

  /* bind the socket to the address */
  if (bind(
       serverfd,
       (struct sockaddr *)&serveraddr,
       sizeof(serveraddr)
     ) == -1
  )
  { perror("bind failed()!!"); return 1; }

  printf("socket bound to port 8080\n");

  /* listen for incomming connections */
  if (listen(serverfd, 10) == -1) {
    perror("listen() failed!!");
    close(serverfd);
    return 1;
  }

  printf("server listening on port 8080\n");
  
  /* 
   * create a client address for incoming connections
     calculate it's size using sizeof()
     pass its inaddr as a pointer for the kernel
     accept incomming connections
     initialize an infinite loop to for multiple clients
   */

 while (1) 
 {
   struct sockaddr_in clientaddr = {0};
   socklen_t addrlen = sizeof(clientaddr);
   
   int clientfd = accept(
        serverfd,
        (struct sockaddr *)&clientaddr,
        &addrlen
   ); 

   if (clientfd == -1) { 
     perror("connect() failed!!"); 
     close(serverfd); 
     return 1; 
   }  

  printf("client connected!!\n");

/*
  printf("\nSocket created successfully!!\n");
  printf("serverfd: %d\n", serverfd);
  printf("socket bound to port 8080\n");
  printf("server listening on port 8080\n");
  printf("client connected!!\n"); 
 */

  /* create a buffer and read client requests */
  char buffer[4096];
  ssize_t bytesread;

  bytesread = read(
    clientfd,
    buffer,
    sizeof(buffer)-1
  );

  if (bytesread == -1) {
   perror("read() failed!!");
   close(clientfd);
  }
  /* null terminate the buffer */
  buffer[bytesread] = '\0';

  printf("\n__________HTTP REQUEST START__________\n");
  printf("%s", buffer);
  printf("__________HTTP REQUEST STOPS__________\n");

  /* extract http method and path */
 
  char method[16]; // GET POST
  char path[256];  // /   /static

  int fields = sscanf(
       buffer,
       "%15s %255s",
       method,
       path
  );
  if (fields != 2) {
   printf("Invalid HTTP request\n");
   close(clientfd);
   continue;
  }
  
  printf("\nPARSED HTTP REQUEST\n");
  printf("Method: %s\n", method);
  printf("Path: %s\n", path);

  printf("-----------------------------\n");

  const char *response;

  if (strcmp(path, "/") ==0) {
    response =
      "HTTP/1.1 200 OK\r\n"
      "Content-Type: text/plain\r\n"
      "Content-Length: 13\r\n"
      "\r\n"
      "Hello, World!";
  } else if (strcmp(path, "/hello") == 0) {
      response = 
        "HTTP/1.0 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: 18\r\n" 
        "\r\n"
        "Hello, from /hello";
    } else {
       /* code for 404 in milestone 11 */
        response = 
          "HTTP/1.0 200 OK\r\n"
          "Content-Type: text/plain\r\n"
          "Content-Length: 13\r\n"
          "\r\n"
          "Hello, world!";
      } //close else

  if (write(
       clientfd, 
       response, 
       strlen(response)
     ) ==-1
  ) 
  {
    perror("write() failed!!");
    close(clientfd);
    close(serverfd);
    return 1;
  }
  printf("HTTP Response Sent..\n");

  printf("Client disconnected...\n");
  printf("Waiting for another client...\n");

  close(clientfd);
} /* end of infinite loop */

  close(serverfd);

  return 0;
}
