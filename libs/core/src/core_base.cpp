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

#include <nike/core_base.hpp>

namespace nike {
namespace core {

std::string mode_to_string(StateEquivalenceMode mode) {
  switch (mode) {
  case StateEquivalenceMode::HASH:
    return "hash";
  case StateEquivalenceMode::BDD:
    return "bdd";
  }
}

bool TrueFirstBranchVariable::choose(std::string varname) { return true; }

bool FalseFirstBranchVariable::choose(std::string varname) { return false; }

bool RandomBranchVariable::choose(std::string varname) {
  return (rand() % 2) == 0;
}

ISynthesis::ISynthesis(const logic::ltlf_ptr &formula,
                       const InputOutputPartition &partition)
    : formula{formula}, partition{partition} {}

std::string branching_strategy_to_string(BranchingStrategy bs) {
  switch (bs) {
  case BranchingStrategy::TRUE_FIRST:
    return "true-first";
  case BranchingStrategy::FALSE_FIRST:
    return "false-first";
  case BranchingStrategy::RANDOM:
    return "random";
  }
}

std::unique_ptr<nike::core::BranchVariable>
get_branching_strategy(BranchingStrategy bs_id) {
  switch (bs_id) {
  case BranchingStrategy::TRUE_FIRST:
    return std::make_unique<nike::core::TrueFirstBranchVariable>();
  case BranchingStrategy::FALSE_FIRST:
    return std::make_unique<nike::core::FalseFirstBranchVariable>();
  case BranchingStrategy::RANDOM:
    return std::make_unique<nike::core::RandomBranchVariable>();
  }
}

} // namespace core
} // namespace nike