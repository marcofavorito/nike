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

#include <cuddObj.hh>
#include <nike/core.hpp>
#include <nike/logic/visitor.hpp>

namespace nike {
namespace core {

class ToBddVisitor : public logic::Visitor {
private:
  CUDD::BDD result;
  Context &context_;

public:
  explicit ToBddVisitor(Context &context) : context_{context} {}
  void visit(const logic::LTLfTrue &) override;
  void visit(const logic::LTLfFalse &) override;
  void visit(const logic::LTLfPropTrue &) override;
  void visit(const logic::LTLfPropFalse &) override;
  void visit(const logic::LTLfAtom &) override;
  void visit(const logic::LTLfNot &) override;
  void visit(const logic::LTLfPropositionalNot &) override;
  void visit(const logic::LTLfAnd &) override;
  void visit(const logic::LTLfOr &) override;
  void visit(const logic::LTLfImplies &) override;
  void visit(const logic::LTLfEquivalent &) override;
  void visit(const logic::LTLfXor &) override;
  void visit(const logic::LTLfNext &) override;
  void visit(const logic::LTLfWeakNext &) override;
  void visit(const logic::LTLfUntil &) override;
  void visit(const logic::LTLfRelease &) override;
  void visit(const logic::LTLfEventually &) override;
  void visit(const logic::LTLfAlways &) override;

  CUDD::BDD apply(const logic::LTLfFormula &formula);
  CUDD::BDD get_bdd_var(const logic::LTLfFormula &formula);
};

CUDD::BDD to_bdd(const logic::LTLfFormula &formula, Context &context);

} // namespace core
} // namespace nike
