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
#include <nike/logic/copy.hpp>
#include <nike/logic/ltlf.hpp>

namespace nike {
namespace logic {
namespace Test {

TEST_CASE("copy of tt", "[logic][ltlf]") {
  auto context = Context();
  auto f1 = context.make_tt();

  auto f2 = copy_ltlf_formula(*f1);
  REQUIRE(f1 != f2);
  REQUIRE(*f1 == *f2);
}

TEST_CASE("copy of ff", "[logic][ltlf]") {
  auto context = Context();
  auto f1 = context.make_ff();

  auto f2 = copy_ltlf_formula(*f1);
  REQUIRE(f1 != f2);
  REQUIRE(*f1 == *f2);
}

TEST_CASE("copy of true", "[logic][ltlf]") {
  auto context = Context();
  auto f1 = context.make_prop_true();

  auto f2 = copy_ltlf_formula(*f1);
  REQUIRE(f1 != f2);
  REQUIRE(*f1 == *f2);
}

TEST_CASE("copy of false", "[logic][ltlf]") {
  auto context = Context();
  auto f1 = context.make_prop_false();

  auto f2 = copy_ltlf_formula(*f1);
  REQUIRE(f1 != f2);
  REQUIRE(*f1 == *f2);
}

TEST_CASE("copy of atom", "[logic][ltlf]") {
  auto context = Context();
  auto f1 = context.make_atom("a");

  auto f2 = copy_ltlf_formula(*f1);
  REQUIRE(f1 != f2);
  REQUIRE(*f1 == *f2);
}

} // namespace Test
} // namespace logic
} // namespace nike