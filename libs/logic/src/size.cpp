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

#include <nike/logic/size.hpp>

namespace nike {
namespace logic {

void SizeVisitor::visit(const logic::LTLfTrue &f) { result += 1; }
void SizeVisitor::visit(const logic::LTLfFalse &f) { result += 1; }
void SizeVisitor::visit(const logic::LTLfPropTrue &f) { result += 1; }
void SizeVisitor::visit(const logic::LTLfPropFalse &f) { result += 1; }
void SizeVisitor::visit(const logic::LTLfAtom &f) { result += 1; }
void SizeVisitor::visit(const logic::LTLfNot &f) { throw_expected_nnf(); }
void SizeVisitor::visit(const logic::LTLfPropositionalNot &f) { result += 1; }
void SizeVisitor::visit(const logic::LTLfAnd &f) {
  result += 1;
  apply_to_ltlf_binary_op_(*this, f);
}
void SizeVisitor::visit(const logic::LTLfOr &f) {
  result += 1;
  apply_to_ltlf_binary_op_(*this, f);
}
void SizeVisitor::visit(const logic::LTLfImplies &f) { throw_expected_nnf(); }
void SizeVisitor::visit(const logic::LTLfEquivalent &f) {
  throw_expected_nnf();
}
void SizeVisitor::visit(const logic::LTLfXor &f) { throw_expected_nnf(); }
void SizeVisitor::visit(const logic::LTLfNext &f) {
  result += 1;
  apply_to_ltlf_unary_op_(*this, f);
}
void SizeVisitor::visit(const logic::LTLfWeakNext &f) {
  result += 1;
  apply_to_ltlf_unary_op_(*this, f);
}
void SizeVisitor::visit(const logic::LTLfUntil &f) {
  result += 1;
  apply_to_ltlf_binary_op_(*this, f);
}
void SizeVisitor::visit(const logic::LTLfRelease &f) {
  result += 1;
  apply_to_ltlf_binary_op_(*this, f);
}
void SizeVisitor::visit(const logic::LTLfEventually &f) {
  result += 1;
  apply_to_ltlf_unary_op_(*this, f);
}
void SizeVisitor::visit(const logic::LTLfAlways &f) {
  result += 1;
  apply_to_ltlf_unary_op_(*this, f);
}
void SizeVisitor::visit(const logic::PLTrue &f) { result += 1; }
void SizeVisitor::visit(const logic::PLFalse &f) { result += 1; }
void SizeVisitor::visit(const logic::PLLiteral &f) { result += 1; }
void SizeVisitor::visit(const logic::PLAnd &f) {
  result += 1;
  apply_to_pl_binary_op_(*this, f);
}
void SizeVisitor::visit(const logic::PLOr &f) {
  result += 1;
  apply_to_pl_binary_op_(*this, f);
}

size_t SizeVisitor::apply(const logic::LTLfFormula &formula) {
  formula.accept(*this);
  return result;
}
size_t SizeVisitor::apply(const logic::PLFormula &formula) {
  formula.accept(*this);
  return result;
}

size_t size(const logic::LTLfFormula &formula) {
  SizeVisitor visitor{};
  return visitor.apply(formula);
}
size_t size(const logic::PLFormula &formula) {
  SizeVisitor visitor{};
  return visitor.apply(formula);
}

} // namespace logic
} // namespace nike
