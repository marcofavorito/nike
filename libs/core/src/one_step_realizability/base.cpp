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

#include <memory>
#include <nike/one_step_realizability/base.hpp>
#include <nike/one_step_realizability/bdd.hpp>

namespace nike {
namespace core {
std::unique_ptr<OneStepRealizabilityChecker>
get_default_realizability_checker() {
  return std::make_unique<BddOneStepRealizabilityChecker>();
}
} // namespace core
} // namespace nike