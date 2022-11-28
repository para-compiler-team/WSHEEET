#include "AST/token.h"
#include "AST/ast.h"

#include <iostream>

void ast::call() {
  std::cout << "Call from ast\n";

  ast::INode *r = new ast::ValueNode{5};
  ast::INode *l = new ast::ValueNode{15};

  ast::INode *pl = new ast::Plus{l, r};

  std::cout << pl->eval() << '\n';

  delete r;
  delete l;
  delete pl;
}
