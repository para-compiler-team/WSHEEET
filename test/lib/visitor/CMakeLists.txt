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

set(PROJECT_NAME visitor)

file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cc)

add_library(${PROJECT_NAME} STATIC)
target_sources(${PROJECT_NAME} PRIVATE ${SRC_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIRS})
