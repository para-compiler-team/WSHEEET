#include <AST/Expr.hpp>

auto wsheeet::ast::BinOpExpr::accept(wsheeet::visitor::InterpreitVisitor &v)
    -> wsheeet::ast::detail::Datum {
  return v.visit(*this);
}
