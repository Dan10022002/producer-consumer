// Copyright 2021 Dan10022002 dan10022002@mail.ru

#ifndef INCLUDE_QUEUE_HPP_
#define INCLUDE_QUEUE_HPP_

#include <queue>
#include <mutex>

template <typename T>
class Queue{
  std::queue<T> _queue;
  std::mutex _mutex;
 public:
  int _counter;
  Queue();
  void push(T&& data);
  T front();
  void pop();
  bool empty();
};

#endif //INCLUDE_QUEUE_HPP_