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

#include <catch.hpp>
#include <glucose/simp/SimpSolver.h>
#include <iostream>


namespace nike {
namespace logic {
namespace Test {

TEST_CASE("glucose example", "[core][glucose]") {
    using Glucose::mkLit;
    using Glucose::lbool;

    Glucose::Solver solver;
    // Create variables
    auto A = solver.newVar();
    auto B = solver.newVar();
    auto C = solver.newVar();
    // Create the clauses
    solver.addClause( mkLit(A), mkLit(B), mkLit(C));
    solver.addClause(~mkLit(A), mkLit(B), mkLit(C));
    solver.addClause( mkLit(A), ~mkLit(B),
                      mkLit(C));
    solver.addClause( mkLit(A), mkLit(B),
                      ~mkLit(C));
    // Check for solution and retrieve model if found
    auto sat = solver.solve();
    if (sat) {
      std::clog << "SAT\n"
                << "Model found:\n";
      std::clog << "A := "
                << (solver.modelValue(A) == l_True)
                << '\n';
      std::clog << "B := "
                << (solver.modelValue(B) == l_True)
                << '\n';
      std::clog << "C := "
                << (solver.modelValue(C) == l_True)
                << '\n';
    } else {
      std::clog << "UNSAT\n";
    }
}
TEST_CASE("glucose example 2", "[core][glucose]") {
    using Glucose::mkLit;
    using Glucose::lbool;

    Glucose::Solver solver;
    // Create variables
    auto A = solver.newVar();
    auto B = solver.newVar();
    // Create the clauses
    solver.addClause( mkLit(A, true), mkLit(B, true));
    // Check for solution and retrieve model if found
    auto sat = solver.solve();
    if (sat) {
      std::clog << "SAT\n"
                << "Model found:\n";
      std::clog << "A := "
                << (solver.modelValue(A) == l_True)
                << '\n';
      std::clog << "B := "
                << (solver.modelValue(B) == l_True)
                << '\n';
    } else {
      std::clog << "UNSAT\n";
    }
}

} // namespace Test
} // namespace logic
} // namespace nike