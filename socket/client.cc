#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>


#include <iostream>
#include <string.h>

#define PORT 1234
#define BUFFER_SIZE 1024

using namespace std;

// wait nearly x second;
static void WAIT(int seconds=0) {
  time_t old = time(NULL);
  while (time(NULL) < old+seconds) {}
}

int main() {
  const char *server_ip = "127.0.0.1";
  int client_fd;

  struct sockaddr_in server_addr;

  if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("socket error");
    exit(1);
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  inet_pton(AF_INET, server_ip, &server_addr.sin_addr);
  bzero(&(server_addr.sin_zero), 8);

  //
  if (connect(client_fd,
              (struct sockaddr *) &server_addr,
              sizeof(server_addr)) < 0) {
    perror("connect error");
    exit(1);
  }
  WAIT();
  //
  const char *hello = "hello server";
  write(client_fd, hello, strlen(hello));

  WAIT();

  close(client_fd);
}

//http://lengly.top/archives/155
//http://www.cnblogs.com/ggjucheng/archive/2012/01/17/2324584.html
