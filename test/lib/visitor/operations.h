#pragma once

#include "AST/datum.h"

namespace visitor {
namespace detail {

struct IsBlank : public boost::static_visitor<bool> {
  bool operator()(const ast::detail::Blank &a) const { return true; }
  template <typename T> bool operator()(const T &a) const { return false; }
};

struct Plus : public boost::static_visitor<ast::detail::Datum> {
  template <typename T, typename U>
  auto operator()(const T &a, const U &b) const
      -> decltype(ast::detail::Datum(a + b)) {
    return ast::detail::Datum(a + b);
  }

  template <typename... Ts>
  ast::detail::Datum operator()(const Ts &...params) const {
    std::string err("invalid Plus operands: ");
    throw std::invalid_argument(err);
  }

  static constexpr const char *text = "+";
};

struct Print : public boost::static_visitor<ast::detail::Datum> {
  template <typename T> ast::detail::Datum operator()(const T &a) const {
    std::cout << ast::detail::Datum(a);
    return {};
  }

  template <typename... Ts>
  ast::detail::Datum operator()(const Ts &...params) const {
    std::string err("invalid print: ");
    throw std::invalid_argument(err);
  }

  static constexpr const char *text = "\'out\'";
};

inline ast::detail::Datum operator+(ast::detail::Datum const &lhs,
                                    ast::detail::Datum const &rhs) {
  return boost::apply_visitor(Plus(), lhs, rhs);
}
inline ast::detail::Datum print(ast::detail::Datum const &lhs) {
  return boost::apply_visitor(Print(), lhs);
}

} // namespace detail
} // namespace visitor