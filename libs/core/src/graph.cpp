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

void Graph::insert_with_default_(
    std::map<Node, std::map<Node, graph_move_t, std::less<>>, std::less<>> &m,
    Node start, const graph_move_t &action, Node end) {
  auto start_item = m.find(start);
  if (start_item == m.end()) {
    auto new_value = std::map<Node, graph_move_t, std::less<>>();
    new_value[end] = action;
    m[start] = new_value;
    return;
  }
  auto end_item = start_item->second.find(end);
  if (end_item == start_item->second.end()) {
    start_item->second[end] = action;
    return;
  }
  // if already inserted and label conflict, fail
  if ((end_item->second & action).IsZero()) {
    throw std::logic_error("action are in conflict, fail");
  }
  end_item->second |= action;
}
void Graph::insert_backward_with_default_(
    std::map<Node, std::map<Node, graph_move_t, std::less<>>, std::less<>> &m,
    Node start, const graph_move_t &action, Node end) {
  auto end_item = m.find(end);

  if (end_item == m.end()) {
    auto new_value = std::map<Node, graph_move_t, std::less<>>();
    new_value[start] = action;
    m[end] = new_value;
    return;
  }
  auto start_item = end_item->second.find(start);
  if (start_item == end_item->second.end()) {
    end_item->second[start] = action;
    return;
  }
  end_item->second[start] |= action;
}

std::map<Node, graph_move_t, std::less<>>
Graph::get_successors(Node start) const {
  return get_or_empty_(successors, start);
};
std::map<Node, graph_move_t, std::less<>>
Graph::get_predecessors(Node end) const {
  return get_or_empty_(predecessors, end);
};

void Graph::add_transition(Node start, const graph_move_t &action, Node end) {
  insert_with_default_(successors, start, action, end);
  insert_backward_with_default_(predecessors, start, action, end);
}

} // namespace core
} // namespace nike
