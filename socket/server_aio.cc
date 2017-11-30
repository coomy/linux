#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <aio.h>
#include <string.h>

#include <iostream>

#define PORT 1234
#define BUFFER_SIZE 1024

using std::cout;
using std::endl;

void aio_read_complete_handler(sigval_t sigval);

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

    struct aiocb *cbp = new struct aiocb;

    bzero((char*)cbp, sizeof(struct aiocb));

    cbp->aio_buf = (volatile void*)malloc(BUFFER_SIZE);
    cbp->aio_nbytes = BUFFER_SIZE;
    cbp->aio_offset = 0;
    cbp->aio_fildes = client_fd;

    //
    cbp->aio_sigevent.sigev_notify = SIGEV_THREAD;
    cbp->aio_sigevent.sigev_value.sival_ptr = (void*)cbp;
    cbp->aio_sigevent.sigev_notify_function = aio_read_complete_handler;

    int ret = aio_read(cbp);
    if (ret < 0) {
      perror("aio read error\n");
      exit(1);
    }
  }
}

void aio_read_complete_handler(sigval_t sigval) {
  struct aiocb *req;
 
  req = (struct aiocb *)sigval.sival_ptr;
 
  /* Did the request complete? */
  if (aio_error(req) == 0) {
 
    if (int ret = aio_return(req) > 0) {
      char *buffer = (char*)req->aio_buf;
      cout << "ret:" << ret << " data:" << buffer << endl;
    } else {
      cout << "ret:" << ret << endl;
      /* read failed, consult errno */
    }
 
  }

  close(req->aio_fildes);
  delete req;
 
  return;
}

