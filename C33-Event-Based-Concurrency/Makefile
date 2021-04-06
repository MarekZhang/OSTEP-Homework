CC = gcc
CFLAGS = -Wall -g

all: tcp_server.out tcp_client.out tcp_server_select.out tcp_client_send.out tcp_client_open.out tcp_server_async.out tcp_server_sync.out


%.out: %.c
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -rf ./*.out *.dSYM
	
