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

#include "core_test_utils.hpp"
#include <catch.hpp>
#include <nike/core.hpp>
#include <nike/parser/driver.hpp>
#include <sstream>

namespace nike {
namespace core {
namespace Test {

TEST_CASE("forward synthesis of random formula 1") {
  auto driver = parser::ltlf::LTLfDriver();
  std::istringstream fstring(
      "(((p0) | (G(F(p5)))) & (F(p4))) U  (((p3) & ((~(p1)) | (F(~(p4))))) | "
      "((p1) & (~(p3)) & (G(p4))))");
  driver.parse(fstring);
  auto temp = driver.result;
  auto not_end = temp->ctx().make_not_end();
  auto formula = temp->ctx().make_and({temp, not_end});
  auto partition = InputOutputPartition({"p5"}, {"p0", "p1", "p3", "p4"});
  bool result = test_is_realizable(formula, partition);
  REQUIRE(result);
}

TEST_CASE("forward synthesis of random formula 2") {
  utils::Logger::level(utils::LogLevel::debug);
  auto driver = parser::ltlf::LTLfDriver();
  std::istringstream fstring(
      "(((p0) | (G(F(p4)))) & (F(p3))) U ((p3) & ((~(p1)) | (F(~(p3)))))");
  driver.parse(fstring);
  auto temp = driver.result;
  auto not_end = temp->ctx().make_not_end();
  auto formula = temp->ctx().make_and({temp, not_end});
  auto partition = InputOutputPartition({"p1", "p0", "p4"}, {"p3"});
  bool result = test_is_realizable(formula, partition);
  REQUIRE(result);
}
TEST_CASE("forward synthesis of random formula 3") {
  auto driver = parser::ltlf::LTLfDriver();
  std::istringstream fstring(
      "(((p0) | (G(F(p5)))) & (F(p4))) U (((p3) & ((~(p1)) | (F(~(p4))))) | "
      "((p1) & (~(p3)) & (G(p4))))");
  driver.parse(fstring);
  auto temp = driver.result;
  auto not_end = temp->ctx().make_not_end();
  auto formula = temp->ctx().make_and({temp, not_end});
  auto partition = InputOutputPartition({"p5", "p3"}, {"p1", "p0", "p4"});
  bool result = test_is_realizable(formula, partition);
  REQUIRE(result);
}
TEST_CASE("forward synthesis of random formula 4") {
  auto driver = parser::ltlf::LTLfDriver();
  std::istringstream fstring("(((~(p2)) | (p4)) & (F((~(p0)) & (~(p1))))) | "
                             "((p2) & (~(p4)) & (G((p0) | (p1))))");
  driver.parse(fstring);
  auto temp = driver.result;
  auto not_end = temp->ctx().make_not_end();
  auto formula = temp->ctx().make_and({temp, not_end});
  auto partition = InputOutputPartition({"p2", "p4"}, {"p1", "p0"});
  bool result = test_is_realizable(formula, partition);
  REQUIRE(result);
}
TEST_CASE("forward synthesis of random formula 5") {
  utils::Logger::level(utils::LogLevel::debug);
  auto driver = parser::ltlf::LTLfDriver();
  std::istringstream fstring("(~(X[!](ff))) -> (F(p0))");
  driver.parse(fstring);
  auto temp = driver.result;
  auto not_end = temp->ctx().make_not_end();
  auto formula = temp->ctx().make_and({temp, not_end});
  auto partition = InputOutputPartition({"p0"}, {"dummy"});
  bool result = test_is_realizable(formula, partition);
  REQUIRE(!result);
}

} // namespace Test
} // namespace core
} // namespace nike