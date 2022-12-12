// -------------------------------------------------------------------------- //
// Copyright 2022 Ilya Andreev, Yuly Tarasov
//
// This file is part of WSHEET.
//
// WSHEET is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// WSHEET is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
// A PARTICULAR PURPOSE. See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with
// WSHEET. If not, see <https://www.gnu.org/licenses/>.
// -------------------------------------------------------------------------- //

#pragma once

#include "Type.hpp"

#include <cassert>
#include <cstddef>
#include <memory>
#include <tuple>
#include <type_traits>
#include <unordered_map>

namespace wsheeet::ast {

class TypeBuilder final {

  template <Type T>
  using TypesList = std::unordered_map<size_t, std::unique_ptr<T>>;

  // TODO make smth like: TypeBuilder.registerTypes<MyInt, MyFloat...>
  std::tuple<TypesList<IntTy>, TypesList<FloatTy>, TypesList<DoubleTy>,
             TypesList<VectorTy>, TypesList<ArrayTy>, TypesList<StructTy>>
      TypesContaner;

  GenericTy GenericType;

  template <typename T, typename... Tys>
  size_t parseArgs(T &&First, Tys &&...Others) {
    auto Hash = reinterpret_cast<size_t>(&First);
    if constexpr (std::same_as<T, IntTy>)
      Hash = First.bitwidth();
    Hash ^= parseArgs(std::forward<Tys>(Others)...) << 1u;
    return Hash;
  }

  auto parseArgs(Type auto &&One) {
    return std::hash<std::remove_cvref_t<decltype(One)>>{}(One);
  }

  auto parseArgs(std::unsigned_integral auto &One) {
    return static_cast<size_t>(One);
  }

  size_t parseArgs() { return {}; }

public:
  TypeBuilder() : GenericType{*this} {};
  TypeBuilder(TypeBuilder const &) = delete;
  TypeBuilder &operator=(TypeBuilder const &) = delete;
  TypeBuilder &operator=(TypeBuilder const &&) = delete;
  ~TypeBuilder() = default;

  GenericTy &getGeneric() { return GenericType; }

  // TODO make concepts
  template <Type T, typename... Args> T &get(Args &&...args) {
    auto &&List = std::get<TypesList<T>>(TypesContaner);

    size_t ArgsHash = parseArgs(args...);

    auto FindIt = List.find(ArgsHash);
    if (FindIt == List.end()) {
      bool IsInserted = false;
      std::tie(FindIt, IsInserted) = List.emplace(
          // Here I need new T for incapsulation.
          ArgsHash, new T{*this, std::forward<Args>(args)...});
      assert(IsInserted && "If Type not found, it has to be inserted once.");
    }

    return *FindIt->second.get();
  }
};

} // namespace wsheeet::ast
