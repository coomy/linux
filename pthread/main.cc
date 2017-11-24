/*
Author: Coomy
*/

#include <memory>
#include <unistd.h>
#include <time.h>
#include "message_loop_thread.h"

using namespace coomy;

//////////////////////////////////////////
// wait nearly 1 second;
static void WAIT() {
  time_t old = time(NULL);
  while (time(NULL) == old) {}
}

template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
//////////////////////////////////////////

void task_fun_add(void *p) {
  (*(int*)p)++;
  std::cout << " -> add: " << *(int*)p << std::endl;
}

void task_fun_minus(void *p) {
  (*(int*)p)--;
  std::cout << " -> munus: " << *(int*)p << std::endl;
}

int main(int argc, char *argv[]) {
  MessageLoopThread *thread_add = new MessageLoopThread();
  thread_add->Start();

  MessageLoopThread *thread_minus = new MessageLoopThread();
  thread_minus->Start();

  int ret = 0;

  WAIT();

  std::cout << "Main thread before ret:" << ret << std::endl;

  for (int i=0; i<100; i++) {
    
    thread_add->PostTask(
        make_unique<MessageLoopThread::Task>(task_fun_add, (void*)&ret));

    thread_minus->PostTask(
        make_unique<MessageLoopThread::Task>(task_fun_minus, (void*)&ret));

    // std::unique_ptr<MessageLoopThread::Task> task_minus(
    //     new MessageLoopThread::Task(task_fun_minus, (void*)&ret));

    // thread->PostTask(std::move(task_minus));
  }

  WAIT();
  std::cout << "Main thread after ret:" << ret << std::endl;

  std::cout << "Main thread still running, wait sub thread exit..." << std::endl;
  pthread_join(thread_add->GetThread(), NULL);
  pthread_join(thread_minus->GetThread(), NULL);

  return 0;
}
