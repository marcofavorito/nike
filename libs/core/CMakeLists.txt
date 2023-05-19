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
set(NIKE_CORE_LIB_NAME nike-core)

project(${NIKE_CORE_LIB_NAME})


#configure directories
set (NIKE_CORE_MODULE_PATH ${PROJECT_SOURCE_DIR})
set (NIKE_CORE_SRC_PATH  "${NIKE_CORE_MODULE_PATH}/src" )
set (NIKE_CORE_INCLUDE_PATH  "${NIKE_CORE_MODULE_PATH}/include")

#set includes
include_directories (${NIKE_CORE_INCLUDE_PATH}
        ${NIKE_UTILS_INCLUDE_PATH}
        ${NIKE_PARSER_INCLUDE_PATH}
        ${NIKE_LOGIC_INCLUDE_PATH}
        ${VENDOR_INCLUDE_PATH}
        ${CUDD_INCLUDE_DIRS})

#set sources
file (GLOB_RECURSE NIKE_CORE_HEADER_FILES "${NIKE_CORE_INCLUDE_PATH}/*.hpp" ${NIKE_CORE_INCLUDE_PATH}/*.h ${NIKE_CORE_INCLUDE_PATH}/*.hh)
file (GLOB_RECURSE NIKE_CORE_SOURCE_FILES "${NIKE_CORE_SRC_PATH}/*.cpp" ${NIKE_CORE_INCLUDE_PATH}/*.cpp ${NIKE_CORE_INCLUDE_PATH}/*.cc)


# ------------------------------------------------------------------------------
# Main Library Target
# ------------------------------------------------------------------------------

add_library (${NIKE_CORE_LIB_NAME} STATIC ${NIKE_CORE_SOURCE_FILES} ${NIKE_CORE_HEADER_FILES})

#add the library
target_link_libraries (${NIKE_CORE_LIB_NAME}
        ${NIKE_UTILS_LIB_NAME}
        ${NIKE_LOGIC_LIB_NAME}
        ${NIKE_PARSER_LIB_NAME}
        ${CUDD_LIBRARIES}
        ${GLUCOSE_LIBRARIES}
        ${Z3_LIBRARIES}
    )

add_dependencies(
    ${NIKE_CORE_LIB_NAME}
    glucose-libraries
)

add_subdirectory(tests)

#export vars (globally)
set (NIKE_CORE_LIB_NAME  ${NIKE_CORE_LIB_NAME} CACHE INTERNAL "NIKE_CORE_LIB_NAME")
set (NIKE_CORE_INCLUDE_PATH  ${NIKE_CORE_INCLUDE_PATH}  CACHE INTERNAL "NIKE_CORE_INCLUDE_PATH")
