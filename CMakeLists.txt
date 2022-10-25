# ---------------------------------------------------------------------------- #
# Copyright 2022 Yuly Tarasov
#
# This file is part of WSHEEET.
#
# WSHEEET is free software: you can redistribute it and/or modify it under the
# terms of the GNU General Public License as published by the Free Software
# Foundation, either version 3 of the License, or (at your option) any later
# version.
#
# WSHEEET is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
# A PARTICULAR PURPOSE. See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along with
# WSHEEET. If not, see <https://www.gnu.org/licenses/>.
# ---------------------------------------------------------------------------- #

cmake_minimum_required(VERSION 3.24.2)

project(WSHEEET)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(FORCE_COLORED_OUTPUT "Always produce ANSI-colored output (GNU/Clang only)." ON)

if(${FORCE_COLORED_OUTPUT})
  message("-- Force colored output enabled")

  if("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
     add_compile_options(-fdiagnostics-color=always)
  elseif("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
     add_compile_options(-fcolor-diagnostics)
  endif()
endif()

include_directories(include)
add_subdirectory(lib)
