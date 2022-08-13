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

#include <nike/logic/pl.hpp>
#include <nike/logic/visitor.hpp>
#include <vector>

namespace nike {
namespace logic {

bool PLTrue::is_equal(const Comparable &o) const { return is_a<PLTrue>(o); }
int PLTrue::compare_(const Comparable &o) const {
  assert(is_a<PLTrue>(o));
  return 0;
}
bool PLFalse::is_equal(const Comparable &o) const { return is_a<PLFalse>(o); }
int PLFalse::compare_(const Comparable &o) const {
  assert(is_a<PLFalse>(o));
  return 0;
}
bool PLLiteral::is_equal(const Comparable &o) const {
  return is_a<PLLiteral>(o) and
         negated == dynamic_cast<const PLLiteral &>(o).negated and
         proposition == dynamic_cast<const PLLiteral &>(o).proposition;
}
int PLLiteral::compare_(const Comparable &o) const {
  assert(is_a<PLLiteral>(o));
  auto negated1 = negated;
  auto negated2 = dynamic_cast<const PLLiteral &>(o).negated;
  if (negated1 != negated2) {
    return negated1 < negated2 ? -1 : 1;
  }
  auto n1 = this->proposition;
  auto n2 = dynamic_cast<const PLLiteral &>(o).proposition;
  return n1 == n2 ? 0 : n1 < n2 ? -1 : 1;
}

bool PLBinaryOp::is_equal(const Comparable &o) const {
  return get_type_code() == o.get_type_code() and
         std::equal(args.begin(), args.end(),
                    dynamic_cast<const PLBinaryOp &>(o).args.begin(),
                    utils::Deref::Equal());
}
int PLBinaryOp::compare_(const Comparable &o) const {
  assert(this->get_type_code() == o.get_type_code());
  return utils::ordered_compare(this->args,
                                dynamic_cast<const PLBinaryOp &>(o).args);
}

void PLTrue::accept(Visitor &visitor) const { visitor.visit(*this); }
void PLFalse::accept(Visitor &visitor) const { visitor.visit(*this); }
void PLLiteral::accept(Visitor &visitor) const { visitor.visit(*this); }
void PLAnd::accept(Visitor &visitor) const { visitor.visit(*this); }
void PLOr::accept(Visitor &visitor) const { visitor.visit(*this); }
} // namespace logic
} // namespace nike