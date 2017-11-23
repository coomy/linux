/*
Author: coomy
*/

#include <memory>
#include <unistd.h>
#include <time.h>
#include "message_loop_thread.h"

using namespace coomy;

// wait nearly 1 second;
static void WAIT() {
  time_t old = time(NULL);
  while (time(NULL) == old) {}
}

void task_fun(void *p) {
  (*(int*)p)++;
}

int main(int argc, char *argv[]) {
  MessageLoopThread *thread = new MessageLoopThread();
  thread->Start();

  int ret = 0;

  std::cout << "Main thread before ret:" << ret << std::endl;
  std::unique_ptr<MessageLoopThread::Task> task(
      new MessageLoopThread::Task(task_fun, (void*)&ret));

  thread->PostTask(std::move(task));

  WAIT();
  std::cout << "Main thread after ret:" << ret << std::endl;

  std::cout << "Main thread still running, wait sub thread exit..." << std::endl;
  pthread_join(thread->GetThread(), NULL);

  return 0;
}
