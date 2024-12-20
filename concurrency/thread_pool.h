#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#include "concurrent_queue.hpp"
#include <thread>
#include <functional>   // std::function
#include <vector>
#include <thread>

using Task = std::function<void()>;

class Thread_Pool { 
private:
  Concurrent_Queue<Task> m_concurrent_queue;
  std::vector<std::thread> m_tvec;
  void worker();

public:
  Thread_Pool();
  ~Thread_Pool();
  void push(Task fn);
};

#endif // THREAD_POOL_HPP