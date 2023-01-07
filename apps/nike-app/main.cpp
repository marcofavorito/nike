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

#include <CLI/CLI.hpp>

int main(int argc, char **argv) {
  nike::utils::Logger logger("main");
  nike::utils::Logger::level(nike::utils::LogLevel::info);

  CLI::App app{"A tool for SDD-based forward LTLf synthesis."};

  bool no_empty = false;
  app.add_flag("-n,--no-empty", no_empty, "Enforce non-empty semantics.");
  bool version = false;
  app.add_flag("-V,--version", version, "Print the version and exit.");
  bool verbose = false;
  app.add_flag("-v,--verbose", verbose, "Set verbose mode.");

  // options & flags
  std::string filename;
  std::string formula;

  auto format = app.add_option_group("Input format");
  CLI::Option *formula_opt = app.add_option("-i,--inline", formula, "Formula.");
  CLI::Option *file_opt =
      app.add_option("-f,--file", filename, "File to formula.")
          ->check(CLI::ExistingFile);
  formula_opt->excludes(file_opt);
  file_opt->excludes(formula_opt);
  format->add_option(formula_opt);
  format->add_option(file_opt);
  format->require_option(1, 1);

  std::string part_file;
  CLI::Option *part_opt = app.add_option("--part", part_file, "Partition file.")
                              ->check(CLI::ExistingFile);

  CLI11_PARSE(app, argc, argv)

  if (version) {
    std::cout << NIKE_VERSION << std::endl;
    return 0;
  }

  if (verbose) {
    nike::utils::Logger::level(nike::utils::LogLevel::debug);
  }

  auto driver = nike::parser::ltlf::LTLfDriver();
  if (!file_opt->empty()) {
    logger.info("Parsing {}", filename);
    driver.parse(filename.c_str());
  } else {
    std::stringstream formula_stream(formula);
    logger.info("Parsing {}", formula);
    driver.parse(formula_stream);
  }

  auto parsed_formula = driver.get_result();
  if (no_empty) {
    logger.info("Apply no-empty semantics.");
    auto context = driver.context;
    auto end = context->make_end();
    auto not_end = context->make_not(end);
    parsed_formula = context->make_and({parsed_formula, not_end});
  }

  logger.info("Reading partition file {}", part_file);
  auto partition = nike::core::InputOutputPartition::read_from_file(part_file);

  logger.info("Starting synthesis");

  auto t_start = std::chrono::high_resolution_clock::now();

  bool result = nike::core::is_realizable<nike::core::ForwardSynthesis>(
      parsed_formula, partition);
  if (result)
    logger.info("realizable.");
  else
    logger.info("unrealizable.");

  auto t_end = std::chrono::high_resolution_clock::now();
  double elapsed_time =
      std::chrono::duration<double, std::milli>(t_end - t_start).count();
  logger.info("Overall time elapsed: {}ms", elapsed_time);
  return 0;
}
