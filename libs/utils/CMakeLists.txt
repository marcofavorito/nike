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

cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
set(NIKE_UTILS_LIB_NAME nike-utils)

project(${NIKE_UTILS_LIB_NAME})


#configure directories
set (NIKE_UTILS_MODULE_PATH ${PROJECT_SOURCE_DIR})
set (NIKE_UTILS_SRC_PATH  "${NIKE_UTILS_MODULE_PATH}/src" )
set (NIKE_UTILS_INCLUDE_PATH  "${NIKE_UTILS_MODULE_PATH}/include")

#set includes
include_directories (${NIKE_UTILS_INCLUDE_PATH} ${VENDOR_INCLUDE_PATH})

#set sources
file (GLOB_RECURSE NIKE_UTILS_HEADER_FILES "${NIKE_UTILS_INCLUDE_PATH}/*.hpp" ${NIKE_UTILS_INCLUDE_PATH}/*.h ${NIKE_UTILS_INCLUDE_PATH}/*.hh)
file (GLOB_RECURSE NIKE_UTILS_SOURCE_FILES "${NIKE_UTILS_SRC_PATH}/*.cpp" ${NIKE_UTILS_INCLUDE_PATH}/*.cpp ${NIKE_UTILS_INCLUDE_PATH}/*.cc)


# ------------------------------------------------------------------------------
# Main Library Target
# ------------------------------------------------------------------------------

add_library (${NIKE_UTILS_LIB_NAME}
        STATIC
            ${NIKE_UTILS_SOURCE_FILES}
            ${NIKE_UTILS_HEADER_FILES})
target_link_libraries(${NIKE_UTILS_LIB_NAME} ${CUDD_LIBRARIES})
set_target_properties(${NIKE_UTILS_LIB_NAME} PROPERTIES LINKER_LANGUAGE CXX)

add_subdirectory(tests)

#export vars (globally)
set (NIKE_UTILS_LIB_NAME  ${NIKE_UTILS_LIB_NAME} CACHE INTERNAL "NIKE_UTILS_LIB_NAME")
set (NIKE_UTILS_INCLUDE_PATH  ${NIKE_UTILS_INCLUDE_PATH}  CACHE INTERNAL "NIKE_UTILS_INCLUDE_PATH")
