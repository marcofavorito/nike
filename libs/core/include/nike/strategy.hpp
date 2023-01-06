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

#include <cuddObj.hh>
#include <map>
#include <stack>
#include <vector>

namespace nike {
namespace core {

enum VarValues {
  TRUE,
  FALSE,
  DONT_CARE,
};

typedef std::vector<std::pair<std::string, VarValues>> move_t;

class Strategy {

private:
  std::vector<std::string> variables_by_id;
  std::map<size_t, move_t> state_to_move;

public:
  CUDD::Cudd manager_;

  explicit Strategy(std::vector<std::string> variables_by_id);
  void add_move(size_t state, move_t move);
  void
  add_move_from_stack(size_t state,
                      std::stack<std::pair<std::string, VarValues>> values);
  move_t
  from_stack_to_vector(std::stack<std::pair<std::string, VarValues>> values);
};

std::string
move_stack_to_string(std::stack<std::pair<std::string, VarValues>> values);

} // namespace core
} // namespace nike