set(NAME "parser")

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

if (NOT DEFINED GRAMMAR)
  set(GRAMMAR "grammar.y" CACHE STRING "file with grammar" FORCE)
endif()

flex_target(scanner
  lexems.l
  ${CMAKE_CURRENT_BINARY_DIR}/lexer.cc
)

bison_target(parser
  ${GRAMMAR}
  ${CMAKE_CURRENT_BINARY_DIR}/parser.cc
  COMPILE_FLAGS "--defines=${CMAKE_CURRENT_BINARY_DIR}/grammar.tab.hh -Wcounterexamples --debug")

add_flex_bison_dependency(scanner parser)

add_executable(${NAME} 
  driver.cc  
  ${FLEX_scanner_OUTPUTS}
  ${BISON_parser_OUTPUTS}
)

# target_compile_features(${NAME} PRIVATE cxx_std_20)
target_include_directories(${NAME} PRIVATE ${INCLUDE_PREFIX})
target_include_directories(${NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
# message(NOTICE "here ${CMAKE_CURRENT_SOURCE_DIR}")
# message(NOTICE "here ${CMAKE_SOURCE_DIR}")
