#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <aio.h>
#include <errno.h>

#define PORT 8888
#define BUFFSIZE 1024
#define ONEMILLION 1000000
#define MAXREQUEST FD_SETSIZE

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
  struct aiocb *aio_request_arr; 
  aio_request_arr = malloc(MAXREQUEST * sizeof(struct aiocb));
  if (aio_request_arr == NULL) {
    perror("failed to allocate memory for aiocb array.\n");
    exit(EXIT_FAILURE);
  }
  struct fd_set main_set, temp_set;
  FD_ZERO(&main_set);
  FD_SET(sock, &main_set);
  struct timeval start_time, end_time;
  int remain_request, total_request;
  remain_request = total_request = 0;
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
          conn = accept(i, (struct sockaddr *) &client_name, (socklen_t *)&size);
          printf("accept a new connection from host: %s | port: %hd\n", inet_ntoa(client_name.sin_addr), ntohs(client_name.sin_port));
          if (conn < 0) {
            perror("failed to accept connection.\n");
            exit(EXIT_FAILURE);
          }
          //printf("the newly added file descriptor is %d\n", conn);
          FD_SET(conn, &main_set);

          if(remain_request == 0) {
            gettimeofday(&start_time, NULL);
          }
          total_request++;
          remain_request++;
        } else {
          char buff[BUFFSIZE];
          memset(buff, 0, BUFFSIZE);

          if(recv(i, buff, BUFFSIZE, 0) == - 1) {
            perror("failed to recieve data from client.\n");
            exit(EXIT_FAILURE);
          }

          printf("openning file: %s\n", buff);

          int fp;
          if((fp = open(buff, O_RDONLY)) < 0) {
            perror("failed to open file");
            exit(EXIT_FAILURE);
          }
          
          aio_request_arr[i].aio_fildes = fp;
          aio_request_arr[i].aio_offset = 0;
          aio_request_arr[i].aio_buf = buff;
          aio_request_arr[i].aio_nbytes = BUFFSIZE;
          memset(buff, 0, BUFFSIZE);

          if(aio_read(&aio_request_arr[i]) < 0) {
            perror("failed to open file");
            exit(EXIT_FAILURE);
          }

        }
      } 
    }

    // the aio_error regularly check interrupt
    int i;
    for (i = 0; i < MAXREQUEST; i++) {
      struct aiocb * aio_request = &aio_request_arr[i];
      const char * buff = (const char*)aio_request->aio_buf;
      if (buff != NULL) {
        int read_status;  
        read_status = aio_error(&aio_request_arr[i]);
        if (read_status == 0) { // read completed
          printf("the server side is sending file content.\n");
          if(send(i, buff, strlen(buff), 0) == -1) {
            perror("failed to response to the client\n");
            exit(EXIT_FAILURE);
          }
          close(i);
          close(aio_request->aio_fildes);
          FD_CLR(i, &main_set);
          remain_request--;
        }else if (read_status == EINPROGRESS) {
         i = 0;
        }
     }
    }
    // might print multiple times when there are more than one file descriptor is valid
    //fprintf(stderr, "Server: connect from host: %s, port: %hd.\n", inet_ntoa(client_name.sin_addr), ntohs(client_name.sin_port));
    //gettimeofday(&cur_time, NULL);
    //t = cur_time.tv_sec;
    //info = localtime(&t); 
    //printf("%s", asctime(info));
    if (remain_request == 0) {
      gettimeofday(&end_time, NULL);
      printf("Server response to all request. | time elapsed: %.6fs\n", end_time.tv_sec - start_time.tv_sec + (end_time.tv_usec - start_time.tv_usec) / (float)ONEMILLION);
    }
  }

  return 0;
}


