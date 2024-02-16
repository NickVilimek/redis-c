#include <iostream>
//#include <stdint.h>
//#include <stdlib.h>
//#include <string.h>
//#include <stdio.h>
//#include <errno.h>
#include <unistd.h>
//#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip.h>

static void msg(const char *msg) {
  fprintf(stderr, "%s\n", msg);
}

static void die(const char *msg) {
  int err = errno;
  fprintf(stderr, "[%d] %s\n", err, msg);
  abort();
}

static void do_something(int connfd) {
  char rbuf[64] = {};
  ssize_t n = read(connfd, rbuf, sizeof(rbuf) - 1);
  if(n < 0) {
    die("read() error");
    return;
  }

  printf("client says: %s\n", rbuf);

  char wbuf[] = "world";
  write(connfd, wbuf, strlen(wbuf));

}

int main() {
  std::cout << "INIT SOCKET W: " << AF_INET << " " << SOCK_STREAM << std::endl;
  int fd = socket(AF_INET, SOCK_STREAM, 0);

  if(fd < 0) {
    die("socket()");
  }

  int val = 1;
  setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

  struct sockaddr_in addr = {};
  addr.sin_family = AF_INET; //IPv4
  addr.sin_port = ntohs(1234);
  addr.sin_addr.s_addr = ntohl(INADDR_LOOPBACK);

  int rv = bind(fd, (const struct sockaddr *)&addr, sizeof(addr));
  std::cout << "RV  " << rv << std::endl;
  if(rv < 0) {
    die("bind()");
  }

  std::cout << "INIT SOCKET W: " << SOMAXCONN << std::endl;
  rv = listen(fd, SOMAXCONN);
  if (rv) {
      die("listen()");
  }

  while (true) {
    struct sockaddr_in client_addr = {};
    socklen_t socklen = sizeof(client_addr);
    int connfd = accept(fd, (struct sockaddr *)&client_addr, &socklen);
    if (connfd < 0) {
        continue;   // error
    }

    do_something(connfd);
    close(connfd);
  }

  return 0;
}