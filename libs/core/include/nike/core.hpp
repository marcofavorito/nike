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

#include <nike/closure.hpp>
#include <nike/graph.hpp>
#include <nike/input_output_partition.hpp>
#include <nike/logger.hpp>
#include <nike/logic/types.hpp>
#include <nike/path.hpp>
#include <nike/statistics.hpp>

namespace nike {
namespace core {

typedef std::map<unsigned long long, void *> strategy_t;

class ISynthesis {
public:
  const logic::ltlf_ptr formula;
  const InputOutputPartition partition;
  ISynthesis(const logic::ltlf_ptr &formula,
             const InputOutputPartition &partition);
  virtual bool is_realizable() = 0;
};

template <class Synthesis,
          typename = typename std::enable_if<
              std::is_base_of<ISynthesis, Synthesis>::value>::type,
          typename... Args>
bool is_realizable(const logic::ltlf_ptr &formula,
                   const InputOutputPartition &partition, const Args &...args) {
  auto synthesis = Synthesis(formula, partition, args...);
  return synthesis.is_realizable();
}

class ForwardSynthesis : public ISynthesis {
public:
  class Context {
  public:
    logic::ltlf_ptr formula;
    logic::Context *ast_manager;
    InputOutputPartition partition;
    logic::ltlf_ptr nnf_formula;
    logic::ltlf_ptr xnf_formula;
    Closure closure_;
    Statistics statistics_;
    Graph graph;
    std::map<std::string, size_t> prop_to_id;
    std::map<unsigned long long, bool> discovered;
    std::set<unsigned long long> loop_tags;
    std::map<unsigned long long, void *> winning_moves;
    std::map<unsigned long long, logic::ltlf_ptr> sdd_node_id_to_formula;
    std::map<logic::ltlf_ptr, void *> formula_to_sdd_node;
    utils::Logger logger;
    size_t indentation = 0;
    const bool use_gc;
    const float gc_threshold;
    std::vector<int> controllable_map;
    std::vector<int> uncontrollable_map;
    Context(const logic::ltlf_ptr &formula,
            const InputOutputPartition &partition, bool use_gc = false,
            float gc_threshold = 0.95);
    ~Context() {}

    logic::ltlf_ptr get_formula(size_t index) const;
    void call_gc_vtree() const;
    template <typename Arg1, typename... Args>
    inline void print_search_debug(const char *fmt, const Arg1 &arg1,
                                   const Args &...args) const {
      logger.debug((std::string(indentation, '\t') + fmt).c_str(), arg1,
                   args...);
    };
    inline void print_search_debug(const char *fmt) const {
      logger.debug((std::string(indentation, '\t') + fmt).c_str());
    };

    void initialie_maps_();
  };
  ForwardSynthesis(const logic::ltlf_ptr &formula,
                   const InputOutputPartition &partition,
                   bool enable_gc = false)
      : context_{formula, partition, enable_gc},
        ISynthesis(formula, partition){};

  static std::map<std::string, size_t>
  compute_prop_to_id_map(const Closure &closure,
                         const InputOutputPartition &partition);
  bool is_realizable() override;

  bool forward_synthesis_();

private:
  Context context_;
  //  strategy_t system_move_(const logic::ltlf_ptr& formula, Path& path);
  //  strategy_t env_move_(SddNodeWrapper& wrapper, Path& path);
  //  void backprop_success(SddNodeWrapper& wrapper, strategy_t& strategy);
  //  SddNodeWrapper next_state_(const SddNodeWrapper& wrapper);
  //  logic::ltlf_ptr next_state_formula_(void* wrapper);
  //  SddNodeWrapper formula_to_sdd_(const logic::ltlf_ptr& formula);
  //  static NodeType node_type_from_sdd_type_(const SddNodeWrapper& wrapper);
  //  void add_transition_(const SddNodeWrapper& start, void* move_node,
  //                       const SddNodeWrapper& end);
};

} // namespace core
} // namespace nike
