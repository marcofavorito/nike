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

#include <functional>

namespace nike {
namespace search {

typedef unsigned long node_id_t;
typedef unsigned long cost_t;

enum SearchNodeType { OR, AND };

template <typename State> class SearchNode {
public:
  node_id_t nodeId;
  State state;
  SearchNode(node_id_t nodeId, State state) : nodeId{nodeId}, state{state} {}
};

template <typename State, typename Action,
          typename EquivalenceCheck = std::equal_to<State>>
class BaseSearchGraph {
private:
  struct c_unique {
    node_id_t current;
    c_unique() { current = 0; }
    node_id_t operator()() { return current++; }
  } UniqueNumber;

public:
  const State initialState_;
  const SearchNode<State> initialNode_;
  explicit BaseSearchGraph(State initialState_)
      : initialState_{initialState_}, initialNode_{SearchNode<State>(
                                          UniqueNumber(), initialState_)} {}

  cost_t get_cost(State state, Action action) {
    // uniform cost
    return 1;
  }

  bool equal_states(State left, State right) {
    return EquivalenceCheck(left, right);
  }
};

} // namespace search
} // namespace nike