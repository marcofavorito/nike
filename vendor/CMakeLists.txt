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
# -------------------------------------------------------------------------
# CLI11
set (CLI11_MODULE_PATH "${VENDOR_MODULE_PATH}/cli11")
set (CLI11_INCLUDE_PATH "${CLI11_MODULE_PATH}/include")
# -------------------------------------------------------------------------

# -------------------------------------------------------------------------
# -------------------------------------------------------------------------
# Glucose
ExternalProject_Add(glucose
    GIT_REPOSITORY https://github.com/whitemech/glucose.git
    GIT_TAG v4.2.1
    GIT_PROGRESS true
    CMAKE_ARGS
    -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
    -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
    -DCMAKE_INSTALL_PREFIX=${USR_DIR}
    -DCMAKE_BUILD_TYPE=Release
    INSTALL_COMMAND ""
)

ExternalProject_Get_Property(glucose SOURCE_DIR BINARY_DIR)
set(LIB_GLUCOSE_SOURCE_DIR ${SOURCE_DIR})
set(LIB_GLUCOSE_BINARY_DIR ${BINARY_DIR})

set(GLUCOSE_LIBRARIES ${LIB_GLUCOSE_BINARY_DIR}/libglucose.a ${LIB_GLUCOSE_BINARY_DIR}/libglucosep.a)

set(LIB_GLUCOSE_INCLUDE_DIR ${LIB_GLUCOSE_SOURCE_DIR}/.. ${LIB_GLUCOSE_SOURCE_DIR})


add_custom_target(glucose-libraries DEPENDS glucose ${GLUCOSE_LIBRARIES})
# -------------------------------------------------------------------------


#set variables
set (VENDOR_INCLUDE_PATH ${SPDLOG_INCLUDE_PATH} ${CLI11_INCLUDE_PATH} ${LIB_GLUCOSE_INCLUDE_DIR})

#set variables for tests
set (TEST_VENDOR_INCLUDE_PATH ${CATCH_INCLUDE_PATH})

#export vars
set (VENDOR_INCLUDE_PATH  ${VENDOR_INCLUDE_PATH} PARENT_SCOPE)
set (TEST_VENDOR_INCLUDE_PATH  ${TEST_VENDOR_INCLUDE_PATH} PARENT_SCOPE)

set (GLUCOSE_LIBRARIES ${GLUCOSE_LIBRARIES} PARENT_SCOPE)