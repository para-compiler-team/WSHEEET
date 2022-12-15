#include <AST/Decl.hpp>

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

auto wsheeet::ast::LayerDecl::accept(wsheeet::visitor::InterpreitVisitor &v)
    -> wsheeet::visitor::InterpreitVisitor::retty {
  return v.visit(*this);
}

auto wsheeet::ast::GlobalScope::accept(wsheeet::visitor::InterpreitVisitor &v)
    -> wsheeet::visitor::InterpreitVisitor::retty {
  return v.visit(*this);
}