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
#include <nike/logic/pl.hpp>

namespace nike {
namespace logic {

inline void throw_not_implemented_error() {
  throw std::logic_error("handler not implemented");
}
inline void throw_not_supported_error() {
  throw std::logic_error("this case is not supported");
}
inline void throw_expected_nnf() {
  throw std::logic_error("expected formula in Negation-Normal Form");
}
inline void throw_expected_xnf() {
  throw std::logic_error("expected formula in Next-Normal Form");
}

/*
 * Abstract visitor class.
 */
class Visitor {
public:
  virtual void visit(const StringSymbol &) { throw_not_implemented_error(); };

  virtual void visit(const PLTrue &) { throw_not_implemented_error(); };
  virtual void visit(const PLFalse &) { throw_not_implemented_error(); };
  virtual void visit(const PLLiteral &) { throw_not_implemented_error(); };
  virtual void visit(const PLAnd &) { throw_not_implemented_error(); };
  virtual void visit(const PLOr &) { throw_not_implemented_error(); };

  virtual void visit(const LTLfTrue &) { throw_not_implemented_error(); };
  virtual void visit(const LTLfFalse &) { throw_not_implemented_error(); };
  virtual void visit(const LTLfPropTrue &) { throw_not_implemented_error(); };
  virtual void visit(const LTLfPropFalse &) { throw_not_implemented_error(); };
  virtual void visit(const LTLfAtom &) { throw_not_implemented_error(); };
  virtual void visit(const LTLfNot &) { throw_not_implemented_error(); };
  virtual void visit(const LTLfPropositionalNot &) {
    throw_not_implemented_error();
  };
  virtual void visit(const LTLfAnd &) { throw_not_implemented_error(); };
  virtual void visit(const LTLfOr &) { throw_not_implemented_error(); };
  virtual void visit(const LTLfImplies &) { throw_not_implemented_error(); };
  virtual void visit(const LTLfEquivalent &) { throw_not_implemented_error(); };
  virtual void visit(const LTLfXor &) { throw_not_implemented_error(); };
  virtual void visit(const LTLfNext &) { throw_not_implemented_error(); };
  virtual void visit(const LTLfWeakNext &) { throw_not_implemented_error(); };
  virtual void visit(const LTLfUntil &) { throw_not_implemented_error(); };
  virtual void visit(const LTLfRelease &) { throw_not_implemented_error(); };
  virtual void visit(const LTLfEventually &) { throw_not_implemented_error(); };
  virtual void visit(const LTLfAlways &) { throw_not_implemented_error(); };
};

template <typename VisitorClass>
inline void apply_to_ltlf_binary_op_(VisitorClass &visitor,
                                     const logic::LTLfBinaryOp &formula) {
  for (const auto &arg : formula.args) {
    visitor.apply(*arg);
  }
}
template <typename VisitorClass>
inline void apply_to_ltlf_unary_op_(VisitorClass &visitor,
                                    const logic::LTLfUnaryOp &formula) {
  visitor.apply(*formula.arg);
}
template <typename VisitorClass>
inline void apply_to_pl_binary_op_(VisitorClass &visitor,
                                   const logic::PLBinaryOp &formula) {
  for (const auto &arg : formula.args) {
    visitor.apply(*arg);
  }
}

} // namespace logic
} // namespace nike