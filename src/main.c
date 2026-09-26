#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

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
    return 1;
    close(serverfd);
  }

  printf("server listening on port 8080\n");
  
  /* 
   * create a client address for incoming connections
     calculate it's size using sizeof()
     pass its inaddr as a pointer for the kernel
     accept incomming connections
   */
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
  close(clientfd);
  close(serverfd);

  return 0;
}
