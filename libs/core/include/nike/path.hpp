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

#include <cstddef>
#include <nike/logic/pl.hpp>
#include <set>
#include <stack>

namespace nike {
namespace core {

class Path {
private:
  std::stack<logic::pl_ptr> path;
  std::set<logic::pl_ptr> node_set;

public:
  void push(logic::pl_ptr node);
  logic::pl_ptr pop();
  logic::pl_ptr back();
  bool contains(logic::pl_ptr);
};

} // namespace core
} // namespace nike
