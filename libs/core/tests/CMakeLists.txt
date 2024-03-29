#
# This file is part of Nike.
#
# Nike is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Nike is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Nike.  If not, see <https://www.gnu.org/licenses/>.
#

#configure variables
set (TEST_APP_NAME "nike-core-test")

#configure directories
set (TEST_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}")

#configure test directories
set (TEST_UNIT_SRC_PATH  "${TEST_MODULE_PATH}/unit" )
set (TEST_INTEGRATION_SRC_PATH  "${TEST_MODULE_PATH}/integration" )

#set includes
include_directories (${NIKE_CORE_INCLUDE_PATH} ${TEST_VENDOR_INCLUDE_PATH})

#set test sources
file (GLOB_RECURSE TEST_SOURCE_FILES "${TEST_UNIT_SRC_PATH}/*.cpp" "${TEST_INTEGRATION_SRC_PATH}/*.cpp")
file (GLOB_RECURSE TEST_HEADER_FILES "${TEST_UNIT_SRC_PATH}/*.hpp" "${TEST_INTEGRATION_SRC_PATH}/*.hpp")

#set target executable
add_executable (${TEST_APP_NAME} ${TEST_SOURCE_FILES} ${TEST_HEADER_FILES})

#add the library
target_link_libraries (${TEST_APP_NAME}
        PRIVATE
            Catch2::Catch2
            ${NIKE_CORE_LIB_NAME})

# Turn on CMake testing capabilities
enable_testing()

catch_discover_tests(${TEST_APP_NAME})
