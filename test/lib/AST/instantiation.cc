#include <AST/Expr.hpp>

auto wsheeet::ast::BinOpExpr::accept(wsheeet::visitor::InterpreitVisitor &v) -> wsheeet::visitor::InterpreitVisitor::retty {
  return v.visit(*this);
}
