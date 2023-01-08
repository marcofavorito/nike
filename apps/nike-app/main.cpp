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
#include <nike/core_base.hpp>
#include <nike/multithreaded.hpp>
#include <nike/parser/driver.hpp>
#include <sstream>

#include <CLI/CLI.hpp>
#include <string>

int main(int argc, char **argv) {
  CLI::App app{"A tool for DPLL-based forward LTLf synthesis."};

  bool no_empty = false;
  app.add_flag("-n,--no-empty", no_empty, "Enforce non-empty semantics.");
  bool version = false;
  app.add_flag("-V,--version", version, "Print the version and exit.");
  bool verbose = false;
  app.add_flag("-v,--verbose", verbose, "Set verbose mode.");
  bool multithreaded = false;
  app.add_flag("-t,--multithreaded", multithreaded, "Multithreaded mode.");

  std::map<std::string, nike::core::StateEquivalenceMode> map{
      {nike::core::mode_to_string(nike::core::StateEquivalenceMode::BDD),
       nike::core::StateEquivalenceMode::BDD},
      {nike::core::mode_to_string(nike::core::StateEquivalenceMode::HASH),
       nike::core::StateEquivalenceMode::HASH},
  };
  nike::core::StateEquivalenceMode mode;
  app.add_option("-m,--mode", mode, "The mode to use.")
      ->transform(CLI::CheckedTransformer(map, CLI::ignore_case))
      ->required();

  std::map<std::string, nike::core::BranchingStrategy> branching_strategy_map{
      {branching_strategy_to_string(nike::core::BranchingStrategy::TRUE_FIRST),
       nike::core::BranchingStrategy::TRUE_FIRST},
      {branching_strategy_to_string(nike::core::BranchingStrategy::FALSE_FIRST),
       nike::core::BranchingStrategy::FALSE_FIRST},
      {branching_strategy_to_string(nike::core::BranchingStrategy::RANDOM),
       nike::core::BranchingStrategy::RANDOM},
  };
  nike::core::BranchingStrategy branching_strategy_id;
  app.add_option("-s,--strategy", branching_strategy_id,
                 "The branching strategy to use.")
      ->required()
      ->transform(
          CLI::CheckedTransformer(branching_strategy_map, CLI::ignore_case));

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

  std::string run_name = "nike";
  CLI::Option *run_name_opt = app.add_option(
      "--name", run_name, "Name to give to the run (useful for logging).");

  CLI11_PARSE(app, argc, argv)

  if (version) {
    std::cout << NIKE_VERSION << std::endl;
    return 0;
  }

  nike::utils::Logger logger(run_name);
  nike::utils::Logger::level(nike::utils::LogLevel::info);

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

  bool result;
  if (multithreaded) {
    logger.warning(
        "multithreaded mode selected. Ignoring mode '{}' and strategy '{}'",
        nike::core::mode_to_string(mode),
        nike::core::branching_strategy_to_string(branching_strategy_id));
    result = nike::core::is_realizable<nike::core::MultithreadedSynthesis>(
        parsed_formula, partition);
  } else {
    logger.info("Using synthesis mode '{}'", nike::core::mode_to_string(mode));
    logger.info("Using branching strategy '{}'",
                branching_strategy_to_string(branching_strategy_id));

    result = nike::core::is_realizable<nike::core::ForwardSynthesis>(
        parsed_formula, partition, branching_strategy_id, mode, run_name);
  }

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
