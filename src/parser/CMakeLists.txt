set(NAME "parser")

find_package(FLEX REQUIRED)

flex_target(scanner
  lexems.l
  ${CMAKE_CURRENT_BINARY_DIR}/lexer.cc
)

add_executable(${NAME} 
  main.cc  
  ${FLEX_scanner_OUTPUTS}
)

# target_compile_features(${NAME} PRIVATE cxx_std_20)
target_include_directories(${NAME} PRIVATE ${INCLUDE_PREFIX})

# message(NOTICE "here ${CMAKE_CURRENT_SOURCE_DIR}")
# message(NOTICE "here ${CMAKE_SOURCE_DIR}")
