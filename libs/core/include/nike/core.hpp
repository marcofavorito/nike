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
#include <nike/closure.hpp>
#include <nike/core_base.hpp>
#include <nike/graph.hpp>
#include <nike/input_output_partition.hpp>
#include <nike/logger.hpp>
#include <nike/logic/types.hpp>
#include <nike/path.hpp>
#include <nike/statistics.hpp>
#include <nike/strategy.hpp>
#include <utility>

namespace nike {
namespace core {

class max_formula_size_reached : public std::logic_error {
public:
  explicit max_formula_size_reached(const std::string &reason)
      : std::logic_error(reason) {}
};
class interrupted_exception : public std::exception {
public:
  explicit interrupted_exception() : std::exception() {}
};

class ForwardSynthesis : public ISynthesis {
public:
  class Context {
  public:
    logic::ltlf_ptr formula;
    InputOutputPartition partition;
    logic::Context *ast_manager;
    logic::ltlf_ptr nnf_formula;
    logic::ltlf_ptr xnf_formula;
    Closure closure_;
    CUDD::Cudd manager_;
    Statistics statistics_;
    Graph graph;
    Strategy strategy;
    Path path;
    std::map<std::string, size_t> prop_to_id;
    std::map<size_t, bool> discovered;
    std::set<long> loop_tags;
    std::map<long, logic::ltlf_ptr> sdd_node_id_to_formula;
    std::map<logic::ltlf_ptr, CUDD::BDD> formula_to_bdd_node;
    utils::Logger logger;
    size_t indentation = 0;
    std::vector<int> controllable_map;
    std::vector<int> uncontrollable_map;
    size_t current_max_size_;
    std::unique_ptr<BranchVariable> branch_variable;
    StateEquivalenceMode mode;
    BranchingStrategy bs;
    bool stopped = false;
    bool disable_one_step_realizability = false;
    bool disable_one_step_unrealizability = false;
    Context(const logic::ltlf_ptr &formula,
            const InputOutputPartition &partition, BranchingStrategy bs,
            StateEquivalenceMode mode, double max_size_factor = 3.0,
            std::string logger_section_name = "nike",
            bool disable_one_step_realizability = false,
            bool disable_one_step_unrealizability = false);
    ~Context() = default;

    template <typename Arg1, typename... Args>
    inline void print_search_debug(const char *fmt, const Arg1 &arg1,
                                   const Args &...args) const {
      logger.debug((std::string(indentation, ' ') + fmt).c_str(), arg1,
                   args...);
    };
    inline void print_search_debug(const char *fmt) const {
      logger.debug((std::string(indentation, ' ') + fmt).c_str());
    };

    void initialie_maps_();
    void reset();
  };
  ForwardSynthesis(const logic::ltlf_ptr &formula,
                   const InputOutputPartition &partition,
                   BranchingStrategy bs = BranchingStrategy::RANDOM,
                   StateEquivalenceMode mode = StateEquivalenceMode::HASH,
                   std::string logger_section_name = "nike",
                   bool disable_one_step_realizability = false,
                   bool disable_one_step_unrealizability = false,
                   double max_size_factor = 3.0)
      : ISynthesis(formula, partition),
        context_{formula, partition,       bs,
                 mode,    max_size_factor, std::move(logger_section_name), disable_one_step_realizability, disable_one_step_unrealizability} {};
  bool is_realizable() override;
  void register_termination_callback(DD_THFP callback,
                                     void *callback_arg) const;
  void stop();
  bool is_stopped() const;

private:
  Context context_;
  size_t get_state_id(const logic::ltlf_ptr &formula);

  static std::map<std::string, size_t>
  compute_prop_to_id_map(const Closure &closure,
                         const InputOutputPartition &partition);
  long get_bdd_id(CUDD::BDD node) {
    return reinterpret_cast<std::intptr_t>(node.getNode());
  }

  inline void check_stopped();
  bool forward_synthesis_();
  bool ids_forward_synthesis_();
  bool system_move_(const logic::ltlf_ptr &formula);
  bool env_move_(const logic::pl_ptr &pl_formula);
  bool find_env_move_(const logic::pl_ptr &pl_formula);
  void backprop_success(size_t &node_id, NodeType node_type);
  logic::ltlf_ptr next_state_formula_(const logic::pl_ptr &pl_formula);
  bool find_system_move(
      const size_t &formula_id, const logic::pl_ptr &pl_formula,
      std::stack<std::pair<std::string, VarValues>> &partial_system_move);
};

} // namespace core
} // namespace nike
