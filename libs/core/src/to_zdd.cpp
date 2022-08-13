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
#include <nike/to_zdd.hpp>

namespace nike {
namespace core {

void ToZddVisitor::visit(const logic::LTLfTrue &formula) {
  result = get_zdd_var(formula);
}
void ToZddVisitor::visit(const logic::LTLfFalse &formula) {
  result = get_zdd_var(formula);
}
void ToZddVisitor::visit(const logic::LTLfPropTrue &formula) {
  auto not_end = formula.ctx().make_not_end();
  result = get_zdd_var(*not_end);
}
void ToZddVisitor::visit(const logic::LTLfPropFalse &formula) {
  result = context_.manager_.zddZero();
}
void ToZddVisitor::visit(const logic::LTLfAtom &formula) {
  result = get_zdd_var(formula);
}
void ToZddVisitor::visit(const logic::LTLfNot &formula) {
  logic::throw_expected_nnf();
}
void ToZddVisitor::visit(const logic::LTLfPropositionalNot &formula) {
  result = get_zdd_var(formula);
}
void ToZddVisitor::visit(const logic::LTLfAnd &formula) {
  CUDD::ZDD finalResult =
      context_.manager_.zddOne(context_.closure_.nb_formulas() - 1);
  for (const auto &subf : formula.args) {
    finalResult = finalResult & apply(*subf);
  }
  result = finalResult;
}
void ToZddVisitor::visit(const logic::LTLfOr &formula) {
  CUDD::ZDD finalResult = context_.manager_.zddZero();
  for (const auto &subf : formula.args) {
    finalResult = finalResult | apply(*subf);
  }
  result = finalResult;
}
void ToZddVisitor::visit(const logic::LTLfImplies &formula) {
  logic::throw_expected_nnf();
}
void ToZddVisitor::visit(const logic::LTLfEquivalent &formula) {
  logic::throw_expected_nnf();
}
void ToZddVisitor::visit(const logic::LTLfXor &formula) {
  logic::throw_expected_nnf();
}
void ToZddVisitor::visit(const logic::LTLfNext &formula) {
  result = get_zdd_var(formula);
}
void ToZddVisitor::visit(const logic::LTLfWeakNext &formula) {
  result = get_zdd_var(formula);
}
void ToZddVisitor::visit(const logic::LTLfUntil &formula) {
  logic::throw_expected_xnf();
}
void ToZddVisitor::visit(const logic::LTLfRelease &formula) {
  logic::throw_expected_xnf();
}
void ToZddVisitor::visit(const logic::LTLfEventually &formula) {
  auto not_end = formula.ctx().make_not_end();
  if (*not_end == formula) {
    result = get_zdd_var(formula);
    return;
  }
  logic::throw_expected_xnf();
}
void ToZddVisitor::visit(const logic::LTLfAlways &formula) {
  auto end = formula.ctx().make_end();
  if (*end == formula) {
    result = get_zdd_var(formula);
    return;
  }
  logic::throw_expected_xnf();
}

CUDD::ZDD ToZddVisitor::apply(const logic::LTLfFormula &formula) {
  //    auto formula_ptr = std::static_pointer_cast<const
  //    logic::LTLfFormula>(formula.shared_from_this()); auto cached_result =
  //    context_.formula_to_zdd_node.find(formula_ptr); if (cached_result !=
  //    context_.formula_to_zdd_node.end()) {
  //        return cached_result->second;
  //    }
  formula.accept(*this);
  //    context_.formula_to_zdd_node[formula_ptr] = result;
  return result;
}

CUDD::ZDD ToZddVisitor::get_zdd_var(const logic::LTLfFormula &formula) {
  auto varIndex = context_.closure_.get_id(
      std::static_pointer_cast<const logic::LTLfFormula>(
          formula.shared_from_this()));
  return context_.manager_.zddVar(varIndex);
}

CUDD::ZDD to_zdd(const logic::LTLfFormula &formula,
                 ForwardSynthesis::Context &context) {
  ToZddVisitor visitor{context};
  auto result = visitor.apply(formula);
  return result;
}

} // namespace core
} // namespace nike
