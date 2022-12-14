add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE
  AST/E2E.cpp
  AST/instantiation.cc
  visitor/visitor.cc
)

target_include_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIRS})

#find_package(Boost REQUIRED)
#add_subdirectory(visitor)

#target_link_libraries(${PROJECT_NAME} AST)
#target_link_libraries(${PROJECT_NAME} visitor)