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

#include "base_graph.hpp"
#include <queue>
#include <set>

namespace nike {
namespace search {

template <typename State, typename Action, typename Graph,
          typename std::enable_if<std::is_base_of<
              BaseSearchGraph<State, Action>, Graph>::value>::type>
void uniform_cost(BaseSearchGraph<State, Action> &graph) {
  auto queue = std::queue<State>();
  auto visited = std::set<node_id_t>();

  auto initialNode = graph.initialNode_;
  queue.push(initialNode);
  visited.insert(initialNode.nodeId);
}

} // namespace search
} // namespace nike