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
set(MY_CMAKE_TEMPLATE_APP1_APP_NAME my-cmake-template-app1)
project(${MY_CMAKE_TEMPLATE_APP1_APP_NAME})

#configure directories
set (MY_CMAKE_TEMPLATE_APP1_APP_MODULE_PATH ${PROJECT_SOURCE_DIR}/)
set (MY_CMAKE_TEMPLATE_APP1_APP_SRC_PATH  "${MY_CMAKE_TEMPLATE_APP1_APP_MODULE_PATH}/src" )
set (MY_CMAKE_TEMPLATE_APP1_APP_INCLUDE_PATH  "${MY_CMAKE_TEMPLATE_APP1_APP_MODULE_PATH}/include")

include_directories (${MY_CMAKE_TEMPLATE_LIB1_LIB_INCLUDE_PATH} ${VENDOR_INCLUDE_PATH})

add_executable(${MY_CMAKE_TEMPLATE_APP1_APP_NAME} main.cpp)
target_link_libraries(${MY_CMAKE_TEMPLATE_APP1_APP_NAME}
        PRIVATE # ...
    )
target_include_directories(${MY_CMAKE_TEMPLATE_APP1_APP_NAME}
        PRIVATE ${VENDOR_INCLUDE_PATH})
