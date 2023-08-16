#pragma once
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
#include <nike/logic/visitor.hpp>

namespace nike {
namespace logic {

class CopyVisitor : public Visitor {
protected:
  logic::Context &context;
  ltlf_ptr result;

public:
  explicit CopyVisitor(Context &context) : context{context} {};

  void visit(const LTLfTrue &) override;
  void visit(const LTLfFalse &) override;
  void visit(const LTLfPropTrue &) override;
  void visit(const LTLfPropFalse &) override;
  void visit(const LTLfAtom &) override;
  void visit(const LTLfNot &) override;
  void visit(const LTLfPropositionalNot &) override;
  void visit(const LTLfAnd &) override;
  void visit(const LTLfOr &) override;
  void visit(const LTLfImplies &) override;
  void visit(const LTLfEquivalent &) override;
  void visit(const LTLfXor &) override;
  void visit(const LTLfNext &) override;
  void visit(const LTLfWeakNext &) override;
  void visit(const LTLfUntil &) override;
  void visit(const LTLfRelease &) override;
  void visit(const LTLfEventually &) override;
  void visit(const LTLfAlways &) override;

  ltlf_ptr apply(const LTLfFormula &b);
  ltlf_ptr apply(const PLFormula &b);
  void visit_binary_op(ltlf_ptr (Context::*fun)(const vec_ptr &),
                       const LTLfBinaryOp &f);
  void visit_unary_op(ltlf_ptr (Context::*fun)(const ltlf_ptr &),
                      const LTLfUnaryOp &f);
};
ltlf_ptr copy_ltlf_formula(Context &context, const LTLfFormula &f);
ltlf_ptr copy_ltlf_formula(const LTLfFormula &f);
} // namespace logic
} // namespace nike