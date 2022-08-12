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

class SearchGraph {
private:
  struct c_unique {
    node_id_t current;
    c_unique() { current = 0; }
    node_id_t operator()() { return current++; }
  } UniqueNumber;

  std::map<node_id_t, SearchNode<State> *> idToNode;
  std::map<node_id_t, SearchNode<State> *> stateToNode;

  std::map<node_id_t, std::map<Action, node_id_t>> transitions;
  // backward transitions might be non-deterministic
  std::map<Node, std::map<size_t, std::set<Node>>> backward_transitions;

  std::map<SddSize, SddNode *> action_by_id;
  static void insert_with_default_(std::map<Node, std::map<size_t, Node>> &m,
                                   Node start, size_t action, Node end);
  static void insert_backward_with_default_(
      std::map<Node, std::map<size_t, std::set<Node>>> &m, Node start,
      size_t action, Node end);
  template <typename K, typename V>
  static std::map<K, V> get_or_empty_(const std::map<Node, std::map<K, V>> &m,
                                      Node key) {
    auto item_or_end = m.find(key);
    if (item_or_end == m.end()) {
      return {};
    }
    return item_or_end->second;
  }

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