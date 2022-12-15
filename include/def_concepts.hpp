#pragma once

#include <concepts>

namespace wsheeet {
namespace visitor {

class InterpreitVisitor;

struct AstVisitable {
  virtual ast::detail::Datum accept(InterpreitVisitor &v) = 0;
  virtual ~AstVisitable() = default;
};

} // namespace visitor

namespace ast {
class TypeBase;
class ExprParent {};

class ExprBase : public visitor::AstVisitable, public ExprParent {
protected:
  TypeBase *Type;
  ExprBase(TypeBase &Ty) : Type{&Ty} {}

public:
  TypeBase &type() { return *Type; }
}; // class ExprBase

template <typename T>
concept Expr = std::derived_from<T, ExprBase>;

} // namespace ast
} // namespace wsheeet
