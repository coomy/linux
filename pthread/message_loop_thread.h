
#include <pthread.h>
#include <iostream>
#include <memory>
#include <queue>

namespace coomy {

typedef void (*FUN_PTR)(void *);
typedef void* FUN_PRM;

typedef pthread_t PTHREAD;
typedef int THEAD_ID;

class MessageLoopThread {
public:
  class Task {
  public:
    Task(FUN_PTR fun, FUN_PRM param);
    ~Task();

    int Run();

  private:
    FUN_PTR fun_;
    FUN_PRM param_;
  };

  MessageLoopThread();
  ~MessageLoopThread();

  // should call before PostTask
  int Start();

  int PostTask(std::unique_ptr<Task> task);

  PTHREAD GetThread();

private:
  static void* MessageLoop(void *p);

  bool HasTask();

  std::unique_ptr<Task> GetTask();

  std::queue<std::unique_ptr<Task> > tasks_;
  PTHREAD thread_;
};

} // namespace coomy