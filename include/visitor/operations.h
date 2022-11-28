#pragma once

#include "datum.h"

namespace visitor {
namespace detail {

struct IsBlank : public boost::static_visitor<bool> {
  bool operator()(const Blank &a) const { return true; }
  template <typename T> bool operator()(const T &a) const { return false; }
};

struct Plus : public boost::static_visitor<Datum> {
  template <typename T, typename U>
  auto operator()(const T &a, const U &b) const -> decltype(Datum(a + b)) {
    return Datum(a + b);
  }

  template <typename... Ts> Datum operator()(const Ts &...params) const {
    std::string err("invalid Plus operands: ");
    throw std::invalid_argument(err);
  }

  static constexpr const char *text = "+";
};

struct Print : public boost::static_visitor<Datum> {
  template <typename T> Datum operator()(const T &a) const {
    std::cout << Datum(a);
    return {};
  }

  template <typename... Ts> Datum operator()(const Ts &...params) const {
    std::string err("invalid Plus operands: ");
    throw std::invalid_argument(err);
  }

  static constexpr const char *text = "+";
};

inline Datum operator+(Datum const &lhs, Datum const &rhs) {
  return boost::apply_visitor(Plus(), lhs, rhs);
}
inline Datum print(Datum const &lhs) {
  return boost::apply_visitor(Print(), lhs);
}

} // namespace detail
} // namespace visitor
