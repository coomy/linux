#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <iostream>
#include <string>

#define PORT 1234
#define BUFFER_SIZE 1024

using namespace std;

int main() {
	int server_fd, client_fd;
	int sin_size;

	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;

	if (server_fd = socket(AF_INET, SOCKET_STREAM, 0)) == -1 {
    perror("socket error");
    exit(1);
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr.s_addr = INADDR_ANY;

  bzero(&(server_addr.sin_zero), 8);

  if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
    perror("bind error");
    exit(1);
  }

  if (listen(server_fd, 10) == -1) {
    perror("listen error");
    exit(1);
  }

  while (true) {
    sin_size = sizeof(struct sockaddr_in);
    if ((client_fd = accept(server_fd, 
                            (struct sockaddr*)&client_addr, 
                            (socklen_t*)&sin_size))) {
      perror("accept error");
      continue;
    }

    /////
  }

}

//http://lengly.top/archives/155
//http://www.cnblogs.com/ggjucheng/archive/2012/01/17/2324584.html