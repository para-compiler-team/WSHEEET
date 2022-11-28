#include "AST/ast.h"
#include "AST/token.h"
#include "visitor/token.h"

#include <iostream>

int main() {
  ast::call();
  visitor::call();

  ast::INode *r = new ast::ValueNode{5};
  ast::INode *l = new ast::ValueNode{15};

  ast::INode *pl = new ast::Plus{l, r};

  std::cout << pl->eval() << '\n';

  delete r;
  delete l;
  delete pl;
  return 0;
}
