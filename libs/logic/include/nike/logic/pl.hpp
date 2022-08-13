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

#include <algorithm>
#include <nike/logic/base.hpp>
#include <nike/logic/visitable.hpp>
#include <stdexcept>
#include <utility>

namespace nike {
namespace logic {

class PLFormula : public AstNode {
public:
  explicit PLFormula(Context &c) : AstNode(c) {}
};

class PLTrue : public PLFormula {
public:
  const static TypeID type_code_id = TypeID::t_PLTrue;
  explicit PLTrue(Context &ctx) : PLFormula(ctx) {}

  void accept(Visitor &visitor) const override;
  inline TypeID get_type_code() const override;
  inline hash_t compute_hash_() const override;
  bool is_equal(const Comparable &o) const override;
  int compare_(const Comparable &o) const override;
};

class PLFalse : public PLFormula {
public:
  const static TypeID type_code_id = TypeID::t_PLFalse;
  explicit PLFalse(Context &ctx) : PLFormula(ctx) {}

  void accept(Visitor &visitor) const override;
  inline TypeID get_type_code() const override;
  inline hash_t compute_hash_() const override;
  bool is_equal(const Comparable &o) const override;
  int compare_(const Comparable &o) const override;
};

class PLLiteral : public PLFormula {
public:
  const ast_ptr proposition;
  bool negated;
  const static TypeID type_code_id = TypeID::t_PLLiteral;
  PLLiteral(Context &ctx, const ast_ptr &proposition, bool negated)
      : PLFormula(ctx), proposition{proposition}, negated{negated} {}
  PLLiteral(Context &ctx, const std::string &propositionName)
      : PLFormula(ctx), proposition{proposition}, negated{negated} {}

  void accept(Visitor &visitor) const override;
  inline TypeID get_type_code() const override;
  inline hash_t compute_hash_() const override;
  bool is_equal(const Comparable &o) const override;
  int compare_(const Comparable &o) const override;
};

class PLBooleanBinaryOp {
public:
  bool (&fun)(bool, bool);

  explicit PLBooleanBinaryOp(bool (&fun)(bool, bool)) : fun{fun} {}
};

class PLBinaryOp : public PLFormula {
public:
  const vec_pl_ptr args;

  PLBinaryOp(Context &ctx, const vec_pl_ptr &args)
      : PLFormula(ctx), args{args} {
    if (args.size() < 2) {
      throw std::invalid_argument(
          "the number of arguments must not be less than two");
    }
  }

  PLBinaryOp(Context &ctx, const set_pl_ptr &args)
      : PLFormula(ctx), args{args.begin(), args.end()} {
    if (args.size() < 2) {
      throw std::invalid_argument(
          "the number of arguments must not be less than two");
    }
  }

  inline hash_t compute_hash_() const override;
  bool is_equal(const Comparable &o) const override;
  int compare_(const Comparable &o) const override;
};

class PLCommutativeIdempotentBinaryOp : public PLBinaryOp {
public:
  PLCommutativeIdempotentBinaryOp(Context &ctx, vec_pl_ptr args)
      : PLBinaryOp(
            ctx, utils::setify<pl_ptr, utils::Deref::Equal, utils::Deref::Less>(
                     args)) {}
  PLCommutativeIdempotentBinaryOp(Context &ctx, const set_pl_ptr &args)
      : PLBinaryOp(ctx, args) {}
};

class PLAnd : public PLCommutativeIdempotentBinaryOp, public PLBooleanBinaryOp {
private:
  static inline bool and_(bool b1, bool b2) { return utils::and_(b1, b2); }

public:
  const static TypeID type_code_id = TypeID::t_PLAnd;
  PLAnd(Context &ctx, vec_pl_ptr args)
      : PLCommutativeIdempotentBinaryOp(ctx, std::move(args)),
        PLBooleanBinaryOp(and_) {}
  PLAnd(Context &ctx, const set_pl_ptr &args)
      : PLCommutativeIdempotentBinaryOp(ctx, args), PLBooleanBinaryOp(and_) {}

  void accept(Visitor &visitor) const override;
  inline TypeID get_type_code() const override;
};

class PLOr : public PLCommutativeIdempotentBinaryOp, public PLBooleanBinaryOp {
private:
  static inline bool or_(bool b1, bool b2) { return utils::or_(b1, b2); }

public:
  const static TypeID type_code_id = TypeID::t_PLOr;
  PLOr(Context &ctx, vec_pl_ptr args)
      : PLCommutativeIdempotentBinaryOp(ctx, std::move(args)),
        PLBooleanBinaryOp(or_) {}
  PLOr(Context &ctx, const set_pl_ptr &args)
      : PLCommutativeIdempotentBinaryOp(ctx, args), PLBooleanBinaryOp(or_) {}

  void accept(Visitor &visitor) const override;
  inline TypeID get_type_code() const override;
};

inline TypeID PLTrue::get_type_code() const { return TypeID::t_PLTrue; }
inline TypeID PLFalse::get_type_code() const { return TypeID::t_PLFalse; }
inline TypeID PLLiteral::get_type_code() const { return TypeID::t_PLLiteral; }
inline TypeID PLAnd::get_type_code() const { return TypeID::t_PLAnd; }
inline TypeID PLOr::get_type_code() const { return TypeID::t_PLOr; }

inline hash_t PLTrue::compute_hash_() const { return type_code_id; }
inline hash_t PLFalse::compute_hash_() const { return type_code_id; }
inline hash_t PLLiteral::compute_hash_() const {
  hash_t result = type_code_id;
  hash_combine(result, negated);
  hash_combine(result, *proposition);
  return result;
}

inline hash_t PLBinaryOp::compute_hash_() const {
  hash_t result = get_type_code();
  auto first = args.begin();
  auto last = args.end();
  for (; first < last; ++first) {
    hash_combine(result, **first);
  }
  return result;
}

} // namespace logic
} // namespace nike