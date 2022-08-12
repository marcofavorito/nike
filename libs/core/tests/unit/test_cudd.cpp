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
#include <cuddObj.hh>

namespace nike {
namespace logic {
namespace Test {

TEST_CASE("check exist", "[core][cudd]") {
  CUDD::Cudd mgr;

  CUDD::BDD x = mgr.bddVar();
  CUDD::BDD y = mgr.bddVar();

  CUDD::BDD x_or_y = x | y;
  CUDD::BDD x_and_y = x & y;

  CUDD::BDD result_or = x_or_y.ExistAbstract(x);
  CUDD::BDD result_and = x_and_y.ExistAbstract(x);
  CUDD::BDD result_x = x.ExistAbstract(x);
  REQUIRE(result_or.IsOne());
  REQUIRE(result_x.IsOne());
  REQUIRE(result_and == y);
}

} // namespace Test
} // namespace logic
} // namespace nike