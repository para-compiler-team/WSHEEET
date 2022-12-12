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

#include <concepts>
#include <cstddef>
#include <functional>
#include <iostream>
#include <memory>
#include <ranges>
#include <vector>

namespace wsheeet::ast {

// Forward declaration.
class TypeBuilder;
class TypeBase;

} // namespace wsheeet::ast

template <> struct std::hash<wsheeet::ast::TypeBase> {
  std::size_t operator()(wsheeet::ast::TypeBase &T) const noexcept {
    return reinterpret_cast<size_t>(&T);
  }
};

namespace wsheeet::ast {

// Base class describes basic info about all Types.
// It contains hash info.
// Unique type has unique pointer to it, therefore, hash conStructTys from this.
class TypeBase {
protected:
  TypeBuilder &Builder;
  TypeBase(TypeBuilder &B) : Builder{B} {}
  TypeBase(const TypeBase &) = delete;
  TypeBase &operator=(const TypeBase &) = delete;

public:
  auto &builder() {return Builder;}
  virtual void print(std::ostream &os) const = 0;
  virtual ~TypeBase(){};
};

template <typename T>
concept Type = std::derived_from<std::remove_cvref_t<T>, TypeBase>;

// out operator to print all Types.
inline std::ostream &operator<<(std::ostream &os, Type auto const &Ty) {
  Ty.print(os);
  return os;
}

class GenericTy final : public TypeBase {
  // TODO get rid of friend?
  friend class TypeBuilder;

protected:
  GenericTy(TypeBuilder &B) : TypeBase{B} {}

public:
  virtual void print(std::ostream &os) const override {
    os << Dsc << '[' << std::hex << this << std::dec << ']';
  }
  virtual ~GenericTy() {}

private:
  static constexpr char Dsc[] = "GenericTy";
};

class CharTy final : public TypeBase {
  // TODO get rid of friend?
  friend class TypeBuilder;

protected:
  CharTy(TypeBuilder &B) : TypeBase{B} {}

public:
  using ValueTy = std::byte;

  virtual void print(std::ostream &os) const override {
    os << Dsc << '[' << std::hex << this << std::dec << ']';
  }
  virtual ~CharTy() {}

private:
  static constexpr char Dsc[] = "CharTy";
};

class IntTy final : public TypeBase {
  // TODO get rid of friend?
  friend class TypeBuilder;
  size_t const Size{32};

protected:
  IntTy(TypeBuilder &B) : TypeBase{B} {}
  IntTy(TypeBuilder &B, std::unsigned_integral auto Size)
      : TypeBase(B), Size{static_cast<size_t>(Size)} {}

public:
  using ValueTy = int;

  [[nodiscard]] auto bitwidth() const noexcept { return Size; }

  virtual void print(std::ostream &os) const override {
    os << Dsc << '[' << std::hex << this << std::dec << "]<Size=" << Size
       << '>';
  }
  virtual ~IntTy() {}

private:
  static constexpr char Dsc[] = "IntTy";
};

} // namespace wsheeet::ast

template <> struct std::hash<wsheeet::ast::IntTy> {
  std::size_t operator()(wsheeet::ast::IntTy &T) const noexcept {
    return T.bitwidth();
  }
};

namespace wsheeet::ast {

template <std::floating_point F> class FPTy final : public TypeBase {
  friend class TypeBuilder;

protected:
  FPTy(TypeBuilder &B) : TypeBase{B} {}

public:
  using ValueTy = F;

  void print(std::ostream &os) const override {
    os << dsc() << '[' << std::hex << this << std::dec << ']';
  }

private:
  static consteval auto dsc() noexcept requires(std::same_as<F, float>) {
    return "FloatTy";
  }

  static consteval auto dsc() noexcept requires(std::same_as<F, double>) {
    return "DoubleTy";
  }
};

using FloatTy = FPTy<float>;
using DoubleTy = FPTy<double>;

template <typename T>
concept SimpleType = std::same_as<T, CharTy> || std::same_as<T, IntTy> ||
    std::same_as<T, FloatTy> || std::same_as<T, DoubleTy>;

template <SimpleType T>
class ConstSimpleValueExpr;
using IntLiteral = ConstSimpleValueExpr<IntTy>;

class ExprBase;

class VectorTy final : public TypeBase {
  friend class TypeBuilder;
  TypeBase *ElemsTy;
  IntLiteral *Size;

  VectorTy(TypeBuilder &B, TypeBase &ElemsTy, IntLiteral &Size)
      : TypeBase{B}, ElemsTy{&ElemsTy}, Size{&Size} {}

public:
  void print(std::ostream &os) const override {
    os << Dsc << "[0x" << std::hex << this << std::dec
       << "]<Size=Expr at addr 0x" << std::hex << Size << std::dec
       << ", Base=" << *ElemsTy << '>';
  }

private:
  static constexpr char Dsc[] = "VectorTy";
};

class ArrayTy final : public TypeBase {
  friend class TypeBuilder;
  TypeBase *ElemsTy;
  ExprBase *Size;

  ArrayTy(TypeBuilder &B, TypeBase &ElemsTy, ExprBase &Size)
      : TypeBase(B), ElemsTy{&ElemsTy}, Size{&Size} {}

public:
  void print(std::ostream &os) const override {
    os << Dsc << '[' << std::hex << this << std::dec << "]<Size=Expr at addr 0x"
       << std::hex << Size << std::dec << ", Base=" << *ElemsTy << '>';
  }

private:
  static constexpr char Dsc[] = "ArrayTy";
};

class StructTy final : public TypeBase {
  friend class TypeBuilder;
  std::vector<TypeBase *> Elems;

#if 0
  StructTy(TypeBuilder &B, TypeBase *One) : TypeBase(B), Elems{One} {}
#endif

  template <Type T> void push_by1(T &&First) { Elems.push_back(&First); }

  template <Type T, typename... Args>
  void push_by1(T &&First, Args &&...Other) {
    Elems.push_back(&First);
    push_by1(std::forward<Args>(Other)...);
  }

  template <Type... Args>
  StructTy(TypeBuilder &B, Args &&...args) : TypeBase(B) {
    // TODO reserve memory.
    push_by1(std::forward<Args>(args)...);
  }

  // StructTy(std::ranges::view auto &v) Elems{v} : {}

public:
  void print(std::ostream &os) const override {
    os << Dsc << '[' << std::hex << this << std::dec << "]<Consists of: ";
    for (auto *x : Elems)
      os << *x << ", ";
    os << '>';
  }

private:
  static constexpr char Dsc[] = "StructTy";
};

} // namespace wsheeet::ast
