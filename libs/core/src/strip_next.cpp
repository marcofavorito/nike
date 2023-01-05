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

#include <nike/logic/utils.hpp>
#include <nike/strip_next.hpp>

namespace nike {
namespace core {
void StripNextVisitor::visit(const logic::LTLfTrue &formula) {
  result = formula.ctx().make_tt();
}
void StripNextVisitor::visit(const logic::LTLfFalse &formula) {
  result = formula.ctx().make_ff();
}
void StripNextVisitor::visit(const logic::LTLfPropTrue &formula) {
  result = formula.ctx().make_prop_true();
}
void StripNextVisitor::visit(const logic::LTLfPropFalse &formula) {
  result = formula.ctx().make_prop_false();
}
void StripNextVisitor::visit(const logic::LTLfAtom &formula) {
  result = std::static_pointer_cast<const logic::LTLfFormula>(
      formula.shared_from_this());
}
void StripNextVisitor::visit(const logic::LTLfNot &formula) {
  logic::throw_expected_nnf();
}
void StripNextVisitor::visit(const logic::LTLfPropositionalNot &formula) {
  result = std::static_pointer_cast<const logic::LTLfFormula>(
      formula.shared_from_this());
  ;
}
void StripNextVisitor::visit(const logic::LTLfAnd &formula) {
  result = logic::forward_call_to_arguments(
      formula,
      [this](const logic::ltlf_ptr &formula) { return apply(*formula); },
      [formula](const logic::vec_ptr &container) {
        return formula.ctx().make_and(container);
      });
}
void StripNextVisitor::visit(const logic::LTLfOr &formula) {
  result = logic::forward_call_to_arguments(
      formula,
      [this](const logic::ltlf_ptr &formula) { return apply(*formula); },
      [formula](const logic::vec_ptr &container) {
        return formula.ctx().make_or(container);
      });
}
void StripNextVisitor::visit(const logic::LTLfImplies &formula) {
  result = logic::forward_call_to_arguments(
      formula,
      [this](const logic::ltlf_ptr &formula) { return apply(*formula); },
      [formula](const logic::vec_ptr &container) {
        return formula.ctx().make_implies(container);
      });
}
void StripNextVisitor::visit(const logic::LTLfEquivalent &formula) {
  result = logic::forward_call_to_arguments(
      formula,
      [this](const logic::ltlf_ptr &formula) { return apply(*formula); },
      [formula](const logic::vec_ptr &container) {
        return formula.ctx().make_equivalent(container);
      });
}
void StripNextVisitor::visit(const logic::LTLfXor &formula) {
  result = logic::forward_call_to_arguments(
      formula,
      [this](const logic::ltlf_ptr &formula) { return apply(*formula); },
      [formula](const logic::vec_ptr &container) {
        return formula.ctx().make_xor(container);
      });
}
void StripNextVisitor::visit(const logic::LTLfNext &formula) {
  result = formula.ctx().make_and({formula.arg, formula.ctx().make_not_end()});
}
void StripNextVisitor::visit(const logic::LTLfWeakNext &formula) {
  result = formula.ctx().make_or({formula.arg, formula.ctx().make_end()});
}
void StripNextVisitor::visit(const logic::LTLfUntil &formula) {
  logic::throw_expected_xnf();
}
void StripNextVisitor::visit(const logic::LTLfRelease &formula) {
  logic::throw_expected_xnf();
}
void StripNextVisitor::visit(const logic::LTLfEventually &formula) {
  auto not_end = formula.ctx().make_not_end();
  if (*not_end == formula) {
    result = formula.ctx().make_tt();
    return;
  }
  logic::throw_expected_xnf();
}
void StripNextVisitor::visit(const logic::LTLfAlways &formula) {
  auto end = formula.ctx().make_end();
  if (*end == formula) {
    result = formula.ctx().make_ff();
    return;
  }
  logic::throw_expected_xnf();
}

logic::ltlf_ptr StripNextVisitor::apply(const logic::LTLfFormula &formula) {
  formula.accept(*this);
  return result;
}

logic::ltlf_ptr strip_next(const logic::LTLfFormula &formula) {
  auto visitor = StripNextVisitor{};
  return visitor.apply(formula);
}

} // namespace core
} // namespace nike
