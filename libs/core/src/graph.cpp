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

#include <nike/graph.hpp>
#include <stdexcept>

namespace nike {
namespace core {

void Graph::insert_with_default_(std::map<Node, std::map<size_t, Node>> &m,
                                 Node start, size_t action, Node end) {
  auto start_item = m.find(start);
  if (start_item == m.end()) {
    auto new_value = std::map<size_t, Node>();
    new_value[action] = end;
    m[start] = new_value;
    return;
  }
  // if already inserted and with different end state, fail
  if (start_item->second.find(action) != start_item->second.end() and
      start_item->second[action].id != end.id) {
    throw std::logic_error("action already set with different state");
  }
  start_item->second[action] = end;
}
void Graph::insert_backward_with_default_(
    std::map<Node, std::map<size_t, std::set<Node>>> &m, Node start,
    size_t action, Node end) {
  auto start_item = m.find(start);
  if (start_item == m.end()) {
    auto new_value = std::map<size_t, std::set<Node>>();
    auto predecessor_set = std::set<Node>({start});
    new_value[action] = predecessor_set;
    m[end] = new_value;
    return;
  }
  start_item->second[action].insert(start);
}

move_t Graph::get_action_by_id(size_t action_id) const {
  return action_by_id.at(action_id);
};
std::map<move_t, Node, CompareVector> Graph::get_successors(Node start) const {
  return get_or_empty_(transitions, start);
};
std::map<move_t, std::set<Node>, CompareVector>
Graph::get_predecessors(Node end) const {
  return get_or_empty_(backward_transitions, end);
};

void Graph::add_transition(Node start, move_t action, Node end) {
  //  unsigned long long action_id = sdd_id(action);
  //  action_by_id[sdd_id(action)] = action;
  //  insert_with_default_(transitions, start, action_id, end);
  //  insert_backward_with_default_(backward_transitions, start, action_id,
  //  end);
}

} // namespace core
} // namespace nike
