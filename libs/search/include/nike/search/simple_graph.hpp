#pragma once
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

#include <iterator>
#include <map>
#include <nike/search/base_graph.hpp>
#include <string>
#include <utility>

namespace nike {
namespace search {

template <typename State, typename Action>
class SimpleSearchGraph : public BaseSearchGraph<State, Action> {
public:
  std::map<State, std::map<Action, State>> transitions;
  std::function<bool(State)> goal_states;

  explicit SimpleSearchGraph(
      State initialState_, std::map<State, std::map<Action, State>> transitions,
      std::function<bool(State)> goal_states)
      : BaseSearchGraph<State, Action>(initialState_), transitions{transitions},
        goal_states{goal_states} {}

  typename std::map<Action, State>::iterator get_transitions_begin(State s) {
    return transitions[s].begin();
  }
  typename std::map<Action, State>::iterator get_transitions_end(State s) {
    return transitions[s].end();
  }

  bool is_goal_state(State s) { return goal_states(s); }
};

} // namespace search
} // namespace nike