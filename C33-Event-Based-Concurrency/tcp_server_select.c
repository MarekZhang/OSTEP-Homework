#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

#define PORT 8888
#define BUFFSIZE 1024

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
  struct fd_set main_set, temp_set;
  FD_ZERO(&main_set);
  FD_SET(sock, &main_set);
  time_t t;
  while(1) {
    //printf("server is running\n");
    temp_set = main_set;
  
    if(select(FD_SETSIZE, &temp_set, NULL, NULL, NULL) == -1) {
      perror("failed to select fd set.\n");
      exit(EXIT_FAILURE);
    }

    // loop for checking the update of file descriptor in the fd_set
    for (int i = 0; i < FD_SETSIZE; i++) {
      if (FD_ISSET(i, &temp_set)) {
        if(i == sock) {
          int conn;
          conn = accept(i, (struct sockaddr *) &client_name, &size);
          printf("accept a new connection from host: %s | port: %hd\n", inet_ntoa(client_name.sin_addr), ntohs(client_name.sin_port));
          if (conn < 0) {
            perror("failed to accept connection.\n");
            exit(EXIT_FAILURE);
          }
          //printf("the newly added file descriptor is %d\n", conn);
          FD_SET(conn, &main_set);
        } else {
          char buff[BUFFSIZE];
          memset(buff, 0, BUFFSIZE);
          if(recv(i, buff, BUFFSIZE, 0) == - 1) {
            perror("failed to recieve data from client.\n");
            exit(EXIT_FAILURE);
          }

          printf("recieved data from the client side: %s\n", buff);
          int data_len = (int)strlen(buff);
          
          memset(buff, 0, BUFFSIZE);
          sprintf(buff, "recieved number of characters: %d\n", data_len);
          
          if(send(i, buff, strlen(buff), 0) == -1) {
            perror("failed to response to the client\n");
            exit(EXIT_FAILURE);
          }
          close(i);
          FD_CLR(i, &main_set);
        }
      } 
      
    }
    // might print multiple times when there are more than one file descriptor is valid
    //fprintf(stderr, "Server: connect from host: %s, port: %hd.\n", inet_ntoa(client_name.sin_addr), ntohs(client_name.sin_port));
    //gettimeofday(&cur_time, NULL);
    //t = cur_time.tv_sec;
    //info = localtime(&t); 
    //printf("%s", asctime(info));
  }
}


