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

#include <nike/core.hpp>
#include <nike/logic/utils.hpp>
#include <nike/to_bdd.hpp>

namespace nike {
namespace core {

void ToBddVisitor::visit(const logic::LTLfTrue &formula) {
  result = get_bdd_var(formula);
}
void ToBddVisitor::visit(const logic::LTLfFalse &formula) {
  result = get_bdd_var(formula);
}
void ToBddVisitor::visit(const logic::LTLfPropTrue &formula) {
  auto not_end = formula.ctx().make_not_end();
  result = get_bdd_var(*not_end);
}
void ToBddVisitor::visit(const logic::LTLfPropFalse &formula) {
  result = context_.manager_.bddZero();
}
void ToBddVisitor::visit(const logic::LTLfAtom &formula) {
  result = get_bdd_var(formula);
}
void ToBddVisitor::visit(const logic::LTLfNot &formula) {
  logic::throw_expected_nnf();
}
void ToBddVisitor::visit(const logic::LTLfPropositionalNot &formula) {
  result = get_bdd_var(formula);
}
void ToBddVisitor::visit(const logic::LTLfAnd &formula) {
  CUDD::BDD finalResult = context_.manager_.bddOne();
  for (const auto &subf : formula.args) {
    finalResult = finalResult & apply(*subf);
  }
  result = finalResult;
}
void ToBddVisitor::visit(const logic::LTLfOr &formula) {
  CUDD::BDD finalResult = context_.manager_.bddZero();
  for (const auto &subf : formula.args) {
    finalResult = finalResult | apply(*subf);
  }
  result = finalResult;
}
void ToBddVisitor::visit(const logic::LTLfImplies &formula) {
  logic::throw_expected_nnf();
}
void ToBddVisitor::visit(const logic::LTLfEquivalent &formula) {
  logic::throw_expected_nnf();
}
void ToBddVisitor::visit(const logic::LTLfXor &formula) {
  logic::throw_expected_nnf();
}
void ToBddVisitor::visit(const logic::LTLfNext &formula) {
  result = get_bdd_var(formula);
}
void ToBddVisitor::visit(const logic::LTLfWeakNext &formula) {
  result = get_bdd_var(formula);
}
void ToBddVisitor::visit(const logic::LTLfUntil &formula) {
  logic::throw_expected_xnf();
}
void ToBddVisitor::visit(const logic::LTLfRelease &formula) {
  logic::throw_expected_xnf();
}
void ToBddVisitor::visit(const logic::LTLfEventually &formula) {
  auto not_end = formula.ctx().make_not_end();
  if (*not_end == formula) {
    result = get_bdd_var(formula);
    return;
  }
  logic::throw_expected_xnf();
}
void ToBddVisitor::visit(const logic::LTLfAlways &formula) {
  auto end = formula.ctx().make_end();
  if (*end == formula) {
    result = get_bdd_var(formula);
    return;
  }
  logic::throw_expected_xnf();
}

CUDD::BDD ToBddVisitor::apply(const logic::LTLfFormula &formula) {
  auto formula_ptr = std::static_pointer_cast<const logic::LTLfFormula>(
      formula.shared_from_this());
  auto cached_result = context_.formula_to_bdd_node.find(formula_ptr);
  if (cached_result != context_.formula_to_bdd_node.end()) {
    return cached_result->second;
  }
  formula.accept(*this);
  context_.formula_to_bdd_node[formula_ptr] = result;
  return result;
}

CUDD::BDD ToBddVisitor::get_bdd_var(const logic::LTLfFormula &formula) {
  auto varIndex = context_.closure_.get_id(
      std::static_pointer_cast<const logic::LTLfFormula>(
          formula.shared_from_this()));
  return context_.manager_.bddVar();
}

CUDD::BDD to_bdd(const logic::LTLfFormula &formula,
                 ForwardSynthesis::Context &context) {
  ToBddVisitor visitor{context};
  auto result = visitor.apply(formula);
  return result;
}

} // namespace core
} // namespace nike
