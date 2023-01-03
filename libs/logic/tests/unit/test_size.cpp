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
#include <nike/logic/ltlf.hpp>
#include <nike/logic/size.hpp>

namespace nike {
namespace logic {
namespace Test {

TEST_CASE("tt size", "[logic][ltlf][size]") {
  auto context = Context();
  auto tt = context.make_tt();
  REQUIRE(size(*tt) == 1);
}

TEST_CASE("ff size", "[logic][ltlf][size]") {
  auto context = Context();
  auto ff = context.make_ff();
  REQUIRE(size(*ff) == 1);
}

TEST_CASE("true size", "[logic][ltlf][size]") {
  auto context = Context();
  auto prop_true = context.make_prop_true();
  REQUIRE(size(*prop_true) == 1);
}

TEST_CASE("false size", "[logic][ltlf][size]") {
  auto context = Context();
  auto prop_false = context.make_prop_false();
  REQUIRE(size(*prop_false) == 1);
}

TEST_CASE("atom size", "[logic][ltlf][size]") {
  auto context = Context();
  auto atom = context.make_atom("a");
  REQUIRE(size(*atom) == 1);
}

TEST_CASE("propositional not size", "[logic][ltlf][size]") {
  auto context = Context();
  auto atom = context.make_atom("a");
  auto not_atom = context.make_prop_not(atom);
  REQUIRE(size(*not_atom) == 1);
}

TEST_CASE("conjunction size", "[logic][ltlf][size]") {
  auto context = Context();

  auto atom_a = context.make_atom("a");
  auto atom_b = context.make_atom("b");
  auto a_and_b = context.make_and(vec_ptr{atom_a, atom_b});
  REQUIRE(size(*a_and_b) == 3);
}

TEST_CASE("disjunction size", "[logic][ltlf][size]") {
  auto context = Context();

  auto atom_a = context.make_atom("a");
  auto atom_b = context.make_atom("b");
  auto a_or_b = context.make_or(vec_ptr{atom_a, atom_b});
  REQUIRE(size(*a_or_b) == 3);
}

TEST_CASE("next size", "[logic][ltlf][size]") {
  auto context = Context();
  auto atom_a = context.make_atom("a");
  auto next_a = context.make_next(atom_a);
  REQUIRE(size(*next_a) == 2);
}

TEST_CASE("weak next size", "[logic][ltlf][size]") {
  auto context = Context();
  auto atom_a = context.make_atom("a");
  auto weak_next_a = context.make_weak_next(atom_a);
  REQUIRE(size(*weak_next_a) == 2);
}

TEST_CASE("until size", "[logic][ltlf][size]") {
  auto context = Context();
  auto atom_a = context.make_atom("a");
  auto atom_b = context.make_atom("b");
  auto a_until_b = context.make_until(vec_ptr{atom_a, atom_b});
  REQUIRE(size(*a_until_b) == 3);
}

TEST_CASE("release size", "[logic][ltlf][size]") {
  auto context = Context();
  auto atom_a = context.make_atom("a");
  auto atom_b = context.make_atom("b");
  auto a_release_b = context.make_release(vec_ptr{atom_a, atom_b});
  REQUIRE(size(*a_release_b) == 3);
}

TEST_CASE("eventually size", "[logic][ltlf][size]") {
  auto context = Context();
  auto atom_a = context.make_atom("a");
  auto eventually_a = context.make_eventually(atom_a);
  REQUIRE(size(*eventually_a) == 2);
}

TEST_CASE("always size", "[logic][ltlf][size]") {
  auto context = Context();
  auto atom_a = context.make_atom("a");
  auto always_a = context.make_always(atom_a);
  REQUIRE(size(*always_a) == 2);
}

TEST_CASE("end size", "[logic][ltlf][size]") {
  auto context = Context();
  auto end_formula = context.make_end();
  REQUIRE(size(*end_formula) == 2);
}

TEST_CASE("last size", "[logic][ltlf][size]") {
  auto context = Context();
  auto last_formula = context.make_last();
  REQUIRE(size(*last_formula) == 2);
}

} // namespace Test
} // namespace logic
} // namespace nike