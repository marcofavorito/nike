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

#include <cstdint>
#include <map>
#include <memory>
#include <nike/utils.hpp>
#include <set>
#include <vector>

namespace nike {
namespace logic {
class AstNode;
class PLFormula;
class LTLfFormula;
class LTLfAtom;

typedef std::size_t hash_t;
typedef std::shared_ptr<const AstNode> ast_ptr;
typedef std::shared_ptr<const PLFormula> pl_ptr;
typedef std::shared_ptr<const LTLfFormula> ltlf_ptr;
typedef std::shared_ptr<const LTLfAtom> atom_ptr;
typedef std::vector<pl_ptr> vec_pl_ptr;
typedef std::vector<ltlf_ptr> vec_ptr;
typedef std::set<pl_ptr, utils::Deref::Less> set_pl_ptr;
typedef std::set<ltlf_ptr, utils::Deref::Less> set_ptr;
typedef std::set<atom_ptr, utils::Deref::Less> set_atoms_ptr;
typedef std::set<ast_ptr, utils::Deref::Less> set_ast_ptr;
typedef std::map<ltlf_ptr, size_t, utils::Deref::Less> map_ptr;

} // namespace logic

} // namespace nike
