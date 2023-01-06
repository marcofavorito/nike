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

#include <algorithm>
#include <nike/strategy.hpp>
#include <stdexcept>
#include <utility>

namespace nike {
namespace core {

std::string varvalues_to_string(VarValues value) {
  switch (value) {
  case VarValues::TRUE:
    return "true";
  case VarValues::FALSE:
    return "false";
  case VarValues::DONT_CARE:
    return "dont-care";
  }
}

Strategy::Strategy(std::vector<std::string> variables_by_id)
    : variables_by_id{std::move(variables_by_id)} {}

void Strategy::add_move(size_t state, move_t move) {
  auto it = state_to_move.find(state);

  // check state has not been already set
  if (it != state_to_move.end()) {
    throw std::logic_error("strategy already set for state " +
                           std::to_string(state));
  }

  // check move is valid
  if (move.size() != variables_by_id.size()) {
    throw std::invalid_argument("expected number of variables to be " +
                                std::to_string(move.size()) + ", got " +
                                std::to_string(variables_by_id.size()));
  }
  for (int i = 0; i < move.size(); i++) {
    auto expected = variables_by_id[i];
    auto actual = move[i].first;
    if (actual != expected) {
      throw std::invalid_argument("expected variable " + expected +
                                  " for index " + std::to_string(i) + ", got " +
                                  actual);
    }
  }
  state_to_move[state] = std::move(move);
}

void Strategy::add_move_from_stack(
    size_t state, std::stack<std::pair<std::string, VarValues>> values) {
  move_t move = from_stack_to_vector(std::move(values));
  add_move(state, move);
}

move_t Strategy::from_stack_to_vector(
    std::stack<std::pair<std::string, VarValues>> values) {
  move_t temp_move;
  move_t result;
  // from stack to vector
  while (!values.empty()) {
    auto element = values.top();
    values.pop();
    temp_move.push_back(element);
  }
  std::reverse(temp_move.begin(), temp_move.end());

  // populate move with missing variables
  int index = 0;
  for (const auto &varname : variables_by_id) {
    if (index < temp_move.size() and temp_move[index].first == varname) {
      result.emplace_back(varname, temp_move[index].second ? VarValues::TRUE
                                                           : VarValues::FALSE);
      ++index;
    } else {
      result.emplace_back(varname, VarValues::DONT_CARE);
    }
  }
  return result;
}

std::string
move_stack_to_string(std::stack<std::pair<std::string, VarValues>> values) {
  std::string result = "{";
  while (!values.empty()) {
    auto pair = values.top();
    values.pop();
    result +=
        "'" + pair.first + "' : " + varvalues_to_string(pair.second) + ", ";
  }
  result += "}";
  return result;
}

} // namespace core
} // namespace nike
