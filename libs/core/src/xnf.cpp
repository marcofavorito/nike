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
#include <nike/xnf.hpp>

namespace nike {
namespace core {

void XnfVisitor::visit(const logic::LTLfTrue &formula) {
  result = formula.ctx().make_tt();
}
void XnfVisitor::visit(const logic::LTLfFalse &formula) {
  result = formula.ctx().make_ff();
}
void XnfVisitor::visit(const logic::LTLfPropTrue &formula) {
  result = formula.ctx().make_prop_true();
}
void XnfVisitor::visit(const logic::LTLfPropFalse &formula) {
  result = formula.ctx().make_prop_false();
}
void XnfVisitor::visit(const logic::LTLfAtom &formula) {
  result = std::static_pointer_cast<const logic::LTLfFormula>(
      formula.shared_from_this());
}
void XnfVisitor::visit(const logic::LTLfNot &formula) {
  logic::throw_expected_nnf();
}
void XnfVisitor::visit(const logic::LTLfPropositionalNot &formula) {
  result = std::static_pointer_cast<const logic::LTLfFormula>(
      formula.shared_from_this());
}
void XnfVisitor::visit(const logic::LTLfAnd &formula) {
  result = logic::forward_call_to_arguments(
      formula,
      [this](const logic::ltlf_ptr &formula) { return apply(*formula); },
      [formula](const logic::vec_ptr &container) {
        return formula.ctx().make_and(container);
      });
}
void XnfVisitor::visit(const logic::LTLfOr &formula) {
  result = logic::forward_call_to_arguments(
      formula,
      [this](const logic::ltlf_ptr &formula) { return apply(*formula); },
      [formula](const logic::vec_ptr &container) {
        return formula.ctx().make_or(container);
      });
}
void XnfVisitor::visit(const logic::LTLfImplies &formula) {
  result = logic::forward_call_to_arguments(
      formula,
      [this](const logic::ltlf_ptr &formula) { return apply(*formula); },
      [formula](const logic::vec_ptr &container) {
        return formula.ctx().make_implies(container);
      });
}
void XnfVisitor::visit(const logic::LTLfEquivalent &formula) {
  result = logic::forward_call_to_arguments(
      formula,
      [this](const logic::ltlf_ptr &formula) { return apply(*formula); },
      [formula](const logic::vec_ptr &container) {
        return formula.ctx().make_equivalent(container);
      });
}
void XnfVisitor::visit(const logic::LTLfXor &formula) {
  result = logic::forward_call_to_arguments(
      formula,
      [this](const logic::ltlf_ptr &formula) { return apply(*formula); },
      [formula](const logic::vec_ptr &container) {
        return formula.ctx().make_xor(container);
      });
}
void XnfVisitor::visit(const logic::LTLfNext &formula) {
  result = std::static_pointer_cast<const logic::LTLfFormula>(
      formula.shared_from_this());
  ;
}
void XnfVisitor::visit(const logic::LTLfWeakNext &formula) {
  result = std::static_pointer_cast<const logic::LTLfFormula>(
      formula.shared_from_this());
  ;
}
void XnfVisitor::visit(const logic::LTLfUntil &formula) {
  auto &c = formula.ctx();
  logic::ltlf_ptr head, tail;
  if (formula.args.size() == 2) {
    head = formula.args[0];
    tail = formula.args[1];
  } else {
    head = formula.args[0];
    tail = c.make_until(
        logic::vec_ptr(formula.args.begin() + 1, formula.args.end()));
  }
  auto next_until =
      c.make_next(result = std::static_pointer_cast<const logic::LTLfFormula>(
                      formula.shared_from_this()));
  auto left_part = c.make_and({tail, c.make_not_end()});
  auto right_part = c.make_and({head, next_until});
  auto temp = c.make_or({left_part, right_part});
  result = apply(*temp);
}
void XnfVisitor::visit(const logic::LTLfRelease &formula) {
  auto &c = formula.ctx();
  logic::ltlf_ptr head, tail;
  if (formula.args.size() == 2) {
    head = formula.args[0];
    tail = formula.args[1];
  } else {
    head = formula.args[0];
    tail = c.make_release(
        logic::vec_ptr(formula.args.begin() + 1, formula.args.end()));
  }
  auto wnext_release = c.make_weak_next(
      result = std::static_pointer_cast<const logic::LTLfFormula>(
          formula.shared_from_this()));
  auto left_part = c.make_or({tail, c.make_end()});
  auto right_part = c.make_or({head, wnext_release});
  auto temp = c.make_and({left_part, right_part});
  result = apply(*temp);
}
void XnfVisitor::visit(const logic::LTLfEventually &formula) {
  auto &c = formula.ctx();
  auto not_end = formula.ctx().make_not_end();
  if (*not_end == formula) {
    result = result = std::static_pointer_cast<const logic::LTLfFormula>(
        formula.shared_from_this());
    ;
    return;
  }
  // F(phi), phi != tt
  auto now_part = c.make_and({apply(*formula.arg), not_end});
  auto next_part =
      c.make_next(result = std::static_pointer_cast<const logic::LTLfFormula>(
                      formula.shared_from_this()));
  auto temp = c.make_or({now_part, next_part});
  result = temp;
}
void XnfVisitor::visit(const logic::LTLfAlways &formula) {
  auto &c = formula.ctx();
  auto end = formula.ctx().make_end();
  if (*end == formula) {
    result = result = std::static_pointer_cast<const logic::LTLfFormula>(
        formula.shared_from_this());
    return;
  }
  // G(phi), phi != ff
  auto now_part = c.make_or({apply(*formula.arg), end});
  auto next_part = c.make_weak_next(
      result = std::static_pointer_cast<const logic::LTLfFormula>(
          formula.shared_from_this()));
  auto temp = c.make_and({now_part, next_part});
  result = apply(*temp);
}

logic::ltlf_ptr XnfVisitor::apply(const logic::LTLfFormula &formula) {
  formula.accept(*this);
  return result;
}

logic::ltlf_ptr xnf(const logic::LTLfFormula &formula) {
  XnfVisitor visitor;
  return visitor.apply(formula);
}

} // namespace core
} // namespace nike
