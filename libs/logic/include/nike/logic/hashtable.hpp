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

#include <memory>
#include <nike/logic/types.hpp>
#include <nike/utils.hpp>
#include <unordered_set>

namespace nike {
namespace logic {

/*
 * A hash table for AST nodes based on STL unordered_set.
 */
class HashTable {
private:
  std::unordered_set<ast_ptr, utils::Deref::Hash, utils::EqualOrDeref> m_table_;

public:
  explicit HashTable() {
    m_table_ =
        std::unordered_set<ast_ptr, utils::Deref::Hash, utils::EqualOrDeref>{};
  }

  template <typename T>
  std::shared_ptr<const T>
  insert_if_not_available(const std::shared_ptr<const T> &ptr) {
    auto it = m_table_.find(ptr);
    if (it == m_table_.end()) {
      m_table_.insert(ptr);
      return ptr;
    } else {
      return std::static_pointer_cast<const T>(*it);
    }
  }

  size_t size() { return m_table_.size(); }
};

} // namespace logic
} // namespace nike