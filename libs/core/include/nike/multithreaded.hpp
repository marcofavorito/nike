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

#include <future>
#include <nike/core_base.hpp>
#include <nike/logic/copy.hpp>
#include <nike/shared_queue.hpp>

namespace nike {
namespace core {

class ThreadedForwardSynthesis {
public:
  logic::ltlf_ptr formula;
  const InputOutputPartition partition;
  BranchingStrategy branch_variable_id;
  StateEquivalenceMode mode;
  SharedQueue<std::pair<unsigned int, bool>> &queue;
  std::thread t;
  std::unique_ptr<ForwardSynthesis> syn;
  unsigned int thread_id;
  std::unique_ptr<logic::Context> context;
  bool started = false;
  ThreadedForwardSynthesis(const logic::ltlf_ptr &formula,
                           const InputOutputPartition &partition,
                           BranchingStrategy bs, StateEquivalenceMode mode,
                           SharedQueue<std::pair<unsigned int, bool>> &queue,
                           unsigned int thread_id)
      : context{std::make_unique<logic::Context>()}, partition{partition},
        branch_variable_id{bs}, mode{mode}, queue{queue}, thread_id{thread_id} {
    this->formula = logic::copy_ltlf_formula(*context, *formula);
    std::string logger_name = "thread_" + std::to_string(thread_id);
    syn = std::make_unique<ForwardSynthesis>(
        this->formula, this->partition, branch_variable_id, mode, logger_name);
  }

  void start();
  void stop();
  void join();
};

class MultithreadedSynthesis : public ISynthesis {
private:
  utils::Logger logger;

public:
  MultithreadedSynthesis(const logic::ltlf_ptr &formula,
                         const InputOutputPartition &partition)
      : ISynthesis(formula, partition), logger{"multithreaded"} {};
  bool is_realizable() override;
};

} // namespace core
} // namespace nike