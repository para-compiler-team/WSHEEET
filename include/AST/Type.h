#pragma once

#include <iostream>
#include <ranges>
#include <vector>

namespace ast {

class Hash {
  size_t V{0x42};
  bool IsSetted{false};

public:
  operator size_t() const {
    if (!IsSetted)
      std::cerr << "Not setted hash.\n";
    return V;
  }
  Hash() = default;
  Hash(size_t VIn) : V{VIn}, IsSetted{true} {}
  void set(size_t VIn) {
    V = VIn;
    IsSetted = true;
  }
  bool isSetted() const { return IsSetted; }
};

std::ostream &operator<<(std::ostream &os, Hash const &H) {
  if (H.isSetted())
    os << std::hex << (size_t)H << std::dec;
  else
    os << "N";
  return os;
}

struct Type {
  Hash HValue;

  Type() { HValue.set(reinterpret_cast<size_t>(this)); }

  size_t getHash() const { return HValue; };
  void setHash(size_t Hash) { HValue.set(Hash); }

  virtual void print(std::ostream &os) const {
    os << Dsc << '[' << std::hex << HValue << ']';
  }

private:
  static constexpr char Dsc[] = "Type";
};

template <typename T>
  requires std::is_base_of_v<Type, T>
inline std::ostream &operator<<(std::ostream &os, T const &Ty) {
  Ty.print(os);
  return os;
}

struct Int : Type {
  size_t const Size{32};
  Int() = default;
  Int(std::unsigned_integral auto Size) : Size{static_cast<size_t>(Size)} {}

  virtual void print(std::ostream &os) const override {
    os << Dsc << '[' << HValue << "]<Size=" << Size << '>';
  }

private:
  static constexpr char Dsc[] = "Int";
};

struct ConstInt : Int {
  int const Value{0};

  ConstInt(std::integral auto Value) : Value{Value} {}

  void print(std::ostream &os) const override {
    os << Dsc << '[' << HValue << "]<Value=" << Value << '>';
  }

private:
  static constexpr char Dsc[] = "ConstInt";
};

struct Float : Type {

  void print(std::ostream &os) const override {
    os << Dsc << '[' << HValue << ']';
  }

private:
  static constexpr char Dsc[] = "Float";
};

struct Double : Type {

  void print(std::ostream &os) const override {
    os << Dsc << '[' << HValue << ']';
  }

private:
  static constexpr char Dsc[] = "Double";
};

struct Array : Type {
  Int *Size;
  Type *Base;

  Array(Int *Size, Type *Base) : Size{Size}, Base{Base} {}

  void print(std::ostream &os) const override {
    os << Dsc << '[' << HValue << "]<Size=" << *Size << ", Base=" << *Base
       << '>';
  }

private:
  static constexpr char Dsc[] = "Array";
};

struct Struct : Type {
  std::vector<Type *> Elems;

  Struct(Type *One) : Elems{One} {}

  template <typename T> void push_by1(T &&First) { Elems.push_back(First); }

  template <typename T, typename... Args>
  void push_by1(T &&First, Args &&...Other) {
    Elems.push_back(First);
    push_by1(std::forward<Args>(Other)...);
  }

  template <typename... Args> Struct(Args &&...args) {
    // TODO reserve memory.
    push_by1(std::forward<Args>(args)...);
  }

  void print(std::ostream &os) const override {
    os << Dsc << '[' << HValue << "]<Consists of: ";
    for (auto *x : Elems)
      os << *x << "; ";
    os << '>';
  }

private:
  static constexpr char Dsc[] = "Struct";
};

} // namespace ast
