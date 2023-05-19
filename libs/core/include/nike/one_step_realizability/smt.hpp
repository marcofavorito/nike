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

#include <z3++.h>
#include <nike/core.hpp>
#include <nike/logic/visitor.hpp>
#include <nike/one_step_realizability/base.hpp>
#include <optional>

namespace nike {
  namespace core {


    class SmtOneStepRealizabilityVisitor : public logic::Visitor {
    public:
      InputOutputPartition partition;
      z3::context& z3_context;
      z3::solver& solver;
      z3::expr result;
      std::set<std::string> uncontrollableVars;
      // dummy value for 'result' since z3::expr does not have a default constructor
      explicit SmtOneStepRealizabilityVisitor(const InputOutputPartition &partition, z3::context& z3_context, z3::solver& solver)
          : partition{partition}, z3_context{z3_context}, solver{solver}, result{z3_context.bool_val(true)} {}
      ~SmtOneStepRealizabilityVisitor() {}
      void visit(const logic::LTLfTrue &) override;
      void visit(const logic::LTLfFalse &) override;
      void visit(const logic::LTLfPropTrue &) override;
      void visit(const logic::LTLfPropFalse &) override;
      void visit(const logic::LTLfAtom &) override;
      void visit(const logic::LTLfNot &) override;
      void visit(const logic::LTLfPropositionalNot &) override;
      void visit(const logic::LTLfAnd &) override;
      void visit(const logic::LTLfOr &) override;
      void visit(const logic::LTLfImplies &) override;
      void visit(const logic::LTLfEquivalent &) override;
      void visit(const logic::LTLfXor &) override;
      void visit(const logic::LTLfNext &) override;
      void visit(const logic::LTLfWeakNext &) override;
      void visit(const logic::LTLfUntil &) override;
      void visit(const logic::LTLfRelease &) override;
      void visit(const logic::LTLfEventually &) override;
      void visit(const logic::LTLfAlways &) override;

      z3::expr apply(const logic::LTLfFormula &f);
    };

    class SmtOneStepRealizabilityChecker : public OneStepRealizabilityChecker {
    public:
      z3::context z3_context;
      z3::solver solver;

      SmtOneStepRealizabilityChecker() : OneStepRealizabilityChecker(), z3_context{}, solver{z3_context} {
        z3::params p(z3_context);
        solver.set(p);
      }

      std::optional<move_t> one_step_realizable(const logic::LTLfFormula &f, const InputOutputPartition &partition) override;
    };

  } // namespace core
} // namespace nike
