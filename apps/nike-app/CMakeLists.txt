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

cmake_minimum_required(VERSION 3.20 FATAL_ERROR)
set(NIKE_APP_NAME nike-app)
project(${NIKE_APP_NAME})

#configure directories
set (NIKE_APP_MODULE_PATH ${PROJECT_SOURCE_DIR}/)
set (NIKE_APP_SRC_PATH  "${NIKE_APP_MODULE_PATH}/src" )
set (NIKE_APP_INCLUDE_PATH  "${NIKE_APP_MODULE_PATH}/include")

include_directories (${NIKE_CORE_INCLUDE_PATH} ${NIKE_PARSER_INCLUDE_PATH} ${NIKE_LOGIC_INCLUDE_PATH} ${NIKE_UTILS_INCLUDE_PATH} ${THIRD_PARTY_INCLUDE_PATH})

add_executable(${NIKE_APP_NAME} main.cpp)
target_link_libraries(${NIKE_APP_NAME}
        PRIVATE ${NIKE_CORE_LIB_NAME}
    )
target_include_directories(${NIKE_APP_NAME}
        PRIVATE ${VENDOR_INCLUDE_PATH}  ${NIKE_CORE_INCLUDE_PATH})
