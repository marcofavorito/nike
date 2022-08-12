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

#include <cassert>
#include <nike/one_step_realizability.hpp>

namespace nike {
namespace core {

void OneStepRealizabilityVisitor::visit(const logic::LTLfTrue &formula) {
  result = manager.bddOne();
}
void OneStepRealizabilityVisitor::visit(const logic::LTLfFalse &formula) {
  result = manager.bddZero();
}
void OneStepRealizabilityVisitor::visit(const logic::LTLfPropTrue &formula) {
  result = manager.bddOne();
}
void OneStepRealizabilityVisitor::visit(const logic::LTLfPropFalse &formula) {
  result = manager.bddZero();
}
void OneStepRealizabilityVisitor::visit(const logic::LTLfAtom &formula) {
  bool controllable = false;
  if (logic::is_a<const logic::StringSymbol>(*formula.symbol)) {
    auto prop =
        std::static_pointer_cast<const logic::StringSymbol>(formula.symbol)
            ->name;
    if (context_.prop_to_id.find(prop) != context_.prop_to_id.end()) {
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
void OneStepRealizabilityVisitor::visit(const logic::LTLfNot &formula) {
  logic::throw_expected_nnf();
}
void OneStepRealizabilityVisitor::visit(
    const logic::LTLfPropositionalNot &formula) {
  result = !apply(*formula.get_atom());
}
void OneStepRealizabilityVisitor::visit(const logic::LTLfAnd &formula) {
  CUDD::BDD finalResult = manager.bddOne();
  for (const auto &subf : formula.args) {
    finalResult = finalResult & apply(*subf);
  }
  result = finalResult;
}
void OneStepRealizabilityVisitor::visit(const logic::LTLfOr &formula) {
  CUDD::BDD finalResult = manager.bddZero();
  for (const auto &subf : formula.args) {
    finalResult = finalResult | apply(*subf);
  }
  result = finalResult;
}
void OneStepRealizabilityVisitor::visit(const logic::LTLfImplies &formula) {
  logic::throw_expected_nnf();
}
void OneStepRealizabilityVisitor::visit(const logic::LTLfEquivalent &formula) {
  logic::throw_expected_nnf();
}
void OneStepRealizabilityVisitor::visit(const logic::LTLfXor &formula) {
  logic::throw_expected_nnf();
}
void OneStepRealizabilityVisitor::visit(const logic::LTLfNext &formula) {
  result = manager.bddZero();
}
void OneStepRealizabilityVisitor::visit(const logic::LTLfWeakNext &formula) {
  result = manager.bddOne();
}
void OneStepRealizabilityVisitor::visit(const logic::LTLfUntil &formula) {
  result = apply(**formula.args.rbegin());
}
void OneStepRealizabilityVisitor::visit(const logic::LTLfRelease &formula) {
  result = apply(**formula.args.rbegin());
}
void OneStepRealizabilityVisitor::visit(const logic::LTLfEventually &formula) {
  result = apply(*formula.arg);
}
void OneStepRealizabilityVisitor::visit(const logic::LTLfAlways &formula) {
  result = apply(*formula.arg);
}

CUDD::BDD OneStepRealizabilityVisitor::apply(const logic::LTLfFormula &f) {
  f.accept(*this);
  return result;
}

bool one_step_realizability(const logic::LTLfFormula &f,
                            ForwardSynthesis::Context &context) {
  auto visitor = OneStepRealizabilityVisitor{context};
  auto result = visitor.apply(f);

  if (result.IsZero()) {
    return false;
  }
  if (result.IsOne()) {
    return true;
  }

  //  result.Print

  auto varToQuantify = visitor.controllablesConj;
  auto quantified = result.ExistAbstract(visitor.controllablesConj);

  if (quantified.IsOne()) {
    return true;
  }

  const std::vector<const char *> &inames = {};
  FILE *fp = fopen("temp.dot", "w");
  std::vector<CUDD::BDD> single({quantified});
  visitor.manager.DumpDot(single, inames.data(), nullptr, fp);

  return false;
}

} // namespace core
} // namespace nike
