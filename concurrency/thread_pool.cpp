#include "thread_pool.h"
#include <thread>        // std::thread::hardware_concurrency
#include <cstddef>       // std::size_t

void Thread_Pool::worker()
{
  while (true) {
    Task fn;
    m_concurrent_queue.pop(fn);
    fn();
  }
}


Thread_Pool::Thread_Pool()
{
  const auto t_count = std::thread::hardware_concurrency();

  for (std::size_t i{}; i < t_count; ++i)
    m_tvec.push_back(std::thread{ &Thread_Pool::worker, this });
  
}

Thread_Pool::~Thread_Pool()
{
  for (auto& tx : m_tvec)
    tx.join();
}

void Thread_Pool::push(Task fn)
{
  m_concurrent_queue.push(fn);
}



