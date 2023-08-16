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

#include <nike/input_output_partition.hpp>
#include <nike/logic/ltlf.hpp>
#include <nike/strategy.hpp>
#include <optional>

namespace nike {
namespace core {

class Context;

class OneStepRealizabilityChecker {
public:
  virtual std::optional<move_t>
  one_step_realizable(const logic::LTLfFormula &f,
                      const InputOutputPartition &partition) = 0;
};

std::unique_ptr<OneStepRealizabilityChecker>
get_default_realizability_checker();

} // namespace core
} // namespace nike
