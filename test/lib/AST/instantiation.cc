#include <AST/Expr.hpp>
#include <AST/Stmt.hpp>

auto wsheeet::ast::BinOpExpr::accept(wsheeet::visitor::InterpreitVisitor &v) -> wsheeet::visitor::InterpreitVisitor::retty {
  return v.visit(*this);
}

auto wsheeet::ast::CompoundStmt::accept(wsheeet::visitor::InterpreitVisitor &v)
    -> wsheeet::visitor::InterpreitVisitor::retty {
  return v.visit(*this);
}

auto wsheeet::ast::ExprStmt::accept(wsheeet::visitor::InterpreitVisitor &v)
    -> wsheeet::visitor::InterpreitVisitor::retty {
  return v.visit(*this);
}
