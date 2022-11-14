cmake_minimum_required(VERSION 3.10)
project (WSHEEET VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
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

find_package(GTest)

if(GTest_FOUND)
  set(BUILD_TESTING ON)
  enable_testing()
  add_subdirectory(test)
endif()

set(INCLUDE_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SRC_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/lib)
list(APPEND INCLUDE_DIRS ${INCLUDE_PREFIX})

add_subdirectory(lib)