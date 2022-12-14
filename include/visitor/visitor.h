#pragma once

#include <AST/datum.h>

#include <def_concepts.hpp>

namespace wsheeet::ast {
// template <Expr E>
class BinOpExpr;
} // namespace wsheeet::ast

namespace wsheeet::visitor {

struct InterpreitVisitor final {
  template <typename T> auto visit(T &E) -> ast::detail::Datum;
};

} // namespace wsheeet::visitor

extern template wsheeet::ast::detail::Datum
wsheeet::visitor::InterpreitVisitor::visit<wsheeet::ast::BinOpExpr>(
    wsheeet::ast::BinOpExpr &);
