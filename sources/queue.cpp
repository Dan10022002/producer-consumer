// Copyright 2021 Dan10022002 dan10022002@mail.ru

#include <queue.hpp>

template <typename T>
Queue<T>::Queue() {
  _counter = 0;
}

template <typename T>
void Queue<T>::push(T &&data) {
  std::lock_guard<std::mutex> _lock(_mutex);
  _queue.push(data);
  _counter += 1;
}

template <typename T>
T Queue<T>::front() {
  std::lock_guard<std::mutex> _lock(_mutex);
  return _queue.front();
}

template <typename T>
void Queue<T>::pop() {
  std::lock_guard<std::mutex> _lock(_mutex);
  _queue.pop();
  _counter -= 1;
}

template <typename T>
bool Queue<T>::empty() {
  std::lock_guard<std::mutex> _lock(_mutex);
  return _queue.empty();
}