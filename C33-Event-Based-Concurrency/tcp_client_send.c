#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT 8888
#define BUFFSIZE 1024

int main(int argc, char *argv[]) {
  if (argc != 3) {
    perror("usage: ./tcp_client <sleep time> <data to send>\n");
    exit(EXIT_FAILURE);
  }
  int sleep_time = atoi(argv[1]);
  int fd;
  fd = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
  if (fd < 0) {
    perror("failed to create socket.\n");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); 

  if (connect(fd, (struct sockaddr *) &server_addr, sizeof(struct sockaddr_in)) != 0) {
    perror("failed to connect to the server. \n");
    exit(EXIT_FAILURE);
  }

  sleep(sleep_time);
  char buff[BUFFSIZE]; 
  memset(buff, 0, BUFFSIZE);
  strncpy(buff, argv[2], strlen(argv[2]));
  
  if(send(fd, buff, strlen(buff), 0) == -1) {
    perror("failed to send data to the server.\n");
    exit(EXIT_FAILURE);
  }

  memset(buff, 0, BUFFSIZE);
  if(recv(fd, buff, BUFFSIZE, 0) == -1) {
    perror("failed to recieve data from the server.\n");
    exit(EXIT_FAILURE);
  }
  printf("recieved data length %d\n", (int)strlen(buff));

  printf("receive from the server %s\n", buff);
  close(fd);
  
  return 0;

}
