#include "visitor/visitor.h"

#include "AST/ast.h"

#include "operations.h"

namespace visitor {

ast::detail::Datum InterpreitVisitor::visit(ast::ValueNode &N) {
  return N.Value;
}
ast::detail::Datum InterpreitVisitor::visit(ast::PlusNode &N) {
  using namespace detail; // For operator+ to be accessable.
  auto lhs = N.lft->accept(*this);
  auto rhs = N.rgh->accept(*this);

  return lhs + rhs;
}

} // namespace visitor