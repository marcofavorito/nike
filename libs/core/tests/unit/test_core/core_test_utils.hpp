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

#include <nike/core.hpp>
#include <nike/input_output_partition.hpp>
#include <nike/logic/types.hpp>

namespace nike::core::Test {

inline bool test_is_realizable(const nike::logic::ltlf_ptr &formula,
                               InputOutputPartition &partition) {
  auto b = RandomBranchVariable();
  auto mode = StateEquivalenceMode::HASH;
  return nike::core::is_realizable<nike::core::ForwardSynthesis>(
      formula, partition, b, mode);
}

} // namespace nike::core::Test
