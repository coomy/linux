/*
Author: Coomy
*/

#include "message_loop_thread.h"

namespace coomy {

// static void WAIT(int DURATION) {
//   time_t flag = time(NULL);
//   while (time(NULL) < flag+DURATION) {}
// }

MessageLoopThread::Task::Task(FUN_PTR fun, FUN_PRM param) :
    fun_(fun), param_(param) {}

MessageLoopThread::Task::~Task() {}

int MessageLoopThread::Task::Run() {
  // std::cout << "# -> a task run" << std::endl;
  fun_(param_);
  return 0;
}


MessageLoopThread::MessageLoopThread() {

}

MessageLoopThread::~MessageLoopThread() {

}

int MessageLoopThread::Start() {
  if (int ret = pthread_create(&thread_, NULL, MessageLoop, (void *)this)) {
    std::cout << "create pthread error : " << ret << std::endl;
    return -1;
  }

  std::cout << "MessageLoopThread started. " << std::endl;
  return 0;
}

int MessageLoopThread::PostTask(std::unique_ptr<Task> task) {
  tasks_.push(std::move(task));
  return 0;
}

PTHREAD MessageLoopThread::GetThread() {
  return thread_;
}

// static
void* MessageLoopThread::MessageLoop(void *p) {
  std::cout << "# MessageLoop running, waiting for task..." << std::endl;
  MessageLoopThread *that = (MessageLoopThread*)p;
  while (true) {
    if (that->HasTask()) {
      std::unique_ptr<Task> task = that->GetTask();
      // check task?
      task->Run();
    }
  }
}

bool MessageLoopThread::HasTask() {
  return !tasks_.empty();
}

std::unique_ptr<MessageLoopThread::Task> MessageLoopThread::GetTask() {
  std::unique_ptr<MessageLoopThread::Task> task = std::move(tasks_.front());
  //check sth.
  tasks_.pop();
  return std::move(task);
}

} // namespace linux
