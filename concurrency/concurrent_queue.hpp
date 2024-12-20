#ifndef _CONCURRENT_QUEUE_HPP
#define _CONCURRENT_QUEUE_HPP

#include <mutex>
#include <queue>
#include <condition_variable>

template <typename T>
class Concurrent_Queue {
private:
  std::mutex m_mtx;
  std::queue<T> m_queue;
  std::condition_variable m_cv;

public:
  void push(const T& item)
  {
    std::scoped_lock guard{ m_mtx };
    m_queue.push(item);
    m_cv.notify_one();
  }

  void pop(T& item)
  {
    std::unique_lock guard{ m_mtx };
    m_cv.wait(guard, [this] { return !m_queue.empty(); });
    item = m_queue.front();
    m_queue.pop();
  }
};

#endif // _CONCURRENT_QUEUE_HPP