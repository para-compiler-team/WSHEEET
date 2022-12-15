#pragma once

namespace wsheeet::visitor {

namespace detail {
// Track list of visitors.
template <typename ...Tys>
struct VTys { using type = VTys; };

// TODO concept for visitor.
// V is visitor and it has to contain retty.
template <typename V>
struct AstVisitable {
  virtual typename V::retty accept(V &v) = 0;
  virtual ~AstVisitable() = default;
};

template <typename Vts>
struct AstVisitables;

// Stop pack unwrap.
template <>
struct AstVisitables<VTys<>> {
  virtual ~AstVisitables() = default;
};

// Recurrent inheritance to insert all accepts in namespace.
template <typename T0, typename ... Ts>
struct AstVisitables<VTys<T0, Ts...>> : virtual AstVisitable<T0>,
  AstVisitables<VTys<Ts...>> {
  using AstVisitable<T0>::accept;
};
} // namespace detail

// To specify that AstNode is visitable by Visitor1, Visitor2 ... use 
// class AstNode : public visitor::AstVisitable<Visitor1, Visitor2, ...>
// Be sure to implement all required "accept" in AstNodes.
template <typename ... Tys>
using AstVisitable = detail::AstVisitables< detail::VTys<Tys...> >;

} // namespace wsheeet::visitor