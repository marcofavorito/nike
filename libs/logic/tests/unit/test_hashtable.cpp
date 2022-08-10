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

#include <catch.hpp>
#include <nike/logic/hashtable.hpp>
#include <nike/logic/ltlf.hpp>

namespace nike {
namespace logic {
namespace Test {
TEST_CASE("Create hash table", "[logic][hashtable]") {
  auto context = Context();
  auto table = HashTable{};
  // table must be empty
  REQUIRE(table.size() == 0);

  auto expected_element_1_ptr = std::make_shared<const LTLfAtom>(context, "1");
  auto actual_element_1_ptr =
      table.insert_if_not_available(expected_element_1_ptr);
  // table must contain one element
  REQUIRE(table.size() == 1);
  REQUIRE(*actual_element_1_ptr == *expected_element_1_ptr);
  REQUIRE(actual_element_1_ptr == expected_element_1_ptr);

  // table must still contain one element, and return the same pointer
  auto actual_element_1_ptr_b = table.insert_if_not_available(
      std::make_shared<const LTLfAtom>(context, "1"));
  REQUIRE(table.size() == 1);
  REQUIRE(*actual_element_1_ptr_b == *expected_element_1_ptr);
  REQUIRE(actual_element_1_ptr_b == expected_element_1_ptr);
}
} // namespace Test
} // namespace logic
} // namespace nike