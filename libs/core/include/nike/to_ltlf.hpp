#pragma once
/*
 * This file is part of Lydia.
 *
 * Lydia is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Lydia is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Lydia.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "nike/logic/ltlf.hpp"
#include "nike/logic/visitor.hpp"
#include <utility>

namespace nike {
namespace logic {

class ToLTLfVisitor : public Visitor {
private:
  ltlf_ptr result;

public:
  ToLTLfVisitor() {}

  void visit(const PLTrue &) override;
  void visit(const PLFalse &) override;
  void visit(const PLLiteral &) override;
  void visit(const PLAnd &) override;
  void visit(const PLOr &) override;

  ltlf_ptr apply(const PLFormula &b);
};

ltlf_ptr to_ltlf(const PLFormula &formula);

} // namespace logic
} // namespace nike