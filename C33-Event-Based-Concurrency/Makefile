CC = gcc
CFLAGS = -Wall -g

all: tcp_server.out tcp_client.out tcp_server_select.out tcp_client_send.out


%.out: %.c
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -rf ./*.out
	
