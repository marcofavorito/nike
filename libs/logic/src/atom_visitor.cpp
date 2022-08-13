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

#include <nike/logic/atom_visitor.hpp>
#include <nike/logic/ltlf.hpp>

namespace nike {
namespace logic {

void AtomsVisitor::visit(const PLTrue &) { result = set_ast_ptr{}; }
void AtomsVisitor::visit(const PLFalse &) { result = set_ast_ptr{}; }
void AtomsVisitor::visit(const PLLiteral &f) {
  result = set_ast_ptr{f.proposition};
}
void AtomsVisitor::visit(const PLAnd &f) {
  set_ast_ptr atoms_result, tmp;
  for (auto &a : f.args) {
    tmp = apply(*a);
    atoms_result.insert(tmp.begin(), tmp.end());
  }
  result = atoms_result;
}
void AtomsVisitor::visit(const PLOr &f) {
  set_ast_ptr atoms_result, tmp;
  for (auto &a : f.args) {
    tmp = apply(*a);
    atoms_result.insert(tmp.begin(), tmp.end());
  }
  result = atoms_result;
}

void AtomsVisitor::visit(const LTLfTrue &f) { result = set_ast_ptr{}; }
void AtomsVisitor::visit(const LTLfFalse &f) { result = set_ast_ptr{}; }
void AtomsVisitor::visit(const LTLfPropTrue &f) { result = set_ast_ptr{}; }
void AtomsVisitor::visit(const LTLfPropFalse &f) { result = set_ast_ptr{}; }
void AtomsVisitor::visit(const LTLfAtom &f) { result = set_ast_ptr{f.symbol}; }
void AtomsVisitor::visit(const LTLfNot &f) { visit_unary_op(f); }
void AtomsVisitor::visit(const LTLfPropositionalNot &f) { visit_unary_op(f); }
void AtomsVisitor::visit(const LTLfAnd &f) { visit_binary_op(f); }
void AtomsVisitor::visit(const LTLfOr &f) { visit_binary_op(f); }
void AtomsVisitor::visit(const LTLfImplies &f) { visit_binary_op(f); }
void AtomsVisitor::visit(const LTLfEquivalent &f) { visit_binary_op(f); }
void AtomsVisitor::visit(const LTLfXor &f) { visit_binary_op(f); }
void AtomsVisitor::visit(const LTLfNext &f) { visit_unary_op(f); }
void AtomsVisitor::visit(const LTLfWeakNext &f) { visit_unary_op(f); }
void AtomsVisitor::visit(const LTLfUntil &f) { visit_binary_op(f); }
void AtomsVisitor::visit(const LTLfRelease &f) { visit_binary_op(f); }
void AtomsVisitor::visit(const LTLfEventually &f) { visit_unary_op(f); }
void AtomsVisitor::visit(const LTLfAlways &f) { visit_unary_op(f); }

void AtomsVisitor::visit_binary_op(const LTLfBinaryOp &f) {
  set_ast_ptr atoms_result, tmp;
  for (auto &a : f.args) {
    tmp = apply(*a);
    atoms_result.insert(tmp.begin(), tmp.end());
  }
  result = atoms_result;
}

void AtomsVisitor::visit_unary_op(const LTLfUnaryOp &f) {
  result = apply(*f.arg);
}

set_ast_ptr AtomsVisitor::apply(const LTLfFormula &b) {
  b.accept(*this);
  return result;
}
set_ast_ptr AtomsVisitor::apply(const PLFormula &b) {
  b.accept(*this);
  return result;
}

set_ast_ptr find_atoms(const LTLfFormula &f) {
  AtomsVisitor atomsVisitor;
  return atomsVisitor.apply(f);
}
set_ast_ptr find_atoms(const PLFormula &f) {
  AtomsVisitor atomsVisitor;
  return atomsVisitor.apply(f);
}

} // namespace logic
} // namespace nike