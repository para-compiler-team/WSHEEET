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

set(TESTS_LIST TreeNode)

if(BUILD_TESTING)
  foreach(TEST_NAME IN LISTS TESTS_LIST)
    add_executable(${TEST_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/${TEST_NAME}.cpp")
    target_link_libraries(${TEST_NAME} gtest gtest_main)
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
  endforeach()
endif()
