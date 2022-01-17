// Copyright 2021 Dan10022002 dan10022002@mail.ru

#ifndef INCLUDE_QUEUE_H_
#define INCLUDE_QUEUE_H_

#include <queue>
#include <mutex>

template <typename T>
class Queue{
  std::queue<T> _queue;
  std::mutex _mutex;

  public:
  int _counter;

Queue() {
  _counter = 0;
}

void push(T &&data) {
  std::lock_guard<std::mutex> _lock(_mutex);
  _queue.push(data);
  _counter += 1;
}

T front() {
  std::lock_guard<std::mutex> _lock(_mutex);
  return _queue.front();
}

void pop() {
  std::lock_guard<std::mutex> _lock(_mutex);
  _queue.pop();
  _counter -= 1;
}

bool empty() {
  std::lock_guard<std::mutex> _lock(_mutex);
  return _queue.empty();
}
};

#endif //INCLUDE_QUEUE_H_
