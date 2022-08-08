#
# This file is part of MyCMakeTemplate.
#
# MyCMakeTemplate is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# MyCMakeTemplate is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with MyCMakeTemplate.  If not, see <https://www.gnu.org/licenses/>.
#

# CMake build : third party

#configure directories
set (VENDOR_MODULE_PATH "${PROJECT_SOURCE_DIR}/vendor")

# -------------------------------------------------------------------------
# catch
set (CATCH_MODULE_PATH "${VENDOR_MODULE_PATH}/Catch2")
set (CATCH_INCLUDE_PATH "${CATCH_MODULE_PATH}/single_include/catch2")
add_subdirectory(Catch2)
list(APPEND CMAKE_MODULE_PATH ${CATCH_MODULE_PATH}/contrib)
include(CTest)
include(Catch)
# -------------------------------------------------------------------------
# -------------------------------------------------------------------------
# spdlog
set (SPDLOG_MODULE_PATH "${VENDOR_MODULE_PATH}/spdlog")
set (SPDLOG_INCLUDE_PATH "${SPDLOG_MODULE_PATH}/include")
# -------------------------------------------------------------------------

#set variables
set (VENDOR_INCLUDE_PATH ${SPDLOG_INCLUDE_PATH})

#set variables for tests
set (TEST_VENDOR_INCLUDE_PATH ${CATCH_INCLUDE_PATH})

#export vars
set (VENDOR_INCLUDE_PATH  ${VENDOR_INCLUDE_PATH} PARENT_SCOPE)
set (TEST_VENDOR_INCLUDE_PATH  ${TEST_VENDOR_INCLUDE_PATH} PARENT_SCOPE)
