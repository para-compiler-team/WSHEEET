#include "visitor/visitor.h"

#include <AST/Expr.hpp>

#include "operations.h"

namespace wsheeet::visitor {

template <>
auto InterpreitVisitor::visit(ast::IntLiteral &E) -> ast::detail::Datum {
  return E.value();
}

template <>
auto InterpreitVisitor::visit(ast::FPLiteral<float> &E) -> ast::detail::Datum {
  return E.value();
}

// template<>
// auto InterpreitVisitor::visit(ast::FPLiteral<double> &E) ->
// ast::detail::Datum {
//   return E.value();
// }

template <>
auto InterpreitVisitor::visit(ast::BinOpExpr &E) -> ast::detail::Datum {
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

} // namespace wsheeet::visitor