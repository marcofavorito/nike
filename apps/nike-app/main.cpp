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

#include <nike/core.hpp>
#include <nike/parser/driver.hpp>
#include <sstream>
#include <string>

int main(int argc, char **argv) {

  //  std::string formula = "X[!](x & a)";
  std::string formula = "(x | a) & (X[!](x | a))";
  std::string part_file = "ignore/part.part";
  nike::utils::Logger::level(nike::utils::LogLevel::debug);
  auto driver = nike::parser::ltlf::LTLfDriver();
  std::stringstream formula_stream(formula);
  driver.parse(formula_stream);

  auto parsed_formula = driver.get_result();
  auto partition = nike::core::InputOutputPartition::read_from_file(part_file);

  bool result = nike::core::is_realizable<nike::core::ForwardSynthesis>(
      parsed_formula, partition, true);
  if (result)
    std::cout << "realizable." << std::endl;
  else
    std::cout << "unrealizable." << std::endl;

  return 0;
}
