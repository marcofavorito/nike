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

#include <map>
#include <set>
#include <string>
#include <vector>

#include <cuddObj.hh>

namespace nike {
namespace core {

typedef CUDD::BDD graph_move_t;

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
  // destination-node -> (start-node -> label)
  std::map<Node, std::map<Node, graph_move_t, std::less<>>, std::less<>>
      predecessors;
  // backward transitions might be non-deterministic
  // start-node -> (destination-node -> label)
  std::map<Node, std::map<Node, graph_move_t, std::less<>>, std::less<>>
      successors;

  static void insert_with_default_(
      std::map<Node, std::map<Node, graph_move_t, std::less<>>, std::less<>> &m,
      Node start, const graph_move_t &action, Node end);
  static void insert_backward_with_default_(
      std::map<Node, std::map<Node, graph_move_t, std::less<>>, std::less<>> &m,
      Node start, const graph_move_t &action, Node end);
  template <typename K, typename V, typename C>
  static std::map<K, V, C>
  get_or_empty_(const std::map<Node, std::map<K, V, C>, C> &m, Node key) {
    auto item_or_end = m.find(key);
    if (item_or_end == m.end()) {
      return {};
    }
    return item_or_end->second;
  }

public:
  void add_transition(Node start, const graph_move_t &action, Node end);
  std::map<Node, graph_move_t, std::less<>> get_successors(Node start) const;
  std::map<Node, graph_move_t, std::less<>> get_predecessors(Node end) const;
};

} // namespace core
} // namespace nike
