#include "visitor/token.h"
#include "visitor/operations.h"

#include <iostream>

void visitor::call() {
  std::cout << "Call from visitor\n";

  visitor::detail::Datum a = 15;
  visitor::detail::Datum b = 5.f;

  visitor::detail::print(a + b);
}
