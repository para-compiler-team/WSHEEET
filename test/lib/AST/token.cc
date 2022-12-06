#include "AST/token.h"

#include "AST/TypeBuilder.h"

#include <iostream>

void ast::call() {
  std::cout << "Call from ast\n";

  ast::TypeBuilder TB;

  auto *Int = TB.get<ast::Int>();
  std::cout << *Int << '\n';
  auto Int16 = TB.get<ast::Int>(16u);
  assert(Int != Int16);
  std::cout << *Int16 << '\n';

  auto Int16_2 = TB.get<ast::Int>(16u);
  assert(Int16_2 == Int16);
  std::cout << *Int16_2 << '\n';

  auto CI = TB.get<ast::ConstInt>(5);
  std::cout << '\n' << *CI << '\n';
  auto array = TB.get<ast::Array>(CI, Int);
  std::cout << *array << '\n';

  auto VA = TB.get<ast::Array>(Int16, Int);
  assert(array != VA);
  std::cout << *VA << '\n';

  // Is it range, or init list?
  auto S = TB.get<ast::Struct>(VA, array, CI, Int16, Int);
  std::cout << *S << '\n';
}
