#include "visitor/visitor.h"

#include <AST/Decl.hpp>

#include "operations.h"

namespace wsheeet::visitor {

template <>
auto InterpreitVisitor::visit(ast::IntLiteral &E)
    -> InterpreitVisitor::retty {
  return E.value();
}

template <>
auto InterpreitVisitor::visit(ast::FPLiteral<float> &E)
    -> InterpreitVisitor::retty {
  return E.value();
}

template <>
auto InterpreitVisitor::visit(ast::FPLiteral<double> &E)
    -> InterpreitVisitor::retty {
  return E.value();
}

template <>
auto InterpreitVisitor::visit(wsheeet::ast::BinOpExpr &E)
    -> InterpreitVisitor::retty {
  auto &&L = E.LHS().accept(*this);
  auto &&R = E.RHS().accept(*this);

  switch (E.opcode()) {
  case ast::BinOpcode::PLUS:
    return L + R;

  default:
    std::cout << "Not Implmented yet.\n";
    break;
  }
  return L;
  // Return blank.
  // return {};
}

template <>
auto InterpreitVisitor::visit(ast::ExprStmt &E) -> InterpreitVisitor::retty {
  return E.child().accept(*this);
}

template <>
auto InterpreitVisitor::visit(ast::CompoundStmt &E)
    -> InterpreitVisitor::retty {
  // This should be iterated throught all children.
  // (((((FUCK.)))))
  return (*E.begin())->accept(*this);
}

template <>
auto InterpreitVisitor::visit(ast::LayerDecl &E) -> InterpreitVisitor::retty {
  return E.child().accept(*this);
}

} // namespace wsheeet::visitor




