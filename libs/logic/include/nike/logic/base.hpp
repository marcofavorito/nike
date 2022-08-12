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

#include <cassert>
#include <memory>
#include <nike/logic/comparable.hpp>
#include <nike/logic/hashable.hpp>
#include <nike/logic/hashtable.hpp>
#include <nike/logic/visitable.hpp>
#include <nike/utils.hpp>
#include <utility>

namespace nike {
namespace logic {

class Context;

class AstNode : public Visitable,
                public Hashable,
                public Comparable,
                public std::enable_shared_from_this<const AstNode> {
private:
  Context *m_ctx_;

public:
  explicit AstNode(Context &ctx) : m_ctx_{&ctx} {}
  Context &ctx() const { return *m_ctx_; }
  friend void check_context(AstNode const &a, AstNode const &b) {
    assert(a.m_ctx_ == b.m_ctx_);
  };
};

class StringSymbol : public AstNode {
public:
  const std::string name;
  const static TypeID type_code_id = TypeID::t_StringSymbol;
  StringSymbol(Context &ctx, const std::string &name)
      : AstNode(ctx), name{name} {}

  void accept(Visitor &visitor) const override;
  inline TypeID get_type_code() const override;
  inline hash_t compute_hash_() const override;
  bool is_equal(const Comparable &o) const override;
  int compare_(const Comparable &o) const override;
};

class Context {
private:
  std::unique_ptr<HashTable> table_;

  ltlf_ptr tt;
  ltlf_ptr ff;
  ltlf_ptr prop_true;
  ltlf_ptr prop_false;
  ltlf_ptr end;
  ltlf_ptr not_end;
  ltlf_ptr last;
  pl_ptr true_;
  pl_ptr false_;

public:
  Context();
  ast_ptr make_string_symbol(const std::string &);
  ltlf_ptr make_tt();
  ltlf_ptr make_ff();
  ltlf_ptr make_prop_true();
  ltlf_ptr make_prop_false();
  ltlf_ptr make_end();
  ltlf_ptr make_not_end();
  ltlf_ptr make_last();
  ltlf_ptr make_bool(bool value);
  ltlf_ptr make_atom(const std::string &name);
  ltlf_ptr make_atom(const ast_ptr &symbol);
  ltlf_ptr make_not(const ltlf_ptr &arg);
  ltlf_ptr make_prop_not(const ltlf_ptr &arg);
  ltlf_ptr make_not_unified(const ltlf_ptr &arg);
  ltlf_ptr make_and(const vec_ptr &args);
  ltlf_ptr make_or(const vec_ptr &arg);
  ltlf_ptr make_implies(const vec_ptr &arg);
  ltlf_ptr make_equivalent(const vec_ptr &arg);
  ltlf_ptr make_xor(const vec_ptr &arg);
  ltlf_ptr make_next(const ltlf_ptr &arg);
  ltlf_ptr make_weak_next(const ltlf_ptr &arg);
  ltlf_ptr make_until(const vec_ptr &args);
  ltlf_ptr make_release(const vec_ptr &args);
  ltlf_ptr make_eventually(const ltlf_ptr &args);
  ltlf_ptr make_always(const ltlf_ptr &args);

  pl_ptr make_prop_bool(bool value);
  pl_ptr make_true();
  pl_ptr make_false();
  pl_ptr make_literal(const ast_ptr &symbol, bool negated);
  pl_ptr make_prop_and(const vec_pl_ptr &args);
  pl_ptr make_prop_or(const vec_pl_ptr &arg);
};

template <typename T, typename caller, typename True, typename False,
          typename And, typename Or>
std::shared_ptr<T>
and_or(Context &context, const std::vector<std::shared_ptr<const T>> &s,
       bool op_x_notx, std::shared_ptr<T> (Context::*const &fun_ptr)(bool x)) {
  std::set<std::shared_ptr<const T>, utils::Deref::Less> args;
  for (auto &a : s) {
    // handle the case when a subformula is true
    if (is_a<True>(*a)) {
      if (op_x_notx)
        return a;
      else
        continue;
    }
    // handle the case when a subformula is false
    else if (is_a<False>(*a)) {
      if (!op_x_notx)
        return a;
      else
        continue;
    }
    // handle the case when a subformula is of the same type of the caller
    else if (is_a<caller>(*a)) {
      const auto &to_insert = dynamic_cast<const caller &>(*a);
      const auto &container = to_insert.args;
      args.insert(container.begin(), container.end());
      continue;
    } else {
      args.insert(a);
    }
  }
  if (args.size() == 1)
    return *(args.begin());
  if (args.empty())
    return (context.*fun_ptr)(not op_x_notx);
  return std::make_shared<caller>(context, args);
}

inline TypeID StringSymbol::get_type_code() const {
  return TypeID::t_StringSymbol;
}
inline hash_t StringSymbol::compute_hash_() const {
  hash_t result = get_type_code();
  hash_combine(result, name);
  return result;
}

} // namespace logic
} // namespace nike

// injected in namespace std custom specialization of std::hash for AstNode
namespace std {
template <> struct hash<nike::logic::AstNode> {
  std::size_t operator()(nike::logic::AstNode const &n) const noexcept {
    return n.hash();
  }
};
} // namespace std