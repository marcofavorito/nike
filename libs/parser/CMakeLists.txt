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
set(NIKE_PARSER_LIB_NAME nike-parser)

project(${NIKE_PARSER_LIB_NAME})


#configure directories
set (NIKE_PARSER_MODULE_PATH ${PROJECT_SOURCE_DIR})
set (NIKE_PARSER_SRC_PATH  "${NIKE_PARSER_MODULE_PATH}/src" )
set (NIKE_PARSER_INCLUDE_PATH  "${NIKE_PARSER_MODULE_PATH}/include")


## generate lexer and parser
FLEX_TARGET(LTLfLexer ${NIKE_PARSER_INCLUDE_PATH}/nike/parser/lexer.l ${NIKE_PARSER_INCLUDE_PATH}/nike/parser/lexer.yy.cc)
BISON_TARGET(LTLfParser ${NIKE_PARSER_INCLUDE_PATH}/nike/parser/parser.yy ${NIKE_PARSER_INCLUDE_PATH}/nike/parser/parser.tab.cc )
ADD_FLEX_BISON_DEPENDENCY(LTLfLexer LTLfParser)


#set includes
include_directories (${NIKE_PARSER_INCLUDE_PATH} ${NIKE_LOGIC_INCLUDE_PATH} ${NIKE_UTILS_INCLUDE_PATH} ${VENDOR_INCLUDE_PATH})

#set sources
file (GLOB_RECURSE NIKE_PARSER_HEADER_FILES "${NIKE_PARSER_INCLUDE_PATH}/*.hpp" ${NIKE_PARSER_INCLUDE_PATH}/*.h ${NIKE_PARSER_INCLUDE_PATH}/*.hh)
file (GLOB_RECURSE NIKE_PARSER_SOURCE_FILES "${NIKE_PARSER_SRC_PATH}/*.cpp" ${NIKE_PARSER_INCLUDE_PATH}/*.cpp ${NIKE_PARSER_INCLUDE_PATH}/*.cc)


# ------------------------------------------------------------------------------
# Main Library Target
# ------------------------------------------------------------------------------

add_library (${NIKE_PARSER_LIB_NAME}
        STATIC
            ${NIKE_PARSER_SOURCE_FILES}
            ${NIKE_PARSER_HEADER_FILES}
            ${BISON_LTLfParser_OUTPUTS}
            ${FLEX_LTLfLexer_OUTPUTS})
target_link_libraries(${NIKE_PARSER_LIB_NAME}
        ${FLEX_LIBRARIES}
        ${BISON_LIBRARIES}
        ${NIKE_LOGIC_LIB_NAME})

add_subdirectory(tests)

#export vars (globally)
set (NIKE_PARSER_LIB_NAME  ${NIKE_PARSER_LIB_NAME} CACHE INTERNAL "NIKE_PARSER_LIB_NAME")
set (NIKE_PARSER_INCLUDE_PATH  ${NIKE_PARSER_INCLUDE_PATH}  CACHE INTERNAL "NIKE_PARSER_INCLUDE_PATH")
