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
#include <nike/to_ltlf.hpp>
#include <utility>

namespace nike {
namespace logic {

void ToLTLfVisitor::visit(const PLTrue &f) { result = f.ctx().make_tt(); }
void ToLTLfVisitor::visit(const PLFalse &f) { result = f.ctx().make_ff(); }
void ToLTLfVisitor::visit(const PLLiteral &f) {
  auto proposition = f.proposition;
  if (is_a<LTLfAtom>(*proposition)) {
    auto atom = std::static_pointer_cast<const LTLfAtom>(proposition);
    if (f.negated) {
      result = f.ctx().make_prop_not(atom);
      return;
    }
    result = atom;
    return;
  }
  if (is_a<StringSymbol>(*proposition)) {
    auto atom = f.ctx().make_atom(
        std::static_pointer_cast<const StringSymbol>(proposition));
    if (f.negated) {
      result = f.ctx().make_prop_not(atom);
      return;
    }
    result = atom;
    return;
  }

  result = std::static_pointer_cast<const LTLfFormula>(proposition);
  // expected to be in NNF
  assert(!f.negated);
}
void ToLTLfVisitor::visit(const PLAnd &f) {
  vec_ptr subf;
  for (auto &a : f.args) {
    subf.push_back(apply(*a));
  }
  result = f.ctx().make_and(subf);
}
void ToLTLfVisitor::visit(const PLOr &f) {
  vec_ptr subf;
  for (auto &a : f.args) {
    subf.push_back(apply(*a));
  }
  result = f.ctx().make_or(subf);
}

ltlf_ptr ToLTLfVisitor::apply(const PLFormula &b) {
  b.accept(*this);
  return result;
}

ltlf_ptr to_ltlf(const PLFormula &formula) {
  ToLTLfVisitor visitor{};
  return visitor.apply(formula);
}

} // namespace logic
} // namespace nike