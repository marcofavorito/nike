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

#include <nike/logic/ltlf.hpp>
#include <nike/logic/visitor.hpp>
#include <vector>

namespace nike {
namespace logic {

bool LTLfTrue::is_equal(const Comparable &o) const { return is_a<LTLfTrue>(o); }
int LTLfTrue::compare_(const Comparable &o) const {
  assert(is_a<LTLfTrue>(o));
  return 0;
}

bool LTLfFalse::is_equal(const Comparable &o) const {
  return is_a<LTLfFalse>(o);
}
int LTLfFalse::compare_(const Comparable &o) const {
  assert(is_a<LTLfFalse>(o));
  return 0;
}

bool LTLfPropTrue::is_equal(const Comparable &o) const {
  return is_a<LTLfPropTrue>(o);
}
int LTLfPropTrue::compare_(const Comparable &o) const {
  assert(is_a<LTLfPropTrue>(o));
  return 0;
}

bool LTLfPropFalse::is_equal(const Comparable &o) const {
  return is_a<LTLfPropFalse>(o);
}
int LTLfPropFalse::compare_(const Comparable &o) const {
  assert(is_a<LTLfPropFalse>(o));
  return 0;
}

bool LTLfAtom::is_equal(const Comparable &o) const {
  return is_a<LTLfAtom>(o) and
         symbol == dynamic_cast<const LTLfAtom &>(o).symbol;
}
int LTLfAtom::compare_(const Comparable &o) const {
  assert(is_a<LTLfAtom>(o));
  auto n1 = this->symbol;
  auto n2 = dynamic_cast<const LTLfAtom &>(o).symbol;
  return n1 == n2 ? 0 : n1 < n2 ? -1 : 1;
}

bool LTLfUnaryOp::is_equal(const Comparable &o) const {
  auto a = get_type_code();
  auto b = o.get_type_code();
  if (a != b) {
    return false;
  }
  auto result = arg->is_equal(*dynamic_cast<const LTLfUnaryOp &>(o).arg);
  return result;
}

int LTLfUnaryOp::compare_(const Comparable &o) const {
  assert(get_type_code() == o.get_type_code());
  return this->arg->compare(*dynamic_cast<const LTLfUnaryOp &>(o).arg);
}

bool LTLfBinaryOp::is_equal(const Comparable &o) const {
  return get_type_code() == o.get_type_code() and
         std::equal(args.begin(), args.end(),
                    dynamic_cast<const LTLfBinaryOp &>(o).args.begin(),
                    utils::EqualOrDeref());
}
int LTLfBinaryOp::compare_(const Comparable &o) const {
  assert(this->get_type_code() == o.get_type_code());
  return utils::ordered_compare(this->args,
                                dynamic_cast<const LTLfBinaryOp &>(o).args);
}

void LTLfCommutativeIdempotentBinaryOp::set_accepts_empty(bool op_x_not_x) {

  auto f = op_x_not_x ? utils::or_ : utils::and_;
  bool global_accept_empty = (*this->args.begin())->metadata().accepts_empty;
  for (auto it = std::next(this->args.begin()); it != this->args.end(); ++it) {
    global_accept_empty =
        f(global_accept_empty, (*it)->metadata().accepts_empty);
  }
  metadata_.accepts_empty = global_accept_empty;
}

ltlf_ptr simplify(const LTLfNot &formula) {
  if (is_a<LTLfTrue>(*formula.arg)) {
    return formula.ctx().make_ff();
  }
  if (is_a<LTLfFalse>(*formula.arg) || is_a<LTLfPropFalse>(*formula.arg)) {
    return formula.ctx().make_tt();
  }
  if (is_a<LTLfPropTrue>(*formula.arg)) {
    return formula.ctx().make_end();
  }
  return std::static_pointer_cast<const LTLfFormula>(
      formula.shared_from_this());
}
ltlf_ptr simplify(const LTLfImplies &formula) {
  auto new_container = vec_ptr(formula.args.size());
  std::transform(
      formula.args.begin(), formula.args.end() - 1, new_container.begin(),
      [](const ltlf_ptr &formula) { return formula->ctx().make_not(formula); });
  new_container[formula.args.size() - 1] = formula.args.back();
  return formula.ctx().make_or(new_container);
}
ltlf_ptr simplify(const LTLfEquivalent &formula) {
  vec_ptr args_negative(formula.args.size());
  std::transform(formula.args.begin(), formula.args.end(),
                 args_negative.begin(),
                 [](const ltlf_ptr &arg) { return arg->ctx().make_not(arg); });
  auto all_positive = formula.ctx().make_and(formula.args);
  auto all_negative = formula.ctx().make_and(args_negative);
  return formula.ctx().make_or({all_positive, all_negative});
}
ltlf_ptr simplify(const LTLfXor &formula) {
  vec_ptr args_negative(formula.args.size());
  std::transform(formula.args.begin(), formula.args.end(),
                 args_negative.begin(),
                 [](const ltlf_ptr &arg) { return arg->ctx().make_not(arg); });
  auto some_positive = formula.ctx().make_or(formula.args);
  auto some_negative = formula.ctx().make_or(args_negative);
  return formula.ctx().make_and({some_positive, some_negative});
}

void StringSymbol::accept(Visitor &visitor) const { visitor.visit(*this); }
void LTLfTrue::accept(Visitor &visitor) const { visitor.visit(*this); }
void LTLfFalse::accept(Visitor &visitor) const { visitor.visit(*this); }
void LTLfPropTrue::accept(Visitor &visitor) const { visitor.visit(*this); }
void LTLfPropFalse::accept(Visitor &visitor) const { visitor.visit(*this); }
void LTLfAtom::accept(Visitor &visitor) const { visitor.visit(*this); }
void LTLfNot::accept(Visitor &visitor) const { visitor.visit(*this); }
void LTLfPropositionalNot::accept(Visitor &visitor) const {
  visitor.visit(*this);
}
void LTLfAnd::accept(Visitor &visitor) const { visitor.visit(*this); }
void LTLfOr::accept(Visitor &visitor) const { visitor.visit(*this); }
void LTLfImplies::accept(Visitor &visitor) const { visitor.visit(*this); }
void LTLfEquivalent::accept(Visitor &visitor) const { visitor.visit(*this); }
void LTLfXor::accept(Visitor &visitor) const { visitor.visit(*this); }
void LTLfNext::accept(Visitor &visitor) const { visitor.visit(*this); }
void LTLfWeakNext::accept(Visitor &visitor) const { visitor.visit(*this); }
void LTLfUntil::accept(Visitor &visitor) const { visitor.visit(*this); }
void LTLfRelease::accept(Visitor &visitor) const { visitor.visit(*this); }
void LTLfEventually::accept(Visitor &visitor) const { visitor.visit(*this); }
void LTLfAlways::accept(Visitor &visitor) const { visitor.visit(*this); }

} // namespace logic
} // namespace nike