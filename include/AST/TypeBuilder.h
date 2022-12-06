#pragma once

#include "Type.h"

#include <cassert>
#include <memory>
#include <tuple>
#include <unordered_map>

namespace ast {

class TypeBuilder final {

  template <typename T>
  using TypesList = std::unordered_map<size_t, std::unique_ptr<T>>;

  // TODO make smth like: TypeBuilder.registerTypes<MyInt, MyFloat...>
  std::tuple<TypesList<IntTy>, TypesList<ConstIntTy>, TypesList<FloatTy>,
             TypesList<DoubleTy>, TypesList<ArrayTy>, TypesList<StructTy>>
      TypesContaner;

  size_t getHash(CIsType auto *x) { return x->getHash(); }
  size_t getHash(std::integral auto x) { return static_cast<size_t>(x); }
  size_t getHash(auto x) {
    std::cerr << "Faild to get hash.\n";
    return static_cast<size_t>(x);
  }

  template <typename T, typename... Tys>
  size_t parseArgs(T &&First, Tys &&...Others) {
    size_t Hash = getHash(First);
    Hash ^= parseArgs(std::forward<Tys>(Others)...) << 1u;
    return Hash;
  }
  template <typename T> size_t parseArgs(T &&One) { return getHash(One); }
  size_t parseArgs() { return {}; }

public:
  TypeBuilder() = default;
  TypeBuilder(TypeBuilder const &) = delete;
  TypeBuilder &operator=(TypeBuilder const &) = delete;
  TypeBuilder &operator=(TypeBuilder const &&) = delete;
  ~TypeBuilder() = default;

  // TODO make concepts
  template <CIsType T, typename... Args> T *get(Args &&...args) {
    auto &&List = std::get<TypesList<T>>(TypesContaner);

    size_t ArgsHash = parseArgs(args...);

    auto FindIt = List.find(ArgsHash);
    if (FindIt == List.end()) {
      bool IsInserted = false;
      std::tie(FindIt, IsInserted) = List.emplace(
          // Here I need new T for incapsulation.
          ArgsHash, new T{std::forward<Args>(args)...});
      assert(IsInserted && "If Type not found, it has to be inserted once.");
    }

    return FindIt->second.get();
  }
};

} // namespace ast
