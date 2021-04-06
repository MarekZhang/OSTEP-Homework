# Chapter 33 - Event-Based Concurrency

Q1. First, write a simple server that can accept and serve TCP connections. You’ll have to poke around the Internet a bit if you don’t already know how to do this. Build this to serve exactly one request at a time; have each request be very simple, e.g., to get the current time of day.

- `fd = socket(domain, type, protocol);`
    - Communication domains: UNIX(AF_UNIX); IPv4(AF_INET); IPv6(AF_INET6);
    - socket types: Stream(`SOCK_STREAM`) ; Datagram sockets(`SOCK_DGRAM`)
- `int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);`
    - sockdf file descriptor obtained from a previous call to socket()

        ![Chapter%2033%20-%20Event-Based%20Concurrency%2003cf2022c9ef4a829df798d469542b14/Untitled.png](Chapter%2033%20-%20Event-Based%20Concurrency%2003cf2022c9ef4a829df798d469542b14/Untitled.png)

- `int listen(int sockfd, int backlog);`
- `int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);`
    - block until a connection arrives

![Chapter%2033%20-%20Event-Based%20Concurrency%2003cf2022c9ef4a829df798d469542b14/Untitled%201.png](Chapter%2033%20-%20Event-Based%20Concurrency%2003cf2022c9ef4a829df798d469542b14/Untitled%201.png)

Q1. First, write a simple server that can accept and serve TCP connections. You’ll have to poke around the Internet a bit if you don’t already know how to do this. Build this to serve exactly one request at a time; have each request be very simple, e.g., to get the current time of day.

```bash
$ make

$ ./tcp_server.out &
[1] 28747
server is running

$ ./tcp_client.out
Server: connect from host: 127.0.0.1, port: -15731.
Mon Apr  5 11:46:28 2021
server is running
```

Q3. Next, let’s make the requests a little more interesting, to mimic a simple web or file server. Each request should be to read the contents of a file (named in the request), and the server should respond by reading the file into a buffer, and then returning the contents to the client. Use the standard `open()`, `read()`, `close()` system calls to implement this feature. Be a little careful here: if you leave this running for a long time, someone may figure out how to use it to read all the files on your computer!

```bash
// Server side
$ ./tcp_server_select.out
accept a new connection from host: 127.0.0.1 | port: -10887
recieved data from the client side: foo
accept a new connection from host: 127.0.0.1 | port: -10886
recieved data from the client side: foobar
accept a new connection from host: 127.0.0.1 | port: -10885
recieved data from the client side: helloworld

// Client side
$ ./tcp_client_send.out 1 foo
From the server:  recieved number of characters: 3

$ ./tcp_client_send.out 1 foobar
From the server:  recieved number of characters: 6

$ ./tcp_client_send.out 1 helloworld
From the server:  recieved number of characters: 10
```

Q6.Finally, we have the hard part: how can you tell if the effort to build an asynchronous, event-based approach are worth it? Can you create an experiment to show the benefits? How much implementation complexity did your approach add?

```bash
//Async IO
$ ./tcp_client_open.out 4 hello & ./tcp_client_open.out 3 hello & ./tcp_client_open.out 2 hello & ./tcp_client_open.out 1 hello &

Server response to all request. | time elapsed: 0.000040s
accept a new connection from host: 127.0.0.1 | port: -7002
the server side is sending file content.
Server response to all request. | time elapsed: 0.000069s
accept a new connection from host: 127.0.0.1 | port: -7001
the server side is sending file content.
Server response to all request. | time elapsed: 0.000137s
accept a new connection from host: 127.0.0.1 | port: -7000
the server side is sending file content.
Server response to all request. | time elapsed: 0.000242s

//Sync IO
Server response to all request. | time elapsed: 0.000408
accept a new connection from host: 127.0.0.1 | port: -6994
openning file: hello
the server side is sending file content.
Server response to all request. | time elapsed: 0.000248
accept a new connection from host: 127.0.0.1 | port: -6993
openning file: hello
the server side is sending file content.
Server response to all request. | time elapsed: 0.000474
accept a new connection from host: 127.0.0.1 | port: -6992
openning file: hello
the server side is sending file content.
Server response to all request. | time elapsed: 0.000248
```

Async average:  0.000122 s

Sync average  :  0.000344 s