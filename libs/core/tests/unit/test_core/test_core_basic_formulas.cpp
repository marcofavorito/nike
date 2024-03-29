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

#include "core_test_utils.hpp"
#include <catch.hpp>
#include <nike/core.hpp>
#include <nike/parser/driver.hpp>
#include <sstream>

namespace nike {
namespace core {
namespace Test {

TEST_CASE("forward synthesis of 'tt'") {
  logic::Context context;
  auto partition = InputOutputPartition({"a"}, {"b"});
  auto tt = context.make_tt();
  bool result = test_is_realizable(tt, partition);
  REQUIRE(result);
}

TEST_CASE("forward synthesis of 'ff'") {
  logic::Context context;
  auto partition = InputOutputPartition({"a"}, {"b"});
  auto ff = context.make_ff();
  bool result = test_is_realizable(ff, partition);
  REQUIRE(!result);
}

TEST_CASE("forward synthesis of 'true'") {
  logic::Context context;
  auto partition = InputOutputPartition({"a"}, {"b"});
  auto true_ = context.make_prop_true();
  bool result = test_is_realizable(true_, partition);
  REQUIRE(result);
}

TEST_CASE("forward synthesis of 'false'") {
  logic::Context context;
  auto partition = InputOutputPartition({"a"}, {"b"});
  auto false_ = context.make_prop_false();
  bool result = test_is_realizable(false_, partition);
  REQUIRE(!result);
}
TEST_CASE("forward synthesis of atom") {
  logic::Context context;
  auto a = context.make_atom("a");

  SECTION("atom controllable") {
    auto partition = InputOutputPartition({"b"}, {"a"});
    bool result = test_is_realizable(a, partition);
    REQUIRE(result);
  }
  SECTION("atom not-controllable") {
    auto partition = InputOutputPartition({"a"}, {"b"});
    bool result = test_is_realizable(a, partition);
    REQUIRE(!result);
  }
}
TEST_CASE("forward synthesis of not atom") {
  logic::Context context;
  auto a = context.make_atom("a");
  auto not_a = context.make_prop_not(a);

  SECTION("atom controllable") {
    auto partition = InputOutputPartition({"b"}, {"a"});
    bool result = test_is_realizable(not_a, partition);
    REQUIRE(result);
  }
  SECTION("atom not-controllable") {
    auto partition = InputOutputPartition({"a"}, {"b"});
    bool result = test_is_realizable(not_a, partition);
    REQUIRE(!result);
  }
}

TEST_CASE("forward synthesis of 'a and b'") {
  logic::Context context;
  auto a = context.make_atom("a");
  auto b = context.make_atom("b");
  auto a_and_b = context.make_and({a, b});

  SECTION("left not-controllable, right not-controllable") {
    auto partition = InputOutputPartition({"a", "b"}, {"c"});
    bool result = test_is_realizable(a_and_b, partition);
    REQUIRE(!result);
  }
  SECTION("left not-controllable, right controllable") {
    auto partition = InputOutputPartition({"a"}, {"b"});
    bool result = test_is_realizable(a_and_b, partition);
    REQUIRE(!result);
  }
  SECTION("left controllable, right not-controllable") {
    auto partition = InputOutputPartition({"b"}, {"a"});
    bool result = test_is_realizable(a_and_b, partition);
    REQUIRE(!result);
  }
  SECTION("left controllable, right controllable") {
    auto partition = InputOutputPartition({"c"}, {"a", "b"});
    bool result = test_is_realizable(a_and_b, partition);
    REQUIRE(result);
  }
}

TEST_CASE("forward synthesis of 'a or b'") {
  logic::Context context;
  auto a = context.make_atom("a");
  auto b = context.make_atom("b");
  auto a_or_b = context.make_or({a, b});
  SECTION("left not-controllable, right not-controllable") {
    auto partition = InputOutputPartition({"a", "b"}, {"c"});
    bool result = test_is_realizable(a_or_b, partition);
    REQUIRE(!result);
  }
  SECTION("left not-controllable, right controllable") {
    auto partition = InputOutputPartition({"a"}, {"b"});
    bool result = test_is_realizable(a_or_b, partition);
    REQUIRE(result);
  }
  SECTION("left controllable, right not-controllable") {
    auto partition = InputOutputPartition({"b"}, {"a"});
    bool result = test_is_realizable(a_or_b, partition);
    REQUIRE(result);
  }
  SECTION("left controllable, right controllable") {
    auto partition = InputOutputPartition({"c"}, {"a", "b"});
    bool result = test_is_realizable(a_or_b, partition);
    REQUIRE(result);
  }
}

TEST_CASE("forward synthesis of 'X[!]a' (controllable)") {
  logic::Context context;
  auto a = context.make_atom("a");
  auto next_a = context.make_next(a);

  SECTION("body controllable") {
    auto partition = InputOutputPartition({"b"}, {"a"});
    bool result = test_is_realizable(next_a, partition);
    REQUIRE(result);
  }
  SECTION("body not-controllable") {
    auto partition = InputOutputPartition({"a"}, {"b"});
    bool result = test_is_realizable(next_a, partition);
    REQUIRE(!result);
  }
}

TEST_CASE("forward synthesis of 'X a'") {
  logic::Context context;
  auto a = context.make_atom("a");
  auto weak_next_a = context.make_weak_next(a);

  SECTION("body controllable") {
    auto partition = InputOutputPartition({"b"}, {"a"});
    bool result = test_is_realizable(weak_next_a, partition);
    REQUIRE(result);
  }
  SECTION("body not-controllable") {
    auto partition = InputOutputPartition({"a"}, {"b"});
    bool result = test_is_realizable(weak_next_a, partition);
    REQUIRE(result);
  }
}
TEST_CASE("forward synthesis of 'X a', non-empty trace") {
  logic::Context context;
  auto a = context.make_atom("a");
  auto weak_next_a = context.make_weak_next(a);
  auto not_end = context.make_eventually(context.make_tt());
  auto formula = context.make_and({weak_next_a, not_end});

  SECTION("body controllable") {
    auto partition = InputOutputPartition({"b"}, {"a"});
    bool result = test_is_realizable(formula, partition);
    REQUIRE(result);
  }
  SECTION("body not-controllable") {
    auto partition = InputOutputPartition({"a"}, {"b"});
    bool result = test_is_realizable(formula, partition);
    REQUIRE(result);
  }
}
TEST_CASE("forward synthesis of 'a U b'") {
  logic::Context context;
  auto b = context.make_atom("b");
  auto a = context.make_atom("a");
  auto a_until_b = context.make_until({a, b});

  SECTION("head non-controllable, tail non-controllable") {
    auto partition = InputOutputPartition({"a", "b"}, {"c"});
    bool result = test_is_realizable(a_until_b, partition);
    REQUIRE(!result);
  }
  SECTION("head non-controllable, tail controllable") {
    auto partition = InputOutputPartition({"a"}, {"b"});
    bool result = test_is_realizable(a_until_b, partition);
    REQUIRE(result);
  }
  SECTION("head controllable, tail non-controllable") {
    auto partition = InputOutputPartition({"b"}, {"a"});
    bool result = test_is_realizable(a_until_b, partition);
    REQUIRE(!result);
  }
  SECTION("head controllable, tail controllable") {
    auto partition = InputOutputPartition({"c"}, {"a", "b"});
    bool result = test_is_realizable(a_until_b, partition);
    REQUIRE(result);
  }
}

TEST_CASE("forward synthesis of 'a R b'") {
  // NOTE: safety formula, always realizable with empty traces
  logic::Context context;
  auto b = context.make_atom("b");
  auto a = context.make_atom("a");
  auto a_release_b = context.make_release({a, b});

  SECTION("head non-controllable, tail non-controllable") {
    auto partition = InputOutputPartition({"a", "b"}, {"c"});
    bool result = test_is_realizable(a_release_b, partition);
    REQUIRE(result);
  }
  SECTION("head non-controllable, tail controllable") {
    auto partition = InputOutputPartition({"a"}, {"b"});
    bool result = test_is_realizable(a_release_b, partition);
    REQUIRE(result);
  }
  SECTION("head controllable, tail non-controllable") {
    auto partition = InputOutputPartition({"b"}, {"a"});
    bool result = test_is_realizable(a_release_b, partition);
    REQUIRE(result);
  }
  SECTION("head controllable, tail controllable") {
    auto partition = InputOutputPartition({"c"}, {"a", "b"});
    bool result = test_is_realizable(a_release_b, partition);
    REQUIRE(result);
  }
}

TEST_CASE("forward synthesis of 'F a'") {
  logic::Context context;
  auto a = context.make_atom("a");
  auto eventually_a = context.make_eventually(a);

  SECTION("tail not-controllable") {
    auto partition = InputOutputPartition({"a"}, {"b"});
    bool result = test_is_realizable(eventually_a, partition);
    REQUIRE(!result);
  }
  SECTION("tail controllable") {
    auto partition = InputOutputPartition({"b"}, {"a"});
    bool result = test_is_realizable(eventually_a, partition);
    REQUIRE(result);
  }
}

TEST_CASE("forward synthesis of 'G a'") {
  // NOTE: safety formula, always realizable with empty traces
  logic::Context context;
  auto a = context.make_atom("a");
  auto always_a = context.make_always(a);

  SECTION("tail not-controllable") {
    auto partition = InputOutputPartition({"a"}, {"b"});
    bool result = test_is_realizable(always_a, partition);
    REQUIRE(result);
  }
  SECTION("tail controllable") {
    auto partition = InputOutputPartition({"b"}, {"a"});
    bool result = test_is_realizable(always_a, partition);
    REQUIRE(result);
  }
}
TEST_CASE("forward synthesis of 'G a', non-empty trace") {
  logic::Context context;
  auto a = context.make_atom("a");
  auto always_a = context.make_always(a);
  auto not_end = context.make_eventually(context.make_tt());
  auto formula = context.make_and({always_a, not_end});

  SECTION("tail not-controllable") {
    auto partition = InputOutputPartition({"a"}, {"b"});
    bool result = test_is_realizable(formula, partition);
    REQUIRE(!result);
  }
  SECTION("tail controllable") {
    auto partition = InputOutputPartition({"b"}, {"a"});
    bool result = test_is_realizable(formula, partition);
    REQUIRE(result);
  }
}
TEST_CASE("forward synthesis of 'G(a | !b)', non-empty trace") {
  logic::Context context;
  auto a = context.make_atom("a");
  auto b = context.make_atom("b");
  auto not_b = context.make_prop_not(b);
  auto a_or_not_b = context.make_or({a, not_b});
  auto always = context.make_always(a_or_not_b);
  auto not_end = context.make_eventually(context.make_tt());
  auto formula = context.make_and({always, not_end});

  SECTION("b controllable") {
    auto partition = InputOutputPartition({"a"}, {"b"});
    bool result = test_is_realizable(formula, partition);
    REQUIRE(result);
  }
  SECTION("a controllable") {
    auto partition = InputOutputPartition({"b"}, {"a"});
    bool result = test_is_realizable(formula, partition);
    REQUIRE(result);
  }
}

TEST_CASE("forward synthesis of 'X(F(p0))'") {
  logic::Context context{};
  auto p0 = context.make_atom("p0");
  auto not_end = context.make_not(context.make_end());
  auto eventually_p0 = context.make_eventually(p0);
  auto next = context.make_weak_next(eventually_p0);
  auto formula = context.make_and({next, not_end});
  SECTION("p0 uncontrollable") {
    auto partition = InputOutputPartition({"p0"}, {"p1"});
    bool result = test_is_realizable(formula, partition);
    REQUIRE(result);
  }
  SECTION("p0 controllable") {
    auto partition = InputOutputPartition({"p1"}, {"p0"});
    bool result = test_is_realizable(formula, partition);
    REQUIRE(result);
  }
}
TEST_CASE("forward synthesis of '((a) & (X(~(a)))) | ((~(a)) & (X(a)))'") {
  logic::Context context{};
  auto a = context.make_atom("a");
  auto not_a = context.make_prop_not(a);
  auto next_a = context.make_next(a);
  auto next_not_a = context.make_next(not_a);

  auto a_next_not_a = context.make_and({a, next_not_a});
  auto not_a_next_a = context.make_and({not_a, next_a});
  auto not_end = context.make_not_end();
  auto formula_or = context.make_or({a_next_not_a, not_a_next_a});
  auto formula = context.make_and({formula_or, not_end});

  SECTION("a uncontrollable") {
    auto partition = InputOutputPartition({"b"}, {"a"});
    bool result = test_is_realizable(formula, partition);
    REQUIRE(result);
  }
  SECTION("a controllable") {
    auto partition = InputOutputPartition({"b"}, {"a"});
    bool result = test_is_realizable(formula, partition);
    REQUIRE(result);
  }
}

TEST_CASE("forward synthesis of 'p0 R (F(p1))'") {
  logic::Context context{};
  auto p0 = context.make_atom("p0");
  auto p1 = context.make_atom("p1");
  auto not_end = context.make_not(context.make_end());
  auto eventually_p1 = context.make_eventually(p1);
  auto release = context.make_release({p0, eventually_p1});
  auto formula = context.make_and({release, not_end});

  SECTION("p0 controllable, p1 controllable") {
    auto partition = InputOutputPartition({"dummy"}, {"p0", "p1"});
    bool result = test_is_realizable(formula, partition);
    REQUIRE(result);
  }
  SECTION("p0 uncontrollable, p1 controllable") {
    auto partition = InputOutputPartition({"p0"}, {"p1"});
    bool result = test_is_realizable(formula, partition);
    REQUIRE(result);
  }
  SECTION("p0 controllable, p1 uncontrollable") {
    auto partition = InputOutputPartition({"p1"}, {"p0"});
    bool result = test_is_realizable(formula, partition);
    REQUIRE(!result);
  }
  SECTION("p0 uncontrollable, p1 uncontrollable") {
    auto partition = InputOutputPartition({"p0", "p1"}, {"dummy"});
    bool result = test_is_realizable(formula, partition);
    REQUIRE(!result);
  }
}
TEST_CASE("forward synthesis of '(X(F(~b))) U (G(a))'") {
  utils::Logger::level(utils::LogLevel::debug);
  auto context = std::make_shared<logic::Context>();
  auto a = context->make_atom("a");
  auto b = context->make_atom("b");
  auto not_b = context->make_prop_not(b);
  auto not_end = context->make_not(context->make_end());
  auto always_a = context->make_eventually(a);
  auto eventually_not_b = context->make_eventually(not_b);
  auto next_eventually = context->make_weak_next(eventually_not_b);
  auto until = context->make_until({next_eventually, always_a});
  auto formula = context->make_and({until, not_end});

  SECTION("a controllable, b controllable") {
    auto partition = InputOutputPartition({"dummy"}, {"a", "b"});
    bool result = test_is_realizable(formula, partition);
    REQUIRE(result);
  }
  SECTION("a uncontrollable, b controllable") {
    auto partition = InputOutputPartition({"a"}, {"b"});
    bool result = test_is_realizable(formula, partition);
    REQUIRE(!result);
  }
  SECTION("a controllable, b uncontrollable") {
    auto partition = InputOutputPartition({"b"}, {"a"});
    bool result = test_is_realizable(formula, partition);
    REQUIRE(result);
  }
  SECTION("a uncontrollable, b uncontrollable") {
    auto partition = InputOutputPartition({"a", "b"}, {"dummy"});
    bool result = test_is_realizable(formula, partition);
    REQUIRE(!result);
  }
}

TEST_CASE("forward synthesis of 'G(b) U F(a)'") {
  utils::Logger::level(utils::LogLevel::debug);
  auto context = std::make_shared<logic::Context>();
  auto a = context->make_atom("a");
  auto b = context->make_atom("b");
  auto not_end = context->make_not(context->make_end());
  auto always_b = context->make_eventually(b);
  auto eventually_a = context->make_eventually(a);
  auto until = context->make_until({always_b, eventually_a});
  auto formula = context->make_and({until, not_end});

  SECTION("a controllable, b controllable") {
    auto partition = InputOutputPartition({"dummy"}, {"a", "b"});
    bool result = test_is_realizable(formula, partition);
    REQUIRE(result);
  }
  SECTION("a uncontrollable, b controllable") {
    auto partition = InputOutputPartition({"a"}, {"b"});
    bool result = test_is_realizable(formula, partition);
    REQUIRE(!result);
  }
  SECTION("a controllable, b uncontrollable") {
    auto partition = InputOutputPartition({"b"}, {"a"});
    bool result = test_is_realizable(formula, partition);
    REQUIRE(result);
  }
  SECTION("a uncontrollable, b uncontrollable") {
    auto partition = InputOutputPartition({"a", "b"}, {"dummy"});
    bool result = test_is_realizable(formula, partition);
    REQUIRE(!result);
  }
}

} // namespace Test
} // namespace core
} // namespace nike