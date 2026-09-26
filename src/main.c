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
  
 
  printf("\nSocket created successfully!!\n");
  printf("serverfd: %d\n", serverfd);
  printf("socket bound to port 8080\n");

  close(serverfd);

  return 0;
}
