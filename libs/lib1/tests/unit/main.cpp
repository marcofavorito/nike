/*
 * This file is part of MyCMakeTemplate.
 *
 * MyCMakeTemplate is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * MyCMakeTemplate is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with MyCMakeTemplate.  If not, see <https://www.gnu.org/licenses/>.
 */

#define CATCH_CONFIG_MAIN // This tells Catch to provide a main() - only do this
// in one cpp file
#include <catch.hpp>

#include <my_cmake_template/lib1.hpp>

namespace my_cmake_template {
namespace lib1 {
namespace Test {

TEST_CASE("Test example", "[lib1]") { REQUIRE(true); }

} // namespace Test
} // namespace lib1
} // namespace my_cmake_template