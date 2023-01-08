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

#include <future>
#include <nike/core.hpp>
#include <nike/core_base.hpp>
#include <nike/multithreaded.hpp>
#include <thread>

namespace nike {
namespace core {

int termination_callback(const void *ptr) {
  auto syn = (ForwardSynthesis *)ptr;
  if (syn->is_stopped()) {
    return 1;
  }
  return 0;
}

void job_is_realizable(ForwardSynthesis *syn, unsigned int thread_id,
                       SharedQueue<std::pair<unsigned int, bool>> &queue) {
  syn->register_termination_callback(termination_callback, syn);
  try {
    auto result = syn->is_realizable();
    queue.push_back(std::make_pair(thread_id, result));
  } catch (interrupted_exception e) {
    // do nothing
  } catch (std::exception e) {
    //  do nothing
  }
}

void ThreadedForwardSynthesis::start() {
  if (started) {
    throw std::runtime_error("already started");
  }
  t = std::thread(job_is_realizable, syn.get(), thread_id, std::ref(queue));
  started = true;
}

void ThreadedForwardSynthesis::stop() { syn->stop(); }
void ThreadedForwardSynthesis::join() { t.join(); }

bool MultithreadedSynthesis::is_realizable() {
  std::vector<StateEquivalenceMode> modes{StateEquivalenceMode::HASH};

  std::vector<BranchingStrategy> bs{
      BranchingStrategy::TRUE_FIRST,
      BranchingStrategy::FALSE_FIRST,
      BranchingStrategy::RANDOM,
  };

  std::vector<ThreadedForwardSynthesis> tasks;
  SharedQueue<std::pair<unsigned int, bool>> queue;

  logger.info("Initializing synthesis tasks");
  auto formula_ = formula;
  auto partition_ = partition;
  unsigned int thread_id = 0;
  for (auto &mode : modes) {
    for (auto &branch_variable : bs) {

      if (!queue.empty()) {
        logger.info("some task already returned during initialization");
        break;
      }

      tasks.emplace_back(formula_, partition_, branch_variable, mode, queue,
                         thread_id);
      tasks[thread_id].start();
      ++thread_id;
    }

    if (!queue.empty())
      break;
  }

  // blocking call
  logger.info("Waiting for completion of one task");
  auto result = queue.front();
  auto task_id = result.first;
  auto is_realizable = result.second;
  logger.info("Task {} completed with result {}", task_id, is_realizable);

  logger.info("Sending stop signal to other tasks");
  for (auto &task : tasks) {
    if (task.thread_id == task_id)
      continue;
    task.stop();
  }
  logger.info("Waiting for completion of tasks");
  for (auto &task : tasks) {
    task.join();
    logger.info("Task {} terminated", task.thread_id);
  }
  logger.info("All tasks terminated");
  return is_realizable;
}

} // namespace core
} // namespace nike
