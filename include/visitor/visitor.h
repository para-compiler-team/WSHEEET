#pragma once

#include <AST/datum.h>

#include <def_concepts.hpp>


namespace wsheeet::visitor {

struct InterpreitVisitor final {
  auto visit(ast::Expr auto &E) -> ast::detail::Datum { return {}; }
};

} // namespace wsheeet::visitor

// extern template wsheeet::ast::detail::Datum
// wsheeet::visitor::InterpreitVisitor::visit<wsheeet::ast::BinOpExpr>(
//     wsheeet::ast::BinOpExpr &);
