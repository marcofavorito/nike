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

#include <nike/logic/ltlf.hpp>
#include <nike/logic/replace.hpp>
#include <nike/logic/utils.hpp>
#include <nike/to_pl.hpp>
#include <utility>

namespace nike {
namespace logic {

void ToPLVisitor::visit(const logic::LTLfTrue &f) {
  result = f.ctx().make_literal(f.shared_from_this(), false);
}
void ToPLVisitor::visit(const logic::LTLfFalse &f) {
  result = f.ctx().make_literal(f.shared_from_this(), false);
}
void ToPLVisitor::visit(const logic::LTLfPropTrue &f) {
  auto not_end = f.ctx().make_not_end();
  result = f.ctx().make_literal(f.shared_from_this(), false);
}
void ToPLVisitor::visit(const logic::LTLfPropFalse &f) {
  result = f.ctx().make_false();
}
void ToPLVisitor::visit(const logic::LTLfAtom &f) {
  result = f.ctx().make_literal(f.symbol, false);
}
void ToPLVisitor::visit(const logic::LTLfNot &f) {
  logic::throw_expected_nnf();
}
void ToPLVisitor::visit(const logic::LTLfPropositionalNot &f) {
  result = f.ctx().make_literal(
      std::static_pointer_cast<const LTLfAtom>(f.arg)->symbol, true);
}
void ToPLVisitor::visit(const logic::LTLfAnd &f) {
  const auto &container = f.args;
  auto new_container = vec_pl_ptr(container.size());
  std::transform(
      container.begin(), container.end(), new_container.begin(),
      [this](const logic::ltlf_ptr &formula) { return apply(*formula); });
  result = f.ctx().make_prop_and(new_container);
}
void ToPLVisitor::visit(const logic::LTLfOr &f) {
  const auto &container = f.args;
  auto new_container = vec_pl_ptr(container.size());
  std::transform(
      container.begin(), container.end(), new_container.begin(),
      [this](const logic::ltlf_ptr &formula) { return apply(*formula); });
  result = f.ctx().make_prop_or(new_container);
}
void ToPLVisitor::visit(const logic::LTLfImplies &f) {
  logic::throw_expected_nnf();
}
void ToPLVisitor::visit(const logic::LTLfEquivalent &f) {
  logic::throw_expected_nnf();
}
void ToPLVisitor::visit(const logic::LTLfXor &f) {
  logic::throw_expected_nnf();
}
void ToPLVisitor::visit(const logic::LTLfNext &f) {
  result = f.ctx().make_literal(f.shared_from_this(), false);
}
void ToPLVisitor::visit(const logic::LTLfWeakNext &f) {
  result = f.ctx().make_literal(f.shared_from_this(), false);
}
void ToPLVisitor::visit(const logic::LTLfUntil &f) {
  logic::throw_expected_xnf();
}
void ToPLVisitor::visit(const logic::LTLfRelease &f) {
  logic::throw_expected_xnf();
}
void ToPLVisitor::visit(const logic::LTLfEventually &f) {
  auto not_end = f.ctx().make_not_end();
  if (*not_end == f) {
    result = f.ctx().make_literal(f.shared_from_this(), false);
    return;
  }
  logic::throw_expected_xnf();
}
void ToPLVisitor::visit(const logic::LTLfAlways &f) {
  auto end = f.ctx().make_end();
  if (*end == f) {
    result = f.ctx().make_literal(f.shared_from_this(), false);
    return;
  }
  logic::throw_expected_xnf();
}

pl_ptr ToPLVisitor::apply(const LTLfFormula &b) {
  b.accept(*this);
  return result;
}

pl_ptr to_pl(const LTLfFormula &formula) {
  ToPLVisitor visitor{};
  return visitor.apply(formula);
}

} // namespace logic
} // namespace nike