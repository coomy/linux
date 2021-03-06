#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>


#include <iostream>
#include <string.h>

#define PORT 1234
#define BUFFER_SIZE 1024

using std::cout;
using std::endl;

// wait nearly x second;
static void WAIT(int seconds=0) {
  time_t old = time(NULL);
  while (time(NULL) < old+seconds) {}
}

int main()
{
  int server_fd, client_fd;
  int sin_size;

  struct sockaddr_in server_addr;
  struct sockaddr_in client_addr;

  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("socket error");
    exit(1);
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr.s_addr = INADDR_ANY;
  bzero(&(server_addr.sin_zero), 8);

  // socket close immediately without wait [l_linger].
  // struct linger so_linger;
  // so_linger.l_onoff = 1;
  // so_linger.l_linger = 0;
  // setsockopt(server_fd, SOL_SOCKET, SO_LINGER, &so_linger, sizeof(so_linger));

  // 设置超时时间为 60 秒
  struct timeval tv;
  tv.tv_sec = 60;
  tv.tv_usec = 0;
  setsockopt(server_fd, SOL_SOCKET, SO_RCVTIMEO,
             &tv, sizeof(tv));

  if (bind(server_fd, (struct sockaddr*)&server_addr,
           sizeof(server_addr)) < 0) {
    perror("bind error");
    exit(1);
  }

  if (listen(server_fd, 10) == -1) {
    perror("listen error");
    exit(1);
  }

  while (true) {
    cout << "wait connect..." << endl;
    sin_size = sizeof(struct sockaddr_in);
    if ((client_fd = accept(server_fd,
                            (struct sockaddr*)&client_addr,
                            (socklen_t*)&sin_size))<0) {
      perror("accept error");
      // continue;
      return 0;
    }

    cout << "wait data..." << endl;
    printf("received a connection from %s:%u\n",
           inet_ntoa(client_addr.sin_addr),
           ntohs(client_addr.sin_port));

    char buffer[BUFFER_SIZE] = {0};
    int ret = read(client_fd, buffer, BUFFER_SIZE);
    if (ret == EWOULDBLOCK) {
      perror("read timeout for 5s.");
      exit(1);
    }

    cout << "data: " << buffer << endl << endl;
    WAIT();

    close(client_fd);
  }

}

//http://lengly.top/archives/155
//http://www.cnblogs.com/ggjucheng/archive/2012/01/17/2324584.html
