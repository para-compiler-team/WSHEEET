#pragma once

#include <concepts>

namespace wsheeet {
namespace visitor {

class InterpreitVisitor;

template <class... Ts> struct types {
  using type = types;
};

template <class T> struct AstVisitable {
  virtual ast::detail::Datum accept(InterpreitVisitor &v) = 0;
  virtual ~AstVisitable() = default;
};

template <class Types> struct AstVisitables;

template <> struct AstVisitables<types<>> {
  virtual ~AstVisitables() = default;
};

template <class T0, class... Ts>
struct AstVisitables<types<T0, Ts...>> : virtual AstVisitable<T0>,
                                         AstVisitables<types<Ts...>> {
  using AstVisitable<T0>::accept;
};
using SuppRetType = types<ast::detail::Datum::types>;

} // namespace visitor

namespace ast {
class TypeBase;
class ExprParent {};

class ExprBase : public visitor::AstVisitables<visitor::SuppRetType>,
                 public ExprParent {
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
