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

#include <cassert>
#include "nike/one_step_realizability/smt.hpp"
#include "nike/logic/print.hpp"


namespace nike {
namespace core {

void SmtOneStepRealizabilityVisitor::visit(const logic::LTLfTrue &formula) {
  result = z3_context.bool_val(true);
}
void SmtOneStepRealizabilityVisitor::visit(const logic::LTLfFalse &formula) {
  result = z3_context.bool_val(false);
}
void SmtOneStepRealizabilityVisitor::visit(const logic::LTLfPropTrue &formula) {
  result = z3_context.bool_val(true);
}
void SmtOneStepRealizabilityVisitor::visit(const logic::LTLfPropFalse &formula) {
  result = z3_context.bool_val(false);
}
void SmtOneStepRealizabilityVisitor::visit(const logic::LTLfAtom &formula) {
  assert(logic::is_a<const logic::StringSymbol>(*formula.symbol));
  auto varname = std::static_pointer_cast<const logic::StringSymbol>(formula.symbol)->name.c_str();
  result = z3_context.bool_const(varname);

  if (partition.is_input(varname)){
    uncontrollableVars.insert(varname);
  }
}

void SmtOneStepRealizabilityVisitor::visit(const logic::LTLfNot &formula) {
  logic::throw_expected_nnf();
}
void SmtOneStepRealizabilityVisitor::visit(
    const logic::LTLfPropositionalNot &formula) {
//  result = !apply(*formula.get_atom()).simplify();
  result = !apply(*formula.get_atom());
}
void SmtOneStepRealizabilityVisitor::visit(const logic::LTLfAnd &formula) {
  z3::expr finalResult = z3_context.bool_val(true);
  for (const auto &subf : formula.args) {
    finalResult = finalResult & apply(*subf);
//    finalResult = finalResult.simplify();
    if (z3::eq(finalResult, z3_context.bool_val(false))) {
      break;
    }
  }
  result = finalResult;
}
void SmtOneStepRealizabilityVisitor::visit(const logic::LTLfOr &formula) {
  z3::expr finalResult = z3_context.bool_val(false);
  for (const auto &subf : formula.args) {
    finalResult = finalResult | apply(*subf);
//    finalResult = finalResult.simplify();
    if (z3::eq(finalResult, z3_context.bool_val(true))) {
      break;
    }
  }
  result = finalResult;
}
void SmtOneStepRealizabilityVisitor::visit(const logic::LTLfImplies &formula) {
  logic::throw_expected_nnf();
}
void SmtOneStepRealizabilityVisitor::visit(const logic::LTLfEquivalent &formula) {
  logic::throw_expected_nnf();
}
void SmtOneStepRealizabilityVisitor::visit(const logic::LTLfXor &formula) {
  logic::throw_expected_nnf();
}
void SmtOneStepRealizabilityVisitor::visit(const logic::LTLfNext &formula) {
  result = z3_context.bool_val(false);
}
void SmtOneStepRealizabilityVisitor::visit(const logic::LTLfWeakNext &formula) {
  result = z3_context.bool_val(true);
}
void SmtOneStepRealizabilityVisitor::visit(const logic::LTLfUntil &formula) {
  result = apply(**formula.args.rbegin());
}
void SmtOneStepRealizabilityVisitor::visit(const logic::LTLfRelease &formula) {
  result = apply(**formula.args.rbegin());
}
void SmtOneStepRealizabilityVisitor::visit(const logic::LTLfEventually &formula) {
  result = apply(*formula.arg);
}
void SmtOneStepRealizabilityVisitor::visit(const logic::LTLfAlways &formula) {
  result = apply(*formula.arg);
}

z3::expr SmtOneStepRealizabilityVisitor::apply(const logic::LTLfFormula &f) {
  f.accept(*this);
  return result;
}

std::optional<move_t> SmtOneStepRealizabilityChecker::one_step_realizable(const logic::LTLfFormula &f, const InputOutputPartition &partition) {
  auto visitor = SmtOneStepRealizabilityVisitor{partition, z3_context, solver};
  auto result = visitor.apply(f);

  auto quantified = result;
  if (!visitor.uncontrollableVars.empty()){
    z3::expr_vector uncontollables_expr(z3_context);
    for (const auto& uncontrollableVar : visitor.uncontrollableVars) {
      uncontollables_expr.push_back(z3_context.bool_const(uncontrollableVar.c_str()));
    }
    quantified = z3::forall(uncontollables_expr, result);
  }
  solver.reset();
  solver.add(quantified);

  if (!solver.check()){
    return std::nullopt;
  }

  // build the agent move from Z3 model
  z3::model m = solver.get_model();
  move_t move{};

  for (const auto& controllableVar : visitor.partition.output_variables) {
    auto evaluation = m.eval(z3_context.bool_const(controllableVar.c_str()));
    VarValues varValue;
    if (z3::eq(evaluation, z3_context.bool_val(true))) {
      varValue = VarValues::TRUE;
    }
    else if (z3::eq(evaluation, z3_context.bool_val(false))) {
      varValue = VarValues::FALSE;
    }
    else{
      varValue = VarValues::DONT_CARE;
    }
    move.emplace_back(controllableVar, varValue);
  }

  return move;

  }


} // namespace core
} // namespace nike
