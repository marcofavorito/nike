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

#include <cuddObj.hh>
#include <map>
#include <nike/input_output_partition.hpp>
#include <nike/logic/types.hpp>

namespace nike {
namespace core {

enum StateEquivalenceMode { HASH = 0, BDD = 1 };

std::string mode_to_string(StateEquivalenceMode mode);

class ISynthesis {
public:
  const logic::ltlf_ptr formula;
  const InputOutputPartition partition;
  ISynthesis(const logic::ltlf_ptr &formula,
             const InputOutputPartition &partition);
  virtual bool is_realizable() = 0;
};

template <class Synthesis,
          typename = typename std::enable_if<
              std::is_base_of<ISynthesis, Synthesis>::value>::type,
          typename... Args>
bool is_realizable(const logic::ltlf_ptr &formula,
                   InputOutputPartition &partition, Args &...args) {
  auto synthesis = Synthesis(formula, partition, args...);
  return synthesis.is_realizable();
}

class BranchVariable {
public:
  virtual bool choose(std::string varname) = 0;
  virtual ~BranchVariable() = default;
};

class TrueFirstBranchVariable : public BranchVariable {
public:
  bool choose(std::string varname) override;
};
class FalseFirstBranchVariable : public BranchVariable {
public:
  bool choose(std::string varname) override;
};
class RandomBranchVariable : public BranchVariable {
public:
  bool choose(std::string varname) override;
};

} // namespace core
} // namespace nike