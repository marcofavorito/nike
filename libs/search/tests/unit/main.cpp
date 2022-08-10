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

#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this
// in one cpp file
#include <catch.hpp>

#include <nike/search/base_graph.hpp>
#include <nike/search/simple_graph.hpp>

namespace nike {
namespace search {
namespace Test {

TEST_CASE("basic test") {
  std::map<int, std::map<std::string, int>> transitions = {
      {4, {{"a", 2}, {"b", 3}}},
      {3, {{"a", 2}}},
      {2,
       {
           {"a", 1},
           {"b", 0},
       }},
      {1, {{"a", 0}}},
      {0, {}}};
  SimpleSearchGraph<int, std::string> graph(4, transitions, [](int state) {
    auto final_states = std::set<int>{{0, 1}};
    return final_states.find(state) != final_states.end();
  });

  SECTION("transitions from 0") {
    auto transitions_from_0_begin = graph.get_transitions_begin(0);
    auto transitions_from_0_end = graph.get_transitions_end(0);
    std::vector<std::pair<std::string, int>> transitions_from_0(
        transitions_from_0_begin, transitions_from_0_end);
    REQUIRE(transitions_from_0.empty());
  };

  SECTION("transitions from 1") {
    auto transitions_from_1_begin = graph.get_transitions_begin(1);
    auto transitions_from_1_end = graph.get_transitions_end(1);
    std::vector<std::pair<std::string, int>> transitions_from_1(
        transitions_from_1_begin, transitions_from_1_end);
    REQUIRE(transitions_from_1 ==
            std::vector<std::pair<std::string, int>>{{"a", 0}});
  }

  SECTION("goal state, positive") { REQUIRE(graph.is_goal_state(0)); }
  SECTION("goal state, negative") { REQUIRE(!graph.is_goal_state(2)); }
}

} // namespace Test
} // namespace search
} // namespace nike