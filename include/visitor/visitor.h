#pragma once

#include "AST/datum.h"

// Forward declarations
namespace ast {
class ValueNode;
class PlusNode;

// TODO how wo forward declare using Datum = ...?
// namespace detail { class Datum; } // namespace detail

} // namespace ast

namespace visitor {

struct IVisitor {
  virtual ast::detail::Datum visit(ast::ValueNode &) = 0;
  virtual ast::detail::Datum visit(ast::PlusNode &) = 0;

  virtual ~IVisitor() {}
};

// TODO after template this has to be implemented here with inline funcitons.
struct InterpreitVisitor final : IVisitor {
  ast::detail::Datum visit(ast::ValueNode &) override;
  ast::detail::Datum visit(ast::PlusNode &) override;
};

} // namespace visitor
