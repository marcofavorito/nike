#pragma once
/*
 * This file is part of Cynthia.
 *
 * Cynthia is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Cynthia is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Cynthia.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <map>
#include <set>
#include <string>
#include <vector>

namespace nike {
namespace core {

enum VarValues {
  TRUE,
  FALSE,
  DONT_CARE,
};

typedef std::vector<VarValues> move_t;

enum NodeType { AND = 0, OR = 1 };

struct Node {
  size_t id;
  NodeType type;

  bool operator<(const Node &node) const { return id < node.id; }
  std::string to_string() const {
    return std::to_string(id) + " " +
           (type == NodeType::AND ? "AND node" : "OR node");
  }
};

class CompareVector {
  template <typename T>
  size_t operator()(std::vector<const T> const &a,
                    std::vector<const T> const &b) const {
    return std::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
  }
};

class Graph {

private:
  std::map<Node, std::map<move_t, Node, CompareVector>> transitions;
  // backward transitions might be non-deterministic
  std::map<Node, std::map<move_t, std::set<Node>, CompareVector>>
      backward_transitions;

  std::map<size_t, move_t> action_by_id;
  static void insert_with_default_(std::map<Node, std::map<size_t, Node>> &m,
                                   Node start, size_t action, Node end);
  static void insert_backward_with_default_(
      std::map<Node, std::map<size_t, std::set<Node>>> &m, Node start,
      size_t action, Node end);
  template <typename K, typename V, typename C>
  static std::map<K, V, C>
  get_or_empty_(const std::map<Node, std::map<K, V, C>> &m, Node key) {
    auto item_or_end = m.find(key);
    if (item_or_end == m.end()) {
      return {};
    }
    return item_or_end->second;
  }

public:
  void add_transition(Node start, move_t action, Node end);
  move_t get_action_by_id(size_t action_id) const;
  std::map<move_t, Node, CompareVector> get_successors(Node start) const;
  std::map<move_t, std::set<Node>, CompareVector>
  get_predecessors(Node end) const;
};

} // namespace core
} // namespace nike
