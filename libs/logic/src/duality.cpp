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

#include <nike/logic/duality.hpp>
#include <nike/logic/nnf.hpp>
#include <nike/logic/utils.hpp>

namespace nike {
namespace logic {

void NegationTransformer::visit(const LTLfTrue &formula) {
  result = formula.ctx().make_ff();
}
void NegationTransformer::visit(const LTLfFalse &formula) {
  result = formula.ctx().make_tt();
}
void NegationTransformer::visit(const LTLfPropTrue &formula) {
  //  nnf(~true) = end
  result = formula.ctx().make_end();
}
void NegationTransformer::visit(const LTLfPropFalse &formula) {
  //  nnf(~false) = tt
  result = formula.ctx().make_tt();
}
void NegationTransformer::visit(const LTLfAtom &formula) {
  //  nnf(~a) = !a | end
  auto &context = formula.ctx();
  auto prop_not_atom = context.make_prop_not(
      std::static_pointer_cast<const LTLfFormula>(formula.shared_from_this()));
  auto end = context.make_end();
  result = context.make_or(vec_ptr{prop_not_atom, end});
}
void NegationTransformer::visit(const LTLfNot &formula) {
  // nnf(~~f) = nnf(f)
  result = to_nnf(*formula.arg);
}
void NegationTransformer::visit(const LTLfPropositionalNot &formula) {
  //  nnf(~!a) = a | end
  auto &context = formula.ctx();
  auto prop_negation = std::static_pointer_cast<const LTLfPropositionalNot>(
      formula.shared_from_this());
  auto atom = prop_negation->arg;
  auto end = context.make_end();
  result = context.make_or(vec_ptr{atom, end});
}
void NegationTransformer::visit(const LTLfAnd &formula) {
  result = forward_call_to_arguments(
      formula, [this](const ltlf_ptr &formula) { return apply(*formula); },
      [formula](const vec_ptr &container) {
        return formula.ctx().make_or(container);
      });
}
void NegationTransformer::visit(const LTLfOr &formula) {
  result = forward_call_to_arguments(
      formula, [this](const ltlf_ptr &formula) { return apply(*formula); },
      [formula](const vec_ptr &container) {
        return formula.ctx().make_and(container);
      });
}
void NegationTransformer::visit(const LTLfImplies &formula) {
  auto simplified_formula = simplify(formula);
  result = apply(*simplified_formula);
}
void NegationTransformer::visit(const LTLfEquivalent &formula) {
  auto simplified_formula = simplify(formula);
  result = apply(*simplified_formula);
}
void NegationTransformer::visit(const LTLfXor &formula) {
  auto simplified_formula = simplify(formula);
  result = apply(*simplified_formula);
}
void NegationTransformer::visit(const LTLfNext &formula) {
  result = formula.ctx().make_weak_next(apply(*formula.arg));
}
void NegationTransformer::visit(const LTLfWeakNext &formula) {
  result = formula.ctx().make_next(apply(*formula.arg));
}
void NegationTransformer::visit(const LTLfUntil &formula) {
  result = forward_call_to_arguments(
      formula, [this](const ltlf_ptr &formula) { return apply(*formula); },
      [formula](const vec_ptr &container) {
        return formula.ctx().make_release(container);
      });
}
void NegationTransformer::visit(const LTLfRelease &formula) {
  result = forward_call_to_arguments(
      formula, [this](const ltlf_ptr &formula) { return apply(*formula); },
      [formula](const vec_ptr &container) {
        return formula.ctx().make_until(container);
      });
}
void NegationTransformer::visit(const LTLfEventually &formula) {
  result = formula.ctx().make_always(apply(*formula.arg));
}
void NegationTransformer::visit(const LTLfAlways &formula) {
  result = formula.ctx().make_eventually(apply(*formula.arg));
}

ltlf_ptr NegationTransformer::apply(const LTLfFormula &f) {
  f.accept(*this);
  return result;
}

ltlf_ptr apply_negation(const LTLfFormula &f) {
  auto visitor = NegationTransformer{};
  return visitor.apply(f);
}

} // namespace logic
} // namespace nike