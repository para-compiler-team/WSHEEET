#pragma once

#include "Type.h"

#include <cassert>
#include <memory>
#include <tuple>
#include <unordered_map>

namespace ast {

// TODO make it singleton.
struct TypeBuilder {

  template <typename T>
  using TypesList = std::unordered_map<size_t, std::unique_ptr<T>>;

  // TODO make smth like: TypeBuilder.registerTypes<MyInt, MyFloat...>
  std::tuple<TypesList<Int>, TypesList<ConstInt>, TypesList<Float>,
             TypesList<Double>, TypesList<Array>, TypesList<Struct>>
      TypesContaner;

  // TODO make concepts
  template <typename T, typename... Args> T *get(Args &&...args) {
    auto &&List = std::get<TypesList<T>>(TypesContaner);

    size_t ArgsHash = parseArgs(args...);

    auto FindIt = List.find(ArgsHash);
    if (FindIt == List.end()) {
      bool IsInserted = false;
      std::tie(FindIt, IsInserted) = List.emplace(
          ArgsHash, std::make_unique<T>(std::forward<Args>(args)...));
      assert(IsInserted && "If Type not found, it has to be inserted once.");
    }

    return FindIt->second.get();
  }

  // TODO concept.
  size_t getHash(auto *x) { return x->getHash(); }

  size_t getHash(std::integral auto x) { return static_cast<size_t>(x); }

  template <typename T> size_t parseArgs(T &&One) { return getHash(One); }

  size_t parseArgs() { return {}; }

  template <typename T, typename... Tys>
  size_t parseArgs(T &&First, Tys &&...Others) {
    size_t Hash = getHash(First);
    Hash ^= parseArgs(std::forward<Tys>(Others)...) << 1u;
    return Hash;
  }
};

} // namespace ast
