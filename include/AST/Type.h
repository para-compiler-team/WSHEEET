#pragma once

#include <iostream>
#include <memory>
#include <ranges>
#include <vector>

namespace ast {

// Forward declaration.
class TypeBuilder;
// ExprNode is the placeholder to put here expresion node from AST.
class ExprNode;

class Hash {
  size_t V{0x42};
  bool IsSetted{false};

public:
  // Safe cast operator, which emits a Warning if smth is broken.
  operator size_t() const {
    if (!IsSetted)
      std::cerr << "Not setted hash.\n";
    return V;
  }
  Hash() = default;
  Hash(size_t VIn) { set(VIn); }
  void set(size_t VIn) {
    V = VIn;
    IsSetted = true;
  }
  bool isSetted() const { return IsSetted; }
};
inline std::ostream &operator<<(std::ostream &os, Hash const &H) {
  if (H.isSetted())
    os << std::hex << (size_t)H << std::dec;
  else
    os << "N";
  return os;
}

// Base class describes basic info about all Types.
// It contains hash info.
// Unique type has unique pointer to it, therefore, hash conStructTys from this.
class TypeBase {
  Hash HValue;

protected:
  TypeBase() { HValue.set(reinterpret_cast<size_t>(this)); }

public:
  Hash const &getHash() const { return HValue; };
  virtual void print(std::ostream &os) const = 0;
  virtual ~TypeBase(){};
};

template <typename T>
concept CIsType = std::is_base_of_v<TypeBase, T>;

// out operator to print all Types.
inline std::ostream &operator<<(std::ostream &os, CIsType auto const &Ty) {
  Ty.print(os);
  return os;
}

class IntTy : public TypeBase {
  // TODO get rid of friend?
  friend class TypeBuilder;
  size_t const Size{32};
  ExprNode *PLACEHOLDER{};

protected:
  IntTy() = default;
  IntTy(std::unsigned_integral auto Size) : Size{static_cast<size_t>(Size)} {}

public:
  virtual void print(std::ostream &os) const override {
    os << Dsc << '[' << getHash() << "]<Size=" << Size << '>';
  }
  virtual ~IntTy() {}

private:
  static constexpr char Dsc[] = "IntTy";
};

class ConstIntTy final : public IntTy {
  friend class TypeBuilder;
  int const Value{0};

  ConstIntTy(std::integral auto Value) : Value{Value} {}

public:
  void print(std::ostream &os) const override {
    os << Dsc << '[' << getHash() << "]<Value=" << Value << '>';
  }

private:
  static constexpr char Dsc[] = "ConstIntTy";
};

class FloatTy final : public TypeBase {
  friend class TypeBuilder;
  ExprNode *PLACEHOLDER{};

public:
  void print(std::ostream &os) const override {
    os << Dsc << '[' << getHash() << ']';
  }

private:
  static constexpr char Dsc[] = "FloatTy";
};

class DoubleTy final : public TypeBase {
  friend class TypeBuilder;
  ExprNode *PLACEHOLDER{};

public:
  void print(std::ostream &os) const override {
    os << Dsc << '[' << getHash() << ']';
  }

private:
  static constexpr char Dsc[] = "DoubleTy";
};

class ArrayTy final : public TypeBase {
  friend class TypeBuilder;
  IntTy *Size;
  TypeBase *Base;

  ArrayTy(IntTy *Size, TypeBase *Base) : Size{Size}, Base{Base} {}

public:
  void print(std::ostream &os) const override {
    os << Dsc << '[' << getHash() << "]<Size=" << *Size << ", Base=" << *Base
       << '>';
  }

private:
  static constexpr char Dsc[] = "ArrayTy";
};

class StructTy final : public TypeBase {
  friend class TypeBuilder;
  std::vector<TypeBase *> Elems;

  StructTy(TypeBase *One) : Elems{One} {}

  template <typename T> void push_by1(T &&First) { Elems.push_back(First); }

  template <typename T, typename... Args>
  void push_by1(T &&First, Args &&...Other) {
    Elems.push_back(First);
    push_by1(std::forward<Args>(Other)...);
  }

  template <typename... Args> StructTy(Args &&...args) {
    // TODO reserve memory.
    push_by1(std::forward<Args>(args)...);
  }

  // StructTy(std::ranges::view auto &v) Elems{v} : {}

public:
  void print(std::ostream &os) const override {
    os << Dsc << '[' << getHash() << "]<Consists of: ";
    for (auto *x : Elems)
      os << *x << "; ";
    os << '>';
  }

private:
  static constexpr char Dsc[] = "StructTy";
};

} // namespace ast
