#include "visitor/token.h"
#include "AST/ast.h"
#include "operations.h"
#include "visitor/visitor.h"

#include <iostream>

void checkDatum() {
  std::cout << "Datum check\n";
  using namespace visitor;
  using namespace detail;
  ast::detail::Datum a = 5;
  ast::detail::Datum b = 15.f;

  std::cout << a + b << std::endl;
}

void visitor::call() {
  checkDatum();
  std::cout << "Call from visitor\n";

  ast::INode *r = new ast::ValueNode{5};
  ast::INode *l = new ast::ValueNode{15};

  ast::INode *pl = new ast::PlusNode{l, r};

  visitor::InterpreitVisitor IV;

  std::cout << pl->accept(IV) << '\n';

  delete r;
  delete l;
  delete pl;
}
