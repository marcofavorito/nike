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
#include "nike/one_step_realizability/bdd.hpp"
#include <catch.hpp>
#include <nike/core.hpp>
#include <nike/parser/driver.hpp>
#include <sstream>

namespace nike {
  namespace core {
    namespace Test {

TEST_CASE("forward synthesis of Uright pattern size 20") {
  auto driver = parser::ltlf::LTLfDriver();
  std::istringstream fstring("p1 U (p2 U (p3 U (p4 U (p5 U (p6 U (p7 U (p8 U (p9 U (p10 U (p11 U (p12 U (p13 U (p14 U (p15 U (p16 U (p17 U (p18 U (p19 U p20))))))))))))))))))");
  driver.parse(fstring);
  auto temp = driver.result;
  auto not_end = temp->ctx().make_not_end();
  auto formula = temp->ctx().make_and({temp, not_end});

  auto input_vars = std::vector<std::string>{"p1", "p3", "p5", "p6", "p8", "p2", "p4", "p7", "p9", "p10"};
  auto output_vars = std::vector<std::string>{"p12", "p14", "p17", "p19", "p11", "p13", "p15", "p16", "p18", "p20"};
  SECTION("realizable"){
    auto partition = InputOutputPartition(input_vars, output_vars);
    bool result = test_is_realizable(formula, partition);
    REQUIRE(result);
  }

}

TEST_CASE("one-step-realizability-check for counters subformula") {
  auto driver = parser::ltlf::LTLfDriver();
  std::istringstream fstring("((counter_env_0) | (G(ff))) & (((inc_env) & (F(tt))) | ((((inc_env) & (X[!](inc_env)) & (F(tt))) | (X[!](F((inc_env) & (X[!](inc_env)))))) & (F(tt))) | (((((inc_env) | (X((!carry_env_0) | (G(ff))))) & ((((counter_env_0) | (X[!](carry_env_0))) & ((!counter_env_0) | (X((!carry_env_0) | (G(ff)))) | (G(ff)))) | (X((!counter_env_0) | (G(ff))))) & ((((counter_sys_0) | (X[!](carry_sys_0))) & ((!counter_sys_0) | (X((!carry_sys_0) | (G(ff)))) | (G(ff)))) | (X((!counter_sys_0) | (G(ff))))) & (((inc_sys) & (carry_sys_0)) | (((!inc_sys) | (G(ff))) & ((!carry_sys_0) | (G(ff))))) & ((!inc_env) | (X(carry_env_0)) | (G(ff))) & (((counter_env_0) & (X(carry_env_0))) | (((!counter_env_0) | (G(ff))) & (X[!]((!carry_env_0) | (G(ff))))) | (X(counter_env_0))) & (((counter_sys_0) & (X(carry_sys_0))) | (((!counter_sys_0) | (G(ff))) & (X[!]((!carry_sys_0) | (G(ff))))) | (X(counter_sys_0)))) | (G(ff))) & (((((counter_env_0) & (counter_sys_0)) | (((!counter_sys_0) | (G(ff))) & ((!counter_env_0) | (G(ff))))) & (F(tt))) | (X[!](F(((counter_env_0) & (counter_sys_0)) | (((!counter_sys_0) | (G(ff))) & ((!counter_env_0) | (G(ff)))))))) & (X(G(((inc_env) | (X((!carry_env_0) | (G(ff))))) & ((((counter_env_0) | (X[!](carry_env_0))) & ((!counter_env_0) | (X((!carry_env_0) | (G(ff)))) | (G(ff)))) | (X((!counter_env_0) | (G(ff))))) & ((((counter_sys_0) | (X[!](carry_sys_0))) & ((!counter_sys_0) | (X((!carry_sys_0) | (G(ff)))) | (G(ff)))) | (X((!counter_sys_0) | (G(ff))))) & (((inc_sys) & (carry_sys_0)) | (((!inc_sys) | (G(ff))) & ((!carry_sys_0) | (G(ff))))) & ((!inc_env) | (X(carry_env_0)) | (G(ff))) & (((counter_env_0) & (X(carry_env_0))) | (((!counter_env_0) | (G(ff))) & (X[!]((!carry_env_0) | (G(ff))))) | (X(counter_env_0))) & (((counter_sys_0) & (X(carry_sys_0))) | (((!counter_sys_0) | (G(ff))) & (X[!]((!carry_sys_0) | (G(ff))))) | (X(counter_sys_0)))))) & (F(tt))))");
  driver.parse(fstring);
  auto formula = driver.result;
  auto input_vars = std::vector<std::string>{"init_counter_0", "inc_env"};
  auto output_vars = std::vector<std::string>{"counter_env_0", "counter_sys_0", "carry_env_0", "carry_sys_0", "inc_sys"};

  std::unique_ptr<OneStepRealizabilityChecker> bddChecker = std::make_unique<BddOneStepRealizabilityChecker>();

  SECTION("realizable"){
    auto partition = InputOutputPartition(input_vars, output_vars);
    auto bddResult = bddChecker->one_step_realizable(*formula, partition);
    REQUIRE(!bddResult );
  }
}


}}}