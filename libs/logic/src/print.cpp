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

#include <nike/logic/print.hpp>
#include <sstream>

namespace nike {
namespace logic {

void PrintVisitor::visit(const PLTrue &f) { result = "ptrue"; };
void PrintVisitor::visit(const PLFalse &f) { result = "pfalse"; };
void PrintVisitor::visit(const PLLiteral &f) {
  if (is_a<StringSymbol>(*f.proposition)) {
    result = std::static_pointer_cast<const StringSymbol>(f.proposition)->name;
  } else {
    result =
        to_string(*std::static_pointer_cast<const LTLfFormula>(f.proposition));
  }
  if (f.negated) {
    result = "!(" + result + ")";
  }
};
void PrintVisitor::visit(const PLAnd &f) { pl_binary_op_to_string(f, "&"); };
void PrintVisitor::visit(const PLOr &f) { pl_binary_op_to_string(f, "|"); };

void PrintVisitor::visit(const LTLfTrue &formula) { result = "tt"; }
void PrintVisitor::visit(const LTLfFalse &formula) { result = "ff"; }
void PrintVisitor::visit(const LTLfPropTrue &formula) { result = "true"; }
void PrintVisitor::visit(const LTLfPropFalse &formula) { result = "false"; }
void PrintVisitor::visit(const LTLfAtom &formula) {
  if (is_a<StringSymbol>(*formula.symbol)) {
    result = std::static_pointer_cast<const StringSymbol>(formula.symbol)->name;
    return;
  }
  result = apply(*std::static_pointer_cast<const LTLfFormula>(formula.symbol));
}
void PrintVisitor::visit(const LTLfNot &formula) {
  unary_op_to_string(formula, "~");
}
void PrintVisitor::visit(const LTLfPropositionalNot &formula) {
  result = "!" + apply(*formula.get_atom());
}
void PrintVisitor::visit(const LTLfAnd &formula) {
  binary_op_to_string(formula, "&");
}
void PrintVisitor::visit(const LTLfOr &formula) {
  binary_op_to_string(formula, "|");
}
void PrintVisitor::visit(const LTLfImplies &formula) {
  binary_op_to_string(formula, "->");
}
void PrintVisitor::visit(const LTLfEquivalent &formula) {
  binary_op_to_string(formula, "<->");
}
void PrintVisitor::visit(const LTLfXor &formula) {
  binary_op_to_string(formula, "^");
}
void PrintVisitor::visit(const LTLfNext &formula) {
  unary_op_to_string(formula, "X[!]");
}
void PrintVisitor::visit(const LTLfWeakNext &formula) {
  unary_op_to_string(formula, "X");
}
void PrintVisitor::visit(const LTLfUntil &formula) {
  binary_op_to_string(formula, "U");
}
void PrintVisitor::visit(const LTLfRelease &formula) {
  binary_op_to_string(formula, "R");
}
void PrintVisitor::visit(const LTLfEventually &formula) {
  unary_op_to_string(formula, "F");
}
void PrintVisitor::visit(const LTLfAlways &formula) {
  unary_op_to_string(formula, "G");
}

std::string PrintVisitor::apply(const LTLfFormula &f) {
  f.accept(*this);
  return result;
}
std::string PrintVisitor::apply(const PLFormula &f) {
  f.accept(*this);
  return result;
}

void PrintVisitor::binary_op_to_string(const LTLfBinaryOp &formula,
                                       const std::string &op_symbol) {
  std::stringstream ss;
  std::string temp;
  for (auto it = formula.args.begin(); it != formula.args.end() - 1; ++it) {
    ss << "(" << apply(**it) << ") " << op_symbol << " ";
  }
  ss << "(" << apply(**(formula.args.end() - 1)) << ")";
  result = ss.str();
}
void PrintVisitor::pl_binary_op_to_string(const PLBinaryOp &formula,
                                          const std::string &op_symbol) {
  std::stringstream ss;
  std::string temp;
  for (auto it = formula.args.begin(); it != formula.args.end() - 1; ++it) {
    ss << "(" << apply(**it) << ") " << op_symbol << " ";
  }
  ss << "(" << apply(**(formula.args.end() - 1)) << ")";
  result = ss.str();
}

void PrintVisitor::unary_op_to_string(const LTLfUnaryOp &formula,
                                      const std::string &op_symbol) {
  std::stringstream ss;
  std::string temp;
  ss << op_symbol << "(" << apply(*formula.arg) << ")";
  result = ss.str();
}

std::string to_string(const LTLfFormula &f) {
  PrintVisitor visitor{};
  return visitor.apply(f);
}

std::string to_string(const PLFormula &f) {
  PrintVisitor visitor{};
  return visitor.apply(f);
}

} // namespace logic
} // namespace nike