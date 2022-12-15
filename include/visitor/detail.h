#pragma once

namespace wsheeet::visitor {

namespace detail {

// TODO concept for visitor.
// V is visitor and it has to contain retty.
template <typename V>
struct AstVisitable {
  virtual typename V::retty accept(V &v) = 0;
  virtual ~AstVisitable() = default;
};

template <typename... Ts> struct AstVisitables : AstVisitable<Ts>... {
  using AstVisitable<Ts>::accept...;
};

} // namespace detail

// To specify that AstNode is visitable by Visitor1, Visitor2 ... use 
// class AstNode : public visitor::AstVisitable<Visitor1, Visitor2, ...>
// Be sure to implement all required "accept" in AstNodes.
template <typename... Tys> using AstVisitable = detail::AstVisitables<Tys...>;

} // namespace wsheeet::visitor