/*
 * This file is part of Nike.
 *
 * Nike is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Nike is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Nike.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <functional>
#include <nike/logic/copy.hpp>
#include <nike/logic/ltlf.hpp>

namespace nike {
namespace logic {

void CopyVisitor::visit(const LTLfTrue &f) { result = context.make_tt(); }
void CopyVisitor::visit(const LTLfFalse &f) { result = context.make_ff(); }
void CopyVisitor::visit(const LTLfPropTrue &f) {
  result = context.make_prop_true();
}
void CopyVisitor::visit(const LTLfPropFalse &f) {
  result = context.make_prop_false();
}
void CopyVisitor::visit(const LTLfAtom &f) {
  auto symbol_name =
      std::static_pointer_cast<const StringSymbol>(f.symbol)->name;
  result = context.make_atom(symbol_name);
}
void CopyVisitor::visit(const LTLfNot &f) {
  visit_unary_op(&Context::make_not, f);
}
void CopyVisitor::visit(const LTLfPropositionalNot &f) {
  visit_unary_op(&Context::make_prop_not, f);
}
void CopyVisitor::visit(const LTLfAnd &f) {
  visit_binary_op(&Context::make_and, f);
}
void CopyVisitor::visit(const LTLfOr &f) {
  visit_binary_op(&Context::make_or, f);
}
void CopyVisitor::visit(const LTLfImplies &f) {
  visit_binary_op(&Context::make_implies, f);
}
void CopyVisitor::visit(const LTLfEquivalent &f) {
  visit_binary_op(&Context::make_equivalent, f);
}
void CopyVisitor::visit(const LTLfXor &f) {
  visit_binary_op(&Context::make_xor, f);
}
void CopyVisitor::visit(const LTLfNext &f) {
  visit_unary_op(&Context::make_next, f);
}
void CopyVisitor::visit(const LTLfWeakNext &f) {
  visit_unary_op(&Context::make_weak_next, f);
}
void CopyVisitor::visit(const LTLfUntil &f) {
  visit_binary_op(&Context::make_until, f);
}
void CopyVisitor::visit(const LTLfRelease &f) {
  visit_binary_op(&Context::make_release, f);
}
void CopyVisitor::visit(const LTLfEventually &f) {
  visit_unary_op(&Context::make_eventually, f);
}
void CopyVisitor::visit(const LTLfAlways &f) {
  visit_unary_op(&Context::make_always, f);
}

void CopyVisitor::visit_binary_op(ltlf_ptr (Context::*fun)(const vec_ptr &v),
                                  const LTLfBinaryOp &f) {
  std::vector<ltlf_ptr> new_args;
  auto bind_function = std::bind(fun, &context, std::placeholders::_1);
  for (auto &a : f.args) {
    new_args.push_back(apply(*a));
  }
  result = bind_function(new_args);
}

void CopyVisitor::visit_unary_op(ltlf_ptr (Context::*fun)(const ltlf_ptr &),
                                 const LTLfUnaryOp &f) {
  ltlf_ptr arg = apply(*f.arg);
  auto bind_function = std::bind(fun, &context, std::placeholders::_1);
  result = bind_function(arg);
}

ltlf_ptr CopyVisitor::apply(const LTLfFormula &b) {
  b.accept(*this);
  return result;
}
ltlf_ptr CopyVisitor::apply(const PLFormula &b) {
  b.accept(*this);
  return result;
}

ltlf_ptr copy_ltlf_formula(Context &context, const LTLfFormula &f) {
  CopyVisitor copy_visitor{context};
  auto result = copy_visitor.apply(f);
  return result;
}
ltlf_ptr copy_ltlf_formula(const LTLfFormula &f) {
  CopyVisitor copy_visitor{f.ctx()};
  auto result = copy_visitor.apply(f);
  return result;
}

} // namespace logic
} // namespace nike