CC = gcc
CFLAGS = -Wall -g

all: stat.out myls.out

%.out: %.c
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -rf ./*.out *.dSYM
