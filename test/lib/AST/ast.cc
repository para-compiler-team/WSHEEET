#include "AST/ast.h"

#include "visitor/visitor.h"

namespace ast {

detail::Datum ValueNode::accept(visitor::IVisitor &v) { return v.visit(*this); }

detail::Datum PlusNode::accept(visitor::IVisitor &v) { return v.visit(*this); }

} // namespace ast
