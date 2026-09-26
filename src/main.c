#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

int main(void) {
  
  int serverfd = socket(
    AF_INET,
    SOCK_STREAM,
    0
  );

  if (serverfd == -1) {
    perror("socket() failed!!");
    return 1;
  }

  printf("\nSocket created successfully!!\n");
  printf("serverfd: %d\n", serverfd);

  close(serverfd);

  return 0;
}
