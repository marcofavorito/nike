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
#include <nike/logic/atom_visitor.hpp>
#include <nike/logic/ltlf.hpp>

namespace nike {
namespace logic {
namespace Test {

TEST_CASE("find_atoms of tt", "[logic][ltlf]") {
  auto context = Context();
  auto tt1 = context.make_tt();
  REQUIRE(find_atoms(*tt1).empty());
}

TEST_CASE("find_atoms of ff", "[logic][ltlf]") {
  auto context = Context();
  auto ff1 = context.make_ff();
  REQUIRE(find_atoms(*ff1).empty());
}

TEST_CASE("find_atoms of true", "[logic][ltlf]") {
  auto context = Context();
  auto prop_true1 = context.make_prop_true();
  REQUIRE(find_atoms(*prop_true1).empty());
}

TEST_CASE("find_atoms of false", "[logic][ltlf]") {
  auto context = Context();
  auto prop_false1 = context.make_prop_false();
  REQUIRE(find_atoms(*prop_false1).empty());
}

TEST_CASE("find_atoms of atom", "[logic][ltlf]") {
  auto context = Context();
  auto atom = context.make_atom("a");

  auto result = find_atoms(*atom);
  REQUIRE(result.size() == 1);
  REQUIRE(result == set_ast_ptr{atom});
}

} // namespace Test
} // namespace logic
} // namespace nike