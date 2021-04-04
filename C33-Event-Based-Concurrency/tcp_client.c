#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <arpa/inet.h>

#define PORT 8888

int main(int argc, char *argv[]) {
  //if (argc != 2) {
  //  perror("usage: ./tcp_client <sleep-time>\n");
  //  exit(EXIT_FAILURE);
  //}
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
  //sleep(sleep_time);

  if (connect(fd, (struct sockaddr *) &server_addr, sizeof(struct sockaddr_in)) != 0) {
    perror("failed to connect to the server. \n");
    exit(EXIT_FAILURE);
  }

  return 0;

}
