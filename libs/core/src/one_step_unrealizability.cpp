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
#include <nike/one_step_unrealizability.hpp>

namespace nike {
namespace core {

void OneStepUnrealizabilityVisitor::visit(const logic::LTLfTrue &formula) {
  result = manager.bddOne();
}
void OneStepUnrealizabilityVisitor::visit(const logic::LTLfFalse &formula) {
  result = manager.bddZero();
}
void OneStepUnrealizabilityVisitor::visit(const logic::LTLfPropTrue &formula) {
  result = manager.bddOne();
}
void OneStepUnrealizabilityVisitor::visit(const logic::LTLfPropFalse &formula) {
  result = manager.bddZero();
}
void OneStepUnrealizabilityVisitor::visit(const logic::LTLfAtom &formula) {
  bool controllable = false;
  if (logic::is_a<const logic::StringSymbol>(*formula.symbol)) {
    auto prop =
        std::static_pointer_cast<const logic::StringSymbol>(formula.symbol)
            ->name;
    if (std::find(context_.partition.output_variables.begin(),
                  context_.partition.output_variables.end(),
                  prop) != context_.partition.output_variables.end()) {
      controllable = true;
    }
  }

  auto varId = propToId.find(formula.shared_from_this());
  if (varId == propToId.end()) {
    result = manager.bddVar();
    propToId[formula.shared_from_this()] = propToId.size();
  } else {
    result = manager.bddVar(varId->second);
  }

  if (controllable) {
    controllablesConj = controllablesConj & result;
  }
}
void OneStepUnrealizabilityVisitor::visit(const logic::LTLfNot &formula) {
  logic::throw_expected_nnf();
}
void OneStepUnrealizabilityVisitor::visit(
    const logic::LTLfPropositionalNot &formula) {
  result = !apply(*formula.get_atom());
}
void OneStepUnrealizabilityVisitor::visit(const logic::LTLfAnd &formula) {
  CUDD::BDD finalResult = manager.bddOne();
  for (const auto &subf : formula.args) {
    finalResult = finalResult & apply(*subf);
  }
  result = finalResult;
}
void OneStepUnrealizabilityVisitor::visit(const logic::LTLfOr &formula) {
  CUDD::BDD finalResult = manager.bddZero();
  for (const auto &subf : formula.args) {
    finalResult = finalResult | apply(*subf);
  }
  result = finalResult;
}
void OneStepUnrealizabilityVisitor::visit(const logic::LTLfImplies &formula) {
  logic::throw_expected_nnf();
}
void OneStepUnrealizabilityVisitor::visit(
    const logic::LTLfEquivalent &formula) {
  logic::throw_expected_nnf();
}
void OneStepUnrealizabilityVisitor::visit(const logic::LTLfXor &formula) {
  logic::throw_expected_nnf();
}
void OneStepUnrealizabilityVisitor::visit(const logic::LTLfNext &formula) {
  result = manager.bddOne();
}
void OneStepUnrealizabilityVisitor::visit(const logic::LTLfWeakNext &formula) {
  result = manager.bddOne();
}
void OneStepUnrealizabilityVisitor::visit(const logic::LTLfUntil &formula) {
  CUDD::BDD finalResult = manager.bddZero();
  for (const auto &subf : formula.args) {
    finalResult = finalResult | apply(*subf);
  }
  result = finalResult;
}
void OneStepUnrealizabilityVisitor::visit(const logic::LTLfRelease &formula) {
  result = apply(**formula.args.rbegin());
}
void OneStepUnrealizabilityVisitor::visit(
    const logic::LTLfEventually &formula) {
  result = manager.bddOne();
}
void OneStepUnrealizabilityVisitor::visit(const logic::LTLfAlways &formula) {
  result = apply(*formula.arg);
}

CUDD::BDD OneStepUnrealizabilityVisitor::apply(const logic::LTLfFormula &f) {
  f.accept(*this);
  return result;
}

bool one_step_unrealizability(const logic::LTLfFormula &f,
                              ForwardSynthesis::Context &context) {
  auto visitor = OneStepUnrealizabilityVisitor{context};
  auto result = visitor.apply(f);

  if (result.IsZero()) {
    return true;
  }
  if (result.IsOne()) {
    return false;
  }

  auto varToQuantify = visitor.controllablesConj;
  auto quantified = result.ExistAbstract(visitor.controllablesConj);

  if (quantified.IsOne()) {
    return false;
  }

  return true;
}

} // namespace core
} // namespace nike
