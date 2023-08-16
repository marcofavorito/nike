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

#include <nike/logic/ltlf.hpp>
#include <nike/logic/visitor.hpp>
#include <utility>

namespace nike {
namespace logic {

class ReplaceVisitor : public Visitor {
private:
  pl_ptr result;
  std::map<ast_ptr, bool, utils::Deref::Less> replacements;

public:
  explicit ReplaceVisitor(
      std::map<ast_ptr, bool, utils::Deref::Less> replacements)
      : replacements{std::move(replacements)} {}

  void visit(const PLTrue &) override;
  void visit(const PLFalse &) override;
  void visit(const PLLiteral &) override;
  void visit(const PLAnd &) override;
  void visit(const PLOr &) override;

  pl_ptr apply(const PLFormula &b);
};

pl_ptr replace(std::map<ast_ptr, bool, utils::Deref::Less> replacements,
               const PLFormula &formula);

} // namespace logic
} // namespace nike