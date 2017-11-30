/**
 * 此文件编译不通过
 * 主要做代码记录，不做运行
*/

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>


#include <iostream>
#include <string.h>

#define PORT 1234
#define BUFFER_SIZE 1024

using std::cout;
using std::endl;


/** Returns true on success, or false if there was an error */
bool SetSocketBlockingEnabled(int fd, bool blocking=false)
{
   if (fd < 0) return false;

#ifdef _WIN32
   unsigned long mode = blocking ? 0 : 1;
   return (ioctlsocket(fd, FIONBIO, &mode) == 0) ? true : false;
#else
   int flags = fcntl(fd, F_GETFL, 0);
   if (flags < 0) return false;
   flags = blocking ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK);
   return (fcntl(fd, F_SETFL, flags) == 0) ? true : false;
#endif
}

/**
 * Non-block is still Sync I/O, no matter select or epoll, all those are Sync.
 * You should check status by block the process, by [select] or [epoll_wait].
 */

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

  // set time out to tv_sec seconds.
  struct timeval tv;
  tv.tv_sec = 1;
  tv.tv_usec = 0;
  setsockopt(server_fd, SOL_SOCKET, SO_RCVTIMEO,
             &tv, sizeof(tv));

  // bind
  if (bind(server_fd, (struct sockaddr*)&server_addr,
           sizeof(server_addr)) < 0) {
    perror("bind error");
    exit(1);
  }

  // listen
  if (listen(server_fd, 10) == -1) {
    perror("listen error");
    exit(1);
  }

  // set non-block
  if (!SetSocketBlockingEnabled(server_fd, true)) {
    perror("make socket non blocking error.");
    exit(1);
  }

  // epoll create and init.
  static const int MAX_EVENTS = 10;
  struct epoll_event event, events[MAX_EVENTS];
  int event_fd;
  if (epoll_create1(0)<0) {
    perror("epoll create error");
    exit(1);
  }

  event.data.fd = server_fd;
  event.events = EPOLLIN | EPOLLET;
  if (epoll_ctl(event_fd, EPOLL_CTL_ADD, server_fd, &event)<0) {
    perror("epoll ctl error.");
    exit(1);
  }

  events = calloc(MAX_EVENTS, sizeof(event));

  while (true) {
    // peek the fd for event
    int size_event = epoll_wait(event_fd, events, MAX_EVENTS, -1);
    for (int i=0; i<size_event; i++) {
      if (events[i].evnets & (EPOLLERR|EPOLLHUP)) {
        cout << "epoll error" << endl;
        close(events[i].data.fd);
        continue;
      }
      // data in
      if (events[i].events & EPOLLIN) {
        if (server_fd == events[i].data.fd) { // connect
          client_fd = accept(server_fd,
                  (struct sockaddr *) &client_addr, &sin_size);

          printf("received a connection from %s:%u\n",
                  inet_ntoa(client_addr.sin_addr),
                  ntohs(client_addr.sin_port));

          if (client_fd < 0) {
              perror("accept error");
              exit(EXIT_FAILURE);
          }
          SetSocketBlockingEnabled(client_fd);
          ev.events = EPOLLIN | EPOLLET;
          ev.data.fd = client_fd;
          if (epoll_ctl(event_fd, EPOLL_CTL_ADD, client_fd,
                      &ev) == -1) {
              perror("epoll_ctl: client_fd");
              exit(EXIT_FAILURE);
          }
        }
        else { // data read.
          char buffer[BUFFER_SIZE] = {0};
          int ret = read(events[i].data.fd, buffer, BUFFER_SIZE);
          if (ret == EWOULDBLOCK) {
            perror("read timeout for 5s.");
            exit(1);
          }
          close(events[i].data.fd);
          cout << "data: " << buffer << endl << endl;
        }
      }
      // else if OUT for write.
    }
  }
}

//http://lengly.top/archives/155
//http://www.cnblogs.com/ggjucheng/archive/2012/01/17/2324584.html

//http://blog.lucode.net/linux/epoll-tutorial.html
//https://www.felix021.com/blog/read.php?1879
//http://www.cnblogs.com/zhanggaofeng/p/5895166.html