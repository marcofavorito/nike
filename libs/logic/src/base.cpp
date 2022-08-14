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

#include <nike/logic/base.hpp>
#include <nike/logic/ltlf.hpp>
#include <nike/logic/pl.hpp>
#include <stdexcept>

// TODO instead of make_shared, use "signature" of object to instantiate
//        so to avoid heap allocation.

namespace nike {
namespace logic {

bool StringSymbol::is_equal(const Comparable &o) const {
  if (is_a<StringSymbol>(o))
    return name == dynamic_cast<const StringSymbol &>(o).name;
  return false;
}
int StringSymbol::compare_(const Comparable &o) const {
  assert(is_a<StringSymbol>(o));
  const auto &s = dynamic_cast<const StringSymbol &>(o);
  if (name == s.name)
    return 0;
  return name < s.name ? -1 : 1;
}

Context::Context() {
  table_ = utils::make_unique<HashTable>();

  tt = std::make_shared<const LTLfTrue>(*this);
  table_->insert_if_not_available(tt);

  ff = std::make_shared<const LTLfFalse>(*this);
  table_->insert_if_not_available(ff);

  prop_true = std::make_shared<const LTLfPropTrue>(*this);
  table_->insert_if_not_available(prop_true);

  prop_false = std::make_shared<const LTLfPropFalse>(*this);
  table_->insert_if_not_available(prop_false);

  end = std::make_shared<const LTLfAlways>(*this, ff);
  table_->insert_if_not_available(end);

  not_end = std::make_shared<const LTLfEventually>(*this, tt);
  table_->insert_if_not_available(not_end);

  last = std::make_shared<const LTLfWeakNext>(*this, ff);
  table_->insert_if_not_available(last);

  true_ = std::make_shared<const PLTrue>(*this);
  table_->insert_if_not_available(true_);

  false_ = std::make_shared<const PLFalse>(*this);
  table_->insert_if_not_available(false_);
}

ltlf_ptr Context::make_tt() { return tt; }
ltlf_ptr Context::make_ff() { return ff; }
ltlf_ptr Context::make_prop_true() { return prop_true; }
ltlf_ptr Context::make_prop_false() { return prop_false; }
ltlf_ptr Context::make_end() { return end; }
ltlf_ptr Context::make_not_end() { return not_end; }
ltlf_ptr Context::make_last() { return last; }
ltlf_ptr Context::make_bool(bool value) {
  return value ? make_tt() : make_ff();
}
pl_ptr Context::make_prop_bool(bool value) {
  return value ? make_true() : make_false();
}

ltlf_ptr Context::make_atom(const std::string &name) {
  auto atom = std::make_shared<const LTLfAtom>(*this, name);
  auto actual_atom = table_->insert_if_not_available(atom);
  return actual_atom;
}

ltlf_ptr Context::make_atom(const ast_ptr &symbol) {
  auto atom = std::make_shared<const LTLfAtom>(*this, symbol);
  auto actual_atom = table_->insert_if_not_available(atom);
  return actual_atom;
}

ltlf_ptr Context::make_not_unified(const ltlf_ptr &arg) {
  // if argument is literal, use propositional not
  // othwerwise, return logical not
  if (is_a<LTLfAtom>(*arg) or is_a<LTLfPropositionalNot>(*arg)) {
    return make_prop_not(arg);
  }
  return make_not(arg);
}
ltlf_ptr Context::make_not(const ltlf_ptr &arg) {
  auto negation = std::make_shared<const LTLfNot>(*this, arg);
  auto actual = table_->insert_if_not_available(negation);
  return actual;
}

ltlf_ptr Context::make_prop_not(const ltlf_ptr &arg) {
  // !(!a) = a
  if (is_a<LTLfPropositionalNot>(*arg)) {
    return table_->insert_if_not_available(
        std::static_pointer_cast<const LTLfPropositionalNot>(arg)->arg);
  }
  // argument must be an atom
  auto negation = std::make_shared<const LTLfPropositionalNot>(*this, arg);
  auto actual = table_->insert_if_not_available(negation);
  return actual;
}

ltlf_ptr Context::make_and(const vec_ptr &args) {
  ltlf_ptr (Context::*fun)(bool) = &Context::make_bool;
  auto tmp =
      and_or<const LTLfFormula, LTLfAnd, LTLfTrue, LTLfFalse, LTLfAnd, LTLfOr>(
          *this, args, false, fun);
  auto actual = table_->insert_if_not_available(tmp);
  return actual;
}

ltlf_ptr Context::make_or(const vec_ptr &args) {
  ltlf_ptr (Context::*fun)(bool) = &Context::make_bool;
  auto tmp =
      and_or<const LTLfFormula, LTLfOr, LTLfTrue, LTLfFalse, LTLfAnd, LTLfOr>(
          *this, args, true, fun);
  auto actual = table_->insert_if_not_available(tmp);
  return actual;
}

ltlf_ptr Context::make_implies(const vec_ptr &args) {
  auto implies = std::make_shared<const LTLfImplies>(*this, args);
  auto actual = table_->insert_if_not_available(implies);
  return actual;
}

ltlf_ptr Context::make_equivalent(const vec_ptr &args) {
  auto equivalent = std::make_shared<const LTLfEquivalent>(*this, args);
  auto actual = table_->insert_if_not_available(equivalent);
  return actual;
}

ltlf_ptr Context::make_xor(const vec_ptr &args) {
  auto equivalent = std::make_shared<const LTLfXor>(*this, args);
  auto actual = table_->insert_if_not_available(equivalent);
  return actual;
}

ltlf_ptr Context::make_next(const ltlf_ptr &arg) {
  auto next = std::make_shared<const LTLfNext>(*this, arg);
  auto actual = table_->insert_if_not_available(next);
  return actual;
}

ltlf_ptr Context::make_weak_next(const ltlf_ptr &arg) {
  auto next = std::make_shared<const LTLfWeakNext>(*this, arg);
  auto actual = table_->insert_if_not_available(next);
  return actual;
}

ltlf_ptr Context::make_until(const vec_ptr &args) {
  auto until = std::make_shared<const LTLfUntil>(*this, args);
  auto actual = table_->insert_if_not_available(until);
  return actual;
}

ltlf_ptr Context::make_release(const vec_ptr &args) {
  auto release = std::make_shared<const LTLfRelease>(*this, args);
  auto actual = table_->insert_if_not_available(release);
  return actual;
}

ltlf_ptr Context::make_eventually(const ltlf_ptr &arg) {
  auto eventually = std::make_shared<const LTLfEventually>(*this, arg);
  auto actual = table_->insert_if_not_available(eventually);
  return actual;
}

ltlf_ptr Context::make_always(const ltlf_ptr &arg) {
  auto always = std::make_shared<const LTLfAlways>(*this, arg);
  auto actual = table_->insert_if_not_available(always);
  return actual;
}

ast_ptr Context::make_string_symbol(const std::string &arg) {
  auto stringSymbol = std::make_shared<const StringSymbol>(*this, arg);
  auto actual = table_->insert_if_not_available(stringSymbol);
  return actual;
}

pl_ptr Context::make_true() { return true_; }
pl_ptr Context::make_false() { return false_; }
pl_ptr Context::make_literal(const ast_ptr &symbol, bool negated) {
  auto literal = std::make_shared<const PLLiteral>(*this, symbol, negated);
  auto actual = table_->insert_if_not_available(literal);
  return literal;
}
pl_ptr Context::make_prop_and(const vec_pl_ptr &args) {
  pl_ptr (Context::*fun)(bool) = &Context::make_prop_bool;
  auto tmp = and_or_prop<const PLFormula, PLAnd, PLTrue, PLFalse, PLAnd, PLOr>(
      *this, args, false, fun);
  auto actual = table_->insert_if_not_available(tmp);
  return actual;
}
pl_ptr Context::make_prop_or(const vec_pl_ptr &args) {
  pl_ptr (Context::*fun)(bool) = &Context::make_prop_bool;
  auto tmp = and_or_prop<const PLFormula, PLOr, PLTrue, PLFalse, PLAnd, PLOr>(
      *this, args, true, fun);
  auto actual = table_->insert_if_not_available(tmp);
  return actual;
}

} // namespace logic
} // namespace nike