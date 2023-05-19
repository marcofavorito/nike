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
#include <map>
#include <iostream>

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
TEST_CASE("check univ", "[core][cudd]") {
  CUDD::Cudd mgr;

  CUDD::BDD x = mgr.bddVar();
  CUDD::BDD y = mgr.bddVar();

  CUDD::BDD x_or_y = x | y;
  CUDD::BDD x_and_y = x & y;

  CUDD::BDD result_or = x_or_y.UnivAbstract(x);
  CUDD::BDD result_and = x_and_y.UnivAbstract(x);
  CUDD::BDD result_x = x.UnivAbstract(x);
  REQUIRE(result_or == y);
  REQUIRE(result_x.IsZero());
  REQUIRE(result_and.IsZero());
}
TEST_CASE("pick cube", "[core][cudd]") {
  CUDD::Cudd mgr;

  CUDD::BDD x = mgr.bddVar();
  CUDD::BDD y = mgr.bddVar();
  CUDD::BDD z = mgr.bddVar();

  CUDD::BDD bdd_or = (x & y) | z;

  char assignment[3];
  bdd_or.UnivAbstract(z).PickOneCube(assignment);
  std::string assignment_str = std::string(assignment);

  REQUIRE(assignment_str[0] == 1);
  REQUIRE(assignment_str[1] == 1);
  REQUIRE(assignment_str[2] == 2);
}

TEST_CASE("test zdd", "[core][cudd]") {
  CUDD::Cudd mgr;

  CUDD::ZDD x = mgr.zddVar(0);
  CUDD::ZDD y = mgr.zddVar(1);

  CUDD::ZDD result_and = x & y;
  CUDD::ZDD result_or = x | y;
  CUDD::ZDD result_x = x & x;
  CUDD::ZDD result_y = y & y;

  auto zddMap = std::map<DdNode *, int>({
      {x.getNode(), 0},
      {y.getNode(), 1},
      {x.getNode(), 2},
  });
  REQUIRE(zddMap.size() == 2);
}
TEST_CASE("test zdd one", "[core][cudd]") {
  CUDD::Cudd mgr(0, 3);

  CUDD::ZDD x = mgr.zddVar(0);
  CUDD::ZDD y = mgr.zddVar(1);
  CUDD::ZDD z = mgr.zddVar(2);

  CUDD::ZDD zero = mgr.zddZero();

  CUDD::ZDD all1 = mgr.zddOne(3);
  CUDD::ZDD xyz1 = all1 & x & y & z;
  REQUIRE(xyz1 != all1);
  REQUIRE(xyz1 != zero);

  CUDD::ZDD all2 = mgr.zddOne(3 - 1);
  CUDD::ZDD xyz2 = all2 & x & y & z;
  REQUIRE(xyz1 == xyz2);
  REQUIRE(xyz2 != all2);
  REQUIRE(xyz2 != zero);

  CUDD::ZDD all3 = mgr.zddOne(3 - 2);
  CUDD::ZDD xyz3 = all2 & x & y & z;
  REQUIRE(xyz1 == xyz3);
  REQUIRE(xyz3 != all3);
  REQUIRE(xyz3 != zero);
}

} // namespace Test
} // namespace logic
} // namespace nike