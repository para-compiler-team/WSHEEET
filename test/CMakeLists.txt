# ---------------------------------------------------------------------------- #
# Copyright 2022 Ilya Andreev
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

cmake_minimum_required(VERSION 3.14)

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE
  main.cc
)

find_package(Boost REQUIRED)
add_subdirectory(lib)

target_link_libraries(${PROJECT_NAME} AST)
target_link_libraries(${PROJECT_NAME} visitor)