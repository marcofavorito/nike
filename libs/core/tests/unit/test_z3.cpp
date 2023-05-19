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
#include <iostream>

#include <z3++.h>


namespace nike {
namespace logic {
namespace Test {


TEST_CASE("z3 example", "[core][z3]") {

    z3::context c;

    z3::expr x = c.bool_const("x");
    z3::expr y = c.bool_const("y");
    z3::expr z = c.bool_const("z");
    z3::expr formula = z3::forall(x, x | (z & y));
    z3::solver s(c);

    // making sure model based quantifier instantiation is enabled.
    z3::params p(c);
    s.set(p);

    s.add(formula);

    REQUIRE(s.check());
    z3::model m = s.get_model();

    REQUIRE(m.size() == 2);
    REQUIRE(z3::eq(m.eval(y), c.bool_val(true)));
    REQUIRE(z3::eq(m.eval(z), c.bool_val(true)));
  }

TEST_CASE("z3 example simplification", "[core][z3]") {

    z3::context c;

    z3::expr x = c.bool_const("x");
    z3::expr formula = x & !x;
    z3::solver s(c);
    formula = formula.simplify();

    // making sure model based quantifier instantiation is enabled.
    z3::params p(c);
    s.set(p);

    s.add(formula);

    REQUIRE(!s.check());
    REQUIRE(z3::eq(formula, c.bool_val(false)));
//    z3::model m = s.get_model();
//
//    REQUIRE(m.size() == 0);
  }


} // namespace Test
} // namespace logic
} // namespace nike