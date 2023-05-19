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

#include <cassert>
#include <vector>
#include "nike/one_step_realizability/bdd.hpp"
#include "nike/logic/print.hpp"


namespace nike {
namespace core {

void BddOneStepRealizabilityVisitor::visit(const logic::LTLfTrue &formula) {
  result = manager.bddOne();
}
void BddOneStepRealizabilityVisitor::visit(const logic::LTLfFalse &formula) {
  result = manager.bddZero();
}
void BddOneStepRealizabilityVisitor::visit(const logic::LTLfPropTrue &formula) {
  result = manager.bddOne();
}
void BddOneStepRealizabilityVisitor::visit(const logic::LTLfPropFalse &formula) {
  result = manager.bddZero();
}
void BddOneStepRealizabilityVisitor::visit(const logic::LTLfAtom &formula) {
  bool controllable = false;
  assert(logic::is_a<const logic::StringSymbol>(*formula.symbol));
  auto prop =
      std::static_pointer_cast<const logic::StringSymbol>(formula.symbol)
          ->name;
  if (std::find(partition.output_variables.begin(),
                partition.output_variables.end(),
                prop) != partition.output_variables.end()) {
    controllable = true;
  }

  auto varId = propToId.find(formula.shared_from_this());
  if (varId == propToId.end()) {
    result = manager.bddVar();
    propToId[formula.shared_from_this()] = propToId.size();
    variableNames.push_back(prop);
    isVariableControllable.push_back(controllable);
  } else {
    result = manager.bddVar(varId->second);
  }

  if (!controllable) {
    uncontrollablesConj = uncontrollablesConj & result;
  }
  else {
    controllablesConj = controllablesConj & result;
  }
}
void BddOneStepRealizabilityVisitor::visit(const logic::LTLfNot &formula) {
  logic::throw_expected_nnf();
}
void BddOneStepRealizabilityVisitor::visit(
    const logic::LTLfPropositionalNot &formula) {
  result = !apply(*formula.get_atom());
}
void BddOneStepRealizabilityVisitor::visit(const logic::LTLfAnd &formula) {
  CUDD::BDD finalResult = manager.bddOne();
  for (const auto &subf : formula.args) {
    finalResult = finalResult & apply(*subf);
    if (finalResult.IsZero()) {
      break;
    }
  }
  result = finalResult;
}
void BddOneStepRealizabilityVisitor::visit(const logic::LTLfOr &formula) {
  CUDD::BDD finalResult = manager.bddZero();
  for (const auto &subf : formula.args) {
    finalResult = finalResult | apply(*subf);
    if (finalResult.IsOne()) {
      break;
    }
  }
  result = finalResult;
}
void BddOneStepRealizabilityVisitor::visit(const logic::LTLfImplies &formula) {
  logic::throw_expected_nnf();
}
void BddOneStepRealizabilityVisitor::visit(const logic::LTLfEquivalent &formula) {
  logic::throw_expected_nnf();
}
void BddOneStepRealizabilityVisitor::visit(const logic::LTLfXor &formula) {
  logic::throw_expected_nnf();
}
void BddOneStepRealizabilityVisitor::visit(const logic::LTLfNext &formula) {
  result = manager.bddZero();
}
void BddOneStepRealizabilityVisitor::visit(const logic::LTLfWeakNext &formula) {
  result = manager.bddOne();
}
void BddOneStepRealizabilityVisitor::visit(const logic::LTLfUntil &formula) {
  result = apply(**formula.args.rbegin());
}
void BddOneStepRealizabilityVisitor::visit(const logic::LTLfRelease &formula) {
  result = apply(**formula.args.rbegin());
}
void BddOneStepRealizabilityVisitor::visit(const logic::LTLfEventually &formula) {
  result = apply(*formula.arg);
}
void BddOneStepRealizabilityVisitor::visit(const logic::LTLfAlways &formula) {
  result = apply(*formula.arg);
}

CUDD::BDD BddOneStepRealizabilityVisitor::apply(const logic::LTLfFormula &f) {
  f.accept(*this);
  return result;
}

std::optional<move_t> BddOneStepRealizabilityChecker::one_step_realizable(const logic::LTLfFormula &f, const InputOutputPartition& partition) {
  auto visitor = BddOneStepRealizabilityVisitor{partition};
  auto result = visitor.apply(f);

  if (result.IsZero()) {
    return std::nullopt;
  }
  if (result.IsOne()) {
    return move_t{};
  }

  auto univQuantified = result.UnivAbstract(visitor.uncontrollablesConj);
  auto existQuantified = univQuantified.ExistAbstract(visitor.controllablesConj);


  std::vector<CUDD::BDD> outputs{result, visitor.uncontrollablesConj, visitor.controllablesConj, existQuantified, univQuantified};
  std::vector<std::string> inames = visitor.variableNames;
  std::vector<std::string> onames = {"result", "uncontrollablesConj", "controllablesConj", "existQuantified", "univQuantified"};

  std::set<std::string> processed;
  if (existQuantified.IsOne()) {
    // realizable
    move_t move{};
    char assignment[visitor.propToId.size()];
    univQuantified.PickOneCube(assignment); // get an assignment


    std::string varName;
    for (int i = 0; i < partition.output_variables.size(); ++i){
      varName = partition.output_variables[i];
      auto it = std::find(visitor.variableNames.begin(), visitor.variableNames.end(), varName);
      if(it == visitor.variableNames.end()) {
        move.emplace_back(varName, VarValues::DONT_CARE);
      }
      else{
        auto propId = std::distance(visitor.variableNames.begin(), it);
        if (!visitor.isVariableControllable[propId]){
          // not controllable; skip
          continue;
        }
        varName = visitor.variableNames[propId];
        auto value = assignment[propId] == 0 ? VarValues::FALSE : (assignment[propId] == 1? VarValues::TRUE : VarValues::DONT_CARE);
        move.emplace_back(varName, value);
        processed.insert(varName);
      }
    }


    return move;
  }

  return std::nullopt;
}


} // namespace core
} // namespace nike
