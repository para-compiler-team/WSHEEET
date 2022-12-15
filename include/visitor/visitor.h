#pragma once

#include <AST/datum.h>

namespace wsheeet::visitor {

struct InterpreitVisitor final {
  using retty = ast::detail::Datum;
  // TODO this template cannot be substituted with Expr concept, because Expr concept requires ExprBase and ExprBase requires InterpreitVisitor.
  template <typename T>
  auto visit(T &E) -> retty { return {}; }
};

} // namespace wsheeet::visitor
