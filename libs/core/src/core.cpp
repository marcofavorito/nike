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

#include "nike/logic/print.hpp"
#include "nike/logic/size.hpp"
#include <map>
#include <nike/core.hpp>
#include <nike/eval.hpp>
#include <nike/logic/atom_visitor.hpp>
#include <nike/logic/nnf.hpp>
#include <nike/logic/replace.hpp>
#include <nike/one_step_realizability.hpp>
#include <nike/one_step_unrealizability.hpp>
#include <nike/strip_next.hpp>
#include <nike/to_bdd.hpp>
#include <nike/to_ltlf.hpp>
#include <nike/to_pl.hpp>
#include <nike/xnf.hpp>

namespace nike {
namespace core {

ISynthesis::ISynthesis(const logic::ltlf_ptr &formula,
                       const InputOutputPartition &partition)
    : formula{formula}, partition{partition} {}

bool ForwardSynthesis::is_realizable() {
  bool result = ids_forward_synthesis_();
  return result;
}

bool ForwardSynthesis::ids_forward_synthesis_() {

  // first try faster version:
  try {
    context_.print_search_debug("start search with max formula size: {}",
                                context_.current_max_size_);
    context_.maxSizeReached = false;
    bool result = forward_synthesis_();
    return result;
  } catch (max_formula_size_reached &e) {
    context_.logger.info(e.what());
  }
  context_.maxSizeReached = true;
  context_.logger.info(
      "Start search with full state propositional equivalence");
  bool result = forward_synthesis_();
  return result;
}

bool ForwardSynthesis::forward_synthesis_() {

  context_.logger.info("Check zero-step realizability");
  if (eval(*context_.nnf_formula)) {
    context_.logger.info("Zero-step realizability check successful");
    return true;
  }

  context_.logger.info("Check one-step realizability");
  auto rel_result = one_step_realizability(*context_.nnf_formula, context_);
  if (rel_result) {
    context_.logger.info("One-step realizability check successful");
    return true;
  }
  context_.logger.info("Check one-step unrealizability");
  auto is_unrealizable =
      one_step_unrealizability(*context_.nnf_formula, context_);
  if (is_unrealizable) {
    context_.logger.info("One-step unrealizability check successful");
    return false;
  }

  context_.logger.info("Starting the search...");

  //  auto root_sdd_node = to_sdd(*context_.xnf_formula, context_);
  //  auto bdd_formula_id = sdd_id(root_sdd_node);
  context_.logger.info("Starting first system move...");
  auto is_realizable = system_move_(context_.xnf_formula);
  //  bool result = strategy[bdd_formula_id] !=
  //  sdd_manager_false(context_.manager);
  context_.logger.info("Explored states: {}",
                       context_.statistics_.nb_visited_nodes());
  //  return result;
  return is_realizable;
}

std::map<std::string, size_t> ForwardSynthesis::compute_prop_to_id_map(
    const Closure &closure, const InputOutputPartition &partition) {
  std::map<std::string, size_t> result;
  size_t offset = closure.nb_formulas();
  size_t i = offset;
  for (const auto &p : partition.input_variables) {
    result[p] = i++;
  }
  for (const auto &p : partition.output_variables) {
    result[p] = i++;
  }
  return result;
}
bool ForwardSynthesis::system_move_(const logic::ltlf_ptr &formula) {
  context_.indentation += 1;

  //  context_.print_search_debug("Formula: {}", logic::to_string(*formula));

  size_t bdd_formula_id;
  if (context_.maxSizeReached) {
    auto bdd = to_bdd(*formula, context_);
    bdd_formula_id = get_bdd_id(bdd);
  } else {
    bdd_formula_id = (size_t)formula.get();
    // check formula is too large
    auto formulaSize = logic::size(*formula);
    context_.print_search_debug("Formula size of {} is {}", bdd_formula_id,
                                formulaSize);
    if (logic::size(*formula) > context_.current_max_size_) {
      context_.maxSizeReached = true;
      context_.print_search_debug("Formula size is {} which is greater than "
                                  "currently tolerated size {}",
                                  formulaSize, context_.current_max_size_);
      context_.indentation -= 1;
      throw max_formula_size_reached(
          "Formula size is " + std::to_string(formulaSize) +
          " which is greater than currently tolerated size " +
          std::to_string(context_.current_max_size_));
    }
  }

  context_.statistics_.visit_node(bdd_formula_id);
  context_.print_search_debug("explored states: {}",
                              context_.statistics_.nb_visited_nodes());
  context_.print_search_debug("visit node {}", bdd_formula_id);

  context_.print_search_debug("State {}", bdd_formula_id);
  if (context_.discovered.find(bdd_formula_id) != context_.discovered.end()) {
    context_.indentation -= 1;
    bool is_success = context_.discovered[bdd_formula_id];
    if (is_success) {
      context_.print_search_debug("{} already discovered, success",
                                  bdd_formula_id);
      return true;
    } else {
      context_.print_search_debug("{} already discovered, failure",
                                  bdd_formula_id);
      return false;
    }
  }

  if (context_.path.contains(bdd_formula_id)) {
    context_.print_search_debug("Loop detected for node {}, tagging the node",
                                bdd_formula_id);
    context_.loop_tags.insert(bdd_formula_id);
    context_.discovered[bdd_formula_id] = false;
    context_.indentation -= 1;
    return false;
  }

  if (eval(*formula)) {
    context_.print_search_debug("{} accepting!", bdd_formula_id);
    context_.discovered[bdd_formula_id] = true;
    context_.indentation -= 1;
    return true;
  }

  auto one_step_realizability_result =
      one_step_realizability(*formula, context_);
  if (one_step_realizability_result) {
    context_.print_search_debug(
        "One-step realizability success for node {}: SUCCESS", bdd_formula_id);
    context_.discovered[bdd_formula_id] = true;
    context_.indentation -= 1;
    return true;
  }
  auto is_unrealizable = one_step_unrealizability(*formula, context_);
  if (is_unrealizable) {
    context_.print_search_debug(
        "One-step unrealizability success for node {}: FAILURE",
        bdd_formula_id);
    context_.discovered[bdd_formula_id] = false;
    context_.indentation -= 1;
    return false;
  }

  context_.path.push(bdd_formula_id);
  logic::pl_ptr pl_formula = to_pl(*formula);
  std::stack<std::pair<std::string, VarValues>> system_move_stack;
  auto result = find_system_move(bdd_formula_id, pl_formula, system_move_stack);
  if (result) {
    context_.print_search_debug("found winning strategy at state {}",
                                bdd_formula_id);
    context_.print_search_debug("updating strategy: {} -> {}", bdd_formula_id,
                                move_stack_to_string(system_move_stack));
    context_.strategy.add_move_from_stack(bdd_formula_id, system_move_stack);
  } else {
    context_.print_search_debug("NOT found winning strategy at state {}",
                                bdd_formula_id);
  }
  context_.discovered[bdd_formula_id] = result;
  context_.indentation -= 1;
  context_.path.pop();
  return result;
}

bool ForwardSynthesis::find_system_move(
    const size_t &formula_id, const logic::pl_ptr &pl_formula,
    std::stack<std::pair<std::string, VarValues>> &partial_system_move) {
  bool result;
  auto allVars = logic::find_atoms(*pl_formula);
  std::vector<logic::ast_ptr> controllableVars;
  controllableVars.reserve(context_.partition.output_variables.size());
  for (const auto &atom : allVars) {
    if (std::find(
            context_.partition.output_variables.begin(),
            context_.partition.output_variables.end(),
            std::static_pointer_cast<const logic::StringSymbol>(atom)->name) !=
        context_.partition.output_variables.end()) {
      controllableVars.push_back(atom);
    }
  }

  if (controllableVars.empty()) {
    // system choice is irrelevant
    context_.print_search_debug("no controllable variables -> find env move");
    context_.indentation += 1;
    result = find_env_move_(pl_formula);
    context_.indentation -= 1;
    return result;
  }
  // pick first variable, and try to set it to 'true'
  auto symbol = controllableVars[0];
  std::string varname =
      std::static_pointer_cast<const logic::StringSymbol>(symbol)->name;
  context_.print_search_debug("branch on system variable {} (true)", varname);

  auto v = (bool)rand() % 2;
  auto pl_formula_true = logic::replace({{symbol, v}}, *pl_formula);
  partial_system_move.emplace(varname, VarValues::TRUE);
  result = find_system_move(formula_id, pl_formula_true, partial_system_move);
  if (result) {
    context_.print_search_debug("branch on system variable {} (true) SUCCESS",
                                varname);
    return true;
  }
  partial_system_move.pop();

  // set variable to 'false'
  context_.print_search_debug("branch on system variable {} (true) FAILURE",
                              varname);
  context_.print_search_debug("branch on system variable {} (false)", varname);
  auto pl_formula_false = logic::replace({{symbol, not v}}, *pl_formula);
  partial_system_move.emplace(varname, VarValues::FALSE);
  result = find_system_move(formula_id, pl_formula_false, partial_system_move);
  if (result) {
    context_.print_search_debug("branch on system variable {} (false) SUCCESS",
                                varname);
    return true;
  }
  partial_system_move.pop();
  context_.print_search_debug("branch on system variable {} (false) FAILURE",
                              varname);
  return false;
}

bool ForwardSynthesis::find_env_move_(const logic::pl_ptr &pl_formula) {
  bool result;
  auto allVars = logic::find_atoms(*pl_formula);
  std::vector<logic::ast_ptr> envVars;
  envVars.reserve(context_.partition.input_variables.size());
  for (const auto &atom : allVars) {
    if (std::find(
            context_.partition.input_variables.begin(),
            context_.partition.input_variables.end(),
            std::static_pointer_cast<const logic::StringSymbol>(atom)->name) !=
        context_.partition.input_variables.end()) {
      envVars.push_back(atom);
    }
  }

  if (envVars.empty()) {
    // env choice is irrelevant -> go to next state
    context_.print_search_debug(
        "no uncontrollable variables -> find next system move");
    auto next_formula = next_state_formula_(pl_formula);
    context_.indentation += 1;
    result = system_move_(next_formula);
    context_.indentation -= 1;
    return result;
  }

  // pick first variable, and try to set it to 'true'
  auto symbol = envVars[0];
  auto varname =
      std::static_pointer_cast<const logic::StringSymbol>(symbol)->name;
  context_.print_search_debug("branch on env variable {} (true)", varname);

  auto pl_formula_true = logic::replace({{symbol, true}}, *pl_formula);
  result = find_env_move_(pl_formula_true);
  if (!result) {
    context_.print_search_debug("branch on env variable {} (true) FAILURE",
                                varname);
    return false;
  }
  //        // if child fails, return
  //        if (strategy_true.empty()){
  //            return strategy_t{};
  //        }
  //
  //        final_strategy.insert(strategy_true.begin(), strategy_true.end());

  // if not, try the other env move
  // set variable to 'false'
  context_.print_search_debug("branch on env variable {} (true) SUCCESS",
                              varname);
  context_.print_search_debug("branch on env variable {} (false)", varname);
  auto pl_formula_false = logic::replace({{symbol, false}}, *pl_formula);
  result = find_env_move_(pl_formula_false);
  if (!result) {
    context_.print_search_debug("branch on env variable {} (false) FAILURE",
                                varname);
    return false;
  }
  context_.print_search_debug("branch on env variable {} (false) SUCCESS",
                              varname);
  return true;
}

logic::ltlf_ptr
ForwardSynthesis::next_state_formula_(const logic::pl_ptr &pl_formula) {
  auto formula = to_ltlf(*pl_formula);
  auto next_state_formula = xnf(*strip_next(*formula));
  return next_state_formula;
}

ForwardSynthesis::Context::Context(const logic::ltlf_ptr &formula,
                                   const InputOutputPartition &partition,
                                   bool use_gc, float gc_threshold)
    : logger{"nike"}, formula{formula}, partition{partition}, use_gc{use_gc},
      gc_threshold{gc_threshold},
      ast_manager{&formula->ctx()}, strategy{partition.output_variables} {
  nnf_formula = logic::to_nnf(*formula);
  xnf_formula = xnf(*nnf_formula);
  current_max_size_ = logic::size(*xnf_formula) * 3;
  Closure closure_object = closure(*xnf_formula);
  closure_ = closure_object;
  manager_ = CUDD::Cudd(0, closure_.nb_formulas());
  manager_.AutodynEnable();
  prop_to_id = compute_prop_to_id_map(closure_, partition);
  statistics_ = Statistics();
  initialie_maps_();
}
void ForwardSynthesis::Context::initialie_maps_() {
  const auto nb_variables = closure_.nb_formulas() + closure_.nb_atoms();
  controllable_map =
      std::vector<int>(closure_.nb_formulas() + closure_.nb_atoms());
  uncontrollable_map =
      std::vector<int>(closure_.nb_formulas() + closure_.nb_atoms());

  auto controllable_offset = closure_.nb_formulas();
  auto uncontrollable_offset =
      closure_.nb_formulas() + partition.input_variables.size();
  for (int i = 0; i < nb_variables; ++i) {
    if (i < controllable_offset) {
      controllable_map[i] = 0;
      uncontrollable_map[i] = 0;
    } else if (i >= controllable_offset and i < uncontrollable_offset) {
      controllable_map[i] = 1;
      uncontrollable_map[i] = 0;
    } else {
      // uncontrollable
      controllable_map[i] = 0;
      uncontrollable_map[i] = 1;
    }
  }
}
} // namespace core
} // namespace nike
