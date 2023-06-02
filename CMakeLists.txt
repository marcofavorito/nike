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
project(nike
        LANGUAGES CXX
        HOMEPAGE_URL https://github.com/marcofavorito/nike.git
        DESCRIPTION "A CMake project template.")
set(PROJECT_VERSION "0.1.0")
set(${PROJECT_NAME}_VERSION ${PROJECT_VERSION})

add_definitions( -DNIKE_VERSION="${PROJECT_VERSION}" )
set_property (GLOBAL PROPERTY USE_FOLDERS ON)

include(ExternalProject)
include(FetchContent)

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    # require at least gcc 8
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 8)
        message(FATAL_ERROR "GCC version must be at least 8!")
    endif()
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    # require at least clang 6
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 6)
        message(FATAL_ERROR "Clang version must be at least 6!")
    endif()
else()
    message(WARNING "You are using an unsupported compiler! Compilation has only been tested with Clang and GCC.")
endif()


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")


message("-- Using build type ${CMAKE_BUILD_TYPE}")

if ((ENABLE_COVERAGE) AND (CMAKE_BUILD_TYPE STREQUAL "Debug"))
    message("-- Code coverage enabled")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fprofile-arcs")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ftest-coverage")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fPIC")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
endif()

if (PROFILE)
    message("-- Profiling enabled")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -p")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer")
    set(DCMAKE_EXE_LINKER_FLAGS "${DCMAKE_EXE_LINKER_FLAGS} -pg")
    set(DCMAKE_SHARED_LINKER_FLAGS "${DCMAKE_SHARED_LINKER_FLAGS} -pg")
endif()


# set up usrlocal directory
set(USR_DIR ${CMAKE_CURRENT_BINARY_DIR}/usrlocal)
set(USR_LIB_DIR ${USR_DIR}/lib)
file(MAKE_DIRECTORY ${USR_DIR})

list(APPEND CMAKE_LIBRARY_PATH ${USR_DIR}/lib)
list(APPEND CMAKE_PROGRAM_PATH ${USR_DIR}/bin)
list(APPEND CMAKE_INCLUDE_PATH ${USR_DIR}/include)
# directory where all external project libraries reside
link_directories(${USR_DIR}/lib)
include_directories(SYSTEM ${USR_DIR}/include)


set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set (THREADS_PREFER_PTHREAD_FLAG ON)
set (CUDD_USE_STATIC_LIBS ON)
find_package(Threads REQUIRED)
find_package(CUDD REQUIRED)
find_package(Graphviz REQUIRED)
find_package(BISON 3.0 REQUIRED)
find_package(FLEX 2.6 REQUIRED)

add_subdirectory(vendor EXCLUDE_FROM_ALL)
add_subdirectory(libs)
add_subdirectory(apps)

enable_testing ()
