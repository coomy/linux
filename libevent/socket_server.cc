#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
// libevent头文件
#include <event.h>

using namespace std;


#define PORT 1234
#define BUFFER_SIZE 1024

void on_data(evutil_socket_t listener, short event, void *arg) {
  char buffer[BUFFER_SIZE] = {0};
  int ret = read(listener, buffer, BUFFER_SIZE);
  if (ret == EWOULDBLOCK) {
      perror("read EWOULDBLOCK");
      exit(1);
  }

  cout << "data: " << buffer << endl;

  close(listener);
}

// 定时事件回调函数
void on_accept(evutil_socket_t listener, short event, void *arg) {
  evutil_socket_t client_fd;
  struct event_base *base = (struct event_base*)arg;
  struct sockaddr_in client_addr;

  int sin_size = sizeof(client_addr);

  client_fd = accept(listener, 
                     (struct sockaddr*)&client_addr, 
                     (socklen_t*)&sin_size);
  if (client_fd < 0) {
    perror("accept error");
    return;
  }

  // evutil_make_listen_socket_reuseable(client_fd);
  // evutil_make_socket_nonblocking(client_fd);

  struct event *event_read = event_new(base, client_fd, EV_READ|EV_PERSIST,
                                       on_data, NULL);
  event_base_set(base, event_read);
  event_add(event_read, NULL);

  printf("received a connection from %s:%u\n",
           inet_ntoa(client_addr.sin_addr),
           ntohs(client_addr.sin_port));

}

int main()
{
  int server_fd;

  struct sockaddr_in server_addr;

  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0))==-1) {
    perror("socket error");
    exit(1);
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr.s_addr = INADDR_ANY;
  bzero(&(server_addr.sin_zero), 8);

  evutil_make_listen_socket_reuseable(server_fd);
  evutil_make_socket_nonblocking(server_fd);

  if (bind(server_fd, (struct sockaddr*)&server_addr,
           sizeof(server_addr)) < 0) {
    perror("bind error");
    exit(1);
  }

  if (listen(server_fd, 10) == -1) {
    perror("listen error");
    exit(1);
  }

  struct event_base *base = event_base_new();
  struct event *listen_event = event_new(base, server_fd, EV_READ|EV_PERSIST,
                                         on_accept, (void*)base);
  event_base_set(base, listen_event);
  event_add(listen_event, NULL);
  event_base_dispatch(base);

  cout << "FINISH" << endl;

  return 0;
}