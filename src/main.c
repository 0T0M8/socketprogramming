#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
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
   continue;
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

  /*
   * smart routing using snprintf
   * it prints to the response string
   * instead of the terminal like printf
   */
  const char *status;
  const char *body = NULL;
  const char *content_type = "text/plain";

  char file_buffer[4096];
  ssize_t body_length;

  int filefd = -1;

  if (strcmp(path, "/") == 0) {
    status = "200 OK";
    body = "Hello, World!\n";
    body_length = strlen(body); 
  }
  else if (strcmp(path, "/hello") == 0) {
    status = "200 OK";
    body = "Hello, from /hello\n";
    body_length = strlen(body);
  }
  
  else if (strcmp(path, "/index.html") == 0) {
    filefd = open("public/index.html", O_RDONLY);

      if (filefd == -1) {
        perror("file open() failed!!");
        status = "404 Not Found\n";
        body = "Not Found\n";
        body_length = strlen(body);
      } 
      else {
        /* read file into memory */
        body_length = read(
          filefd,
          file_buffer,
          sizeof(file_buffer)
        );

         if (body_length == -1) {
           perror("read() file failed!!");
           close(filefd); /******************/
           filefd = -1;
           status = "Internal Server Error";
           body = "Internal Server Error\n";
           body_length = strlen(body);
         }
         else {
           status = "200 OK";
           body = file_buffer;
           content_type = "text/html";
         } /*×××××××××××××××××××××××××*/
      } // else
  } //else if "/index.html"
  else {
    status = "404 Not Found";
    body = "Not Found\n";
    body_length = strlen(body);
  } 

  char response[4096];
   
  int response_length = snprintf(
    response,
    sizeof(response),
    "HTTP/1.1 %s\r\n"
    "Content-Type: %s\r\n"
    "Content-Length: %zd\r\n"
    "Connection: close\r\n"
    "\r\n"
    "%s",
    status,
    content_type,
    body_length,
    body
  );
  
  /* send http headers */
  if (write(
       clientfd, 
       response, 
       response_length
     ) ==-1
  ) 
  {
    perror("write() headers failed!!");
    if (filefd != -1) {
      close(filefd);
    }

    close(clientfd);
    continue;
  }

  /* send http body */
  if (write(
       clientfd,
       body,
       body_length
     ) == -1 )
  {
    perror("write() body failed!!");
    if (filefd != -1) {
      close(filefd);
    }
  
    close(clientfd);
    continue;
  }

  /* close the file if one was open */
  if (filefd != -1) {
   close(filefd);
  }

  /* send http body */
  

  printf("HTTP Response Sent..\n");

  printf("Client disconnected...\n");
  printf("Waiting for another client...\n");

  close(clientfd);
} /* end of infinite loop */

  close(serverfd);

  printf("\n");
  return 0;
}
