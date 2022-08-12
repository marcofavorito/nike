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
#include <utility>

namespace nike {
namespace logic {

void ReplaceVisitor::visit(const PLTrue &f) { result = f.ctx().make_true(); }
void ReplaceVisitor::visit(const PLFalse &f) { result = f.ctx().make_false(); }
void ReplaceVisitor::visit(const PLLiteral &f) {
  auto literal =
      std::static_pointer_cast<const PLFormula>(f.shared_from_this());
  auto replacement = replacements.find(literal);
  if (replacement == replacements.end()) {
    result = literal;
    return;
  }

  result = replacement->second != f.negated ? f.ctx().make_true()
                                            : f.ctx().make_false();
}

void ReplaceVisitor::visit(const PLAnd &f) {
  result = forward_call_to_arguments(
      f, [this](const pl_ptr &formula) { return apply(*formula); },
      [f](const vec_pl_ptr &container) {
        return f.ctx().make_prop_and(container);
      });
}
void ReplaceVisitor::visit(const PLOr &f) {
  result = forward_call_to_arguments(
      f, [this](const pl_ptr &formula) { return apply(*formula); },
      [f](const vec_pl_ptr &container) {
        return f.ctx().make_prop_or(container);
      });
}
pl_ptr ReplaceVisitor::apply(const PLFormula &b) {
  b.accept(*this);
  return result;
}

pl_ptr replace(std::map<pl_ptr, bool, utils::Deref::Less> replacements,
               const PLFormula &formula) {
  ReplaceVisitor visitor{std::move(replacements)};
  return visitor.apply(formula);
}

} // namespace logic
} // namespace nike