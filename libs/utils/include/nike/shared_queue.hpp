#pragma once
/*
 * This file is part of Nike.
 *
 * Nike is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Nike is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Nike.  If not, see <https://www.gnu.org/licenses/>.
 */

// Implementation of a thread-safe queue:
// https://stackoverflow.com/a/36763257/7150172

#include <condition_variable>
#include <mutex>
#include <queue>

template <typename T> class SharedQueue {
public:
  SharedQueue();
  ~SharedQueue();

  T &front();
  void pop_front();

  void push_back(const T &item);
  void push_back(T &&item);

  int size();
  bool empty() { return size() == 0; };

private:
  std::deque<T> queue_;
  std::mutex mutex_;
  std::condition_variable cond_;
};

template <typename T> SharedQueue<T>::SharedQueue() = default;

template <typename T> SharedQueue<T>::~SharedQueue() = default;

template <typename T> T &SharedQueue<T>::front() {
  std::unique_lock<std::mutex> mlock(mutex_);
  while (queue_.empty()) {
    cond_.wait(mlock);
  }
  return queue_.front();
}

template <typename T> void SharedQueue<T>::pop_front() {
  std::unique_lock<std::mutex> mlock(mutex_);
  while (queue_.empty()) {
    cond_.wait(mlock);
  }
  queue_.pop_front();
}

template <typename T> void SharedQueue<T>::push_back(const T &item) {
  std::unique_lock<std::mutex> mlock(mutex_);
  queue_.push_back(item);
  mlock.unlock();     // unlock before notification to minimize mutex con
  cond_.notify_one(); // notify one waiting thread
}

template <typename T> void SharedQueue<T>::push_back(T &&item) {
  std::unique_lock<std::mutex> mlock(mutex_);
  queue_.push_back(std::move(item));
  mlock.unlock();     // unlock before notification to minimize mutex con
  cond_.notify_one(); // notify one waiting thread
}

template <typename T> int SharedQueue<T>::size() {
  std::unique_lock<std::mutex> mlock(mutex_);
  int size = queue_.size();
  mlock.unlock();
  return size;
}
