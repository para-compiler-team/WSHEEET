#pragma once

// File constains data representation.
#include <boost/variant.hpp>

#include <iostream>
#include <map>

namespace ast {
namespace detail {

struct Blank {
  // TODO get unary operators
};

// TOOD get binary operators

inline std::ostream &operator<<(std::ostream &os, const Blank &blank) {
  return os << "(blank)";
}

using Datum = boost::variant<Blank, int, float>;

using DatumID = unsigned short;
using DatumType = std::pair<DatumID, Datum>;
using Datums = std::map<DatumID, Datum>;

} // namespace detail
} // namespace ast
