#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 8888

int main(int argc, char *argv[]) {
  int sock;
  //socket
  sock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
  if (sock < 0) {
    perror("falied to create socket.\n");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in server_name, client_name;
  size_t size;
  server_name.sin_family = AF_INET;
  server_name.sin_port = htons(PORT);
  server_name.sin_addr.s_addr = htonl (INADDR_ANY);
  // bind
  if (bind(sock, (struct sockaddr *) &server_name, sizeof(server_name)) < 0) {
    perror("failed to bind socket.\n");
    exit(EXIT_FAILURE);
  }

  //listen
  if (listen(sock, 1) < 0) {
    perror("server failed to listen.\n");
    exit(EXIT_FAILURE);
  }

  //request current time
  struct timeval cur_time;
  struct tm *info;
  time_t t;
  while(1) {
    printf("server is running\n");
    int conn;
    size = sizeof(client_name);
    conn = accept(sock, (struct sockaddr *) &client_name, &size);
    if (conn < 0) {
      perror("failed to accept connection.\n");
      exit(EXIT_FAILURE);
    }

    fprintf(stderr, "Server: connect from host: %s, port: %hd.\n", inet_ntoa(client_name.sin_addr), ntohs(client_name.sin_port));
    gettimeofday(&cur_time, NULL);
    t = cur_time.tv_sec;
    info = localtime(&t); 
    printf("%s", asctime(info));
  }
}


