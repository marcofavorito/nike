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

cmake_minimum_required(VERSION 3.20 FATAL_ERROR)
set(MY_CMAKE_TEMPLATE_LIB1_LIB_NAME my-cmake-template-lib1)

project(${MY_CMAKE_TEMPLATE_LIB1_LIB_NAME})


#configure directories
set (MY_CMAKE_TEMPLATE_LIB1_LIB_MODULE_PATH ${PROJECT_SOURCE_DIR})
set (MY_CMAKE_TEMPLATE_LIB1_LIB_SRC_PATH  "${MY_CMAKE_TEMPLATE_LIB1_LIB_MODULE_PATH}/src" )
set (MY_CMAKE_TEMPLATE_LIB1_LIB_INCLUDE_PATH  "${MY_CMAKE_TEMPLATE_LIB1_LIB_MODULE_PATH}/include")

#set includes
include_directories (${MY_CMAKE_TEMPLATE_LIB1_LIB_INCLUDE_PATH} ${VENDOR_INCLUDE_PATH})

#set sources
file (GLOB_RECURSE MY_CMAKE_TEMPLATE_LIB1_LIB_HEADER_FILES "${MY_CMAKE_TEMPLATE_LIB1_LIB_INCLUDE_PATH}/*.hpp" ${MY_CMAKE_TEMPLATE_LIB1_LIB_INCLUDE_PATH}/*.h ${MY_CMAKE_TEMPLATE_LIB1_LIB_INCLUDE_PATH}/*.hh)
file (GLOB_RECURSE MY_CMAKE_TEMPLATE_LIB1_LIB_SOURCE_FILES "${MY_CMAKE_TEMPLATE_LIB1_LIB_SRC_PATH}/*.cpp" ${MY_CMAKE_TEMPLATE_LIB1_LIB_SRC_PATH}/*.cpp ${MY_CMAKE_TEMPLATE_LIB1_LIB_SRC_PATH}/*.cc)


# ------------------------------------------------------------------------------
# Main Library Target
# ------------------------------------------------------------------------------

add_library (${MY_CMAKE_TEMPLATE_LIB1_LIB_NAME} STATIC ${MY_CMAKE_TEMPLATE_LIB1_LIB_SOURCE_FILES} ${MY_CMAKE_TEMPLATE_LIB1_LIB_HEADER_FILES})

#add the library
#target_link_libraries (${MY_CMAKE_TEMPLATE_LIB1_LIB_NAME} ...)
add_subdirectory(tests)

#export vars (globally)
set (MY_CMAKE_TEMPLATE_LIB1_LIB_NAME  ${MY_CMAKE_TEMPLATE_LIB1_LIB_NAME} CACHE INTERNAL "MY_CMAKE_TEMPLATE_LIB1_LIB_NAME")
set (MY_CMAKE_TEMPLATE_LIB1_LIB_INCLUDE_PATH  ${MY_CMAKE_TEMPLATE_LIB1_LIB_INCLUDE_PATH}  CACHE INTERNAL "MY_CMAKE_TEMPLATE_LIB1_LIB_INCLUDE_PATH")
