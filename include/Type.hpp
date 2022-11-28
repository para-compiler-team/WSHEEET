// -------------------------------------------------------------------------- //
// Copyright 2022 Yuly Tarasov
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

#include "AST/Decl.hpp"
#include "AST/Stmt.hpp"
#include "AST/SymbolTable.hpp"

#include <algorithm>
#include <concepts>
#include <string>
#include <sstream>
#include <type_traits>
#include <vector>

namespace wsheeet {

class IType {
public:
  virtual std::string name() const = 0;
  virtual bool isGeneric() const = 0;
  virtual bool isSimple() const = 0;
  virtual bool isChar() const = 0;
  virtual bool isInt() const = 0;
  virtual bool isFloat() const = 0;
  virtual bool isDouble() const = 0;
  virtual bool isCompound() const = 0;
  virtual bool isArray() const = 0;
  virtual bool isStructure() const = 0;
  virtual bool isVector() const = 0;
  virtual bool isFunction() const = 0;
  virtual bool isReference() const = 0;
}; // class IType

namespace concepts {

template <typename T>
concept Type = std::derived_from<T, IType>;

} // namespace concepts

class GenericType : public IType {
public:
  bool isGeneric() const override { return true; }
  std::string name() const override { return std::string{"generic"}; };

  bool isSimple() const override { return false; }
  bool isChar() const override { return false; }
  bool isInt() const override { return false; }
  bool isFloat() const override { return false; }
  bool isDouble() const override { return false; }
  bool isCompound() const override { return false; }
  bool isArray() const override { return false; }
  bool isStructure() const override { return false; }
  bool isVector() const override { return false; }
  bool isFunction() const override { return false; }
  bool isReference() const override { return false; }
}; // class GenericType

namespace concepts {

template <typename T>
concept GenericType = std::derived_from<T, wsheeet::GenericType>;

} // namespace concepts


class SimpleType : public IType {
public:
  bool isSimple() const override { return true; }

  bool isGeneric() const override { return false; }
  bool isCompound() const override { return false; }
  bool isArray() const override { return false; }
  bool isStructure() const override { return false; }
  bool isVector() const override { return false; }
  bool isFunction() const override { return false; }
  bool isReference() const override { return false; }
}; // class SimpleType

namespace concepts {

template <typename T>
concept SimpleType = std::derived_from<T, wsheeet::SimpleType>;

} // namespace concepts

class CharType : public SimpleType {
public:
  using ValueTy = std::byte;

  bool isChar() const override { return true; }

  bool isInt() const override { return false; }
  bool isFloat() const override { return false; }
  bool isDouble() const override { return false; }

  std::string name() const override { return "char"; }
}; // class IntType

class IntType : public SimpleType {
public:
  using ValueTy = int;

  bool isInt() const override { return true; }

  bool isChar() const override { return false; }
  bool isFloat() const override { return false; }
  bool isDouble() const override { return false; }

  std::string name() const override { return std::string{"int"}.append(std::to_string(bitwidth)); }
protected:
  size_t bitwidth = 32;
}; // class IntType

template <std::floating_point F>
class FPType;

template <>
class FPType<float> : public SimpleType {
public:
  using ValueTy = float;

  bool isFloat() const override { return true; }

  bool isChar() const override { return false; }
  bool isInt() const override { return false; }
  bool isDouble() const override { return false; }

  std::string name() const override { return "float"; };
}; // class FloatType

template <>
class FPType<double> : public SimpleType {
public:
  using ValueTy = double;

  bool isDouble() const override { return true; }

  bool isChar() const override { return false; }
  bool isInt() const override { return false; }
  bool isFloat() const override { return false; }

  std::string name() const override { return "double"; };
}; // class FloatType

class CompoundType : public IType {
public:
  bool isCompound() const override { return true; }

  bool isGeneric() const override { return false; }
  bool isChar() const override { return false; }
  bool isSimple() const override { return false; }
  bool isInt() const override { return false; }
  bool isFloat() const override { return false; }
  bool isDouble() const override { return false; }
}; // class CompoundType

template <concepts::Type T>
class ArrayType : public CompoundType {
public:
  bool isArray() const override { return true; }

  bool isStructure() const override { return false; }
  bool isVector() const override { return false; }
  bool isFunction() const override { return false; }

protected:
  IType *ElemType_;
  AST::IExpr *Size_;

public:
  std::string name() const override {
    std::ostringstream oss;
    oss << ElemType_->name() << "[";
    Size_->print(oss);
    oss << "]";
    return oss.str();
  }
}; // class ArrayType

class StructureType : public CompoundType {
public:
  bool isStructure() const override { return true; }

  bool isArray() const override { return false; }
  bool isVector() const override { return false; }
  bool isFunction() const override { return false; }

protected:
  GlueTypeDecl &Decl;

public:
  std::string name() const override {
    std::ostringstream oss;
    oss << "{ ";
    bool firstPrinted = false;
    std::ranges::for_each(Decl.members(), [firstPrinted, &oss](auto *member) mutable {
      if (firstPrinted)
        oss << ", ";
      else
        firstPrinted = true;
      oss << member->name();
    });
    oss << " }";
    return oss.str();
  }
}; // class StructureType

class VectorType : public CompoundType {
public:
  bool isVector() const override { return true; }

  bool isArray() const override { return false; }
  bool isStructure() const override { return false; }
  bool isFunction() const override { return false; }

protected:
  IType *ElemType_;
  size_t Size_;

public:
  std::string name() const override {
    std::ostringstream oss;
    oss << "<" << Size_ << " x " << ElemType_->name() << ">";
    return oss.str();
  }
}; // class VectorType

class FunctionType : public CompoundType {
public:
  bool isFunction() const override { return true; }

  bool isArray() const override { return false; }
  bool isStructure() const override { return false; }
  bool isVector() const override { return false; }

protected:
  IType *ReturnType_;
  std::vector<IType *> ArgsTypes_;

public:
  std::string name() const override {
    std::ostringstream oss;
    oss << ReturnType_->name() << "(";
    bool firstPrinted = false;
    std::ranges::for_each(ArgsTypes_, [firstPrinted, &oss](auto *type) mutable {
      if (firstPrinted)
        oss << ", ";
      else
        firstPrinted = true;
      oss << type->name();
    });
    oss << ")";
    return oss.str();
  }
}; // class FunctionType

template <concepts::Type T>
class RefType : public T {
public:
  bool isReference() const override { return true; }
};

class CharRefType : public RefType<CharType> {
}; // class CharRefType

class IntRefType : public RefType<IntType> {
}; // class IntRefType

template <std::floating_point F>
class FPRefType : public RefType<FPType<F>> {
}; // class FPRefType

template <concepts::Type T>
class ArrayRefType : public RefType<ArrayType<T>> {
}; // class ArrayRefType

class StructRefType : public RefType<StructureType> {
}; // class StructRefType

class VectorRefType : public RefType<VectorType> {
}; // class VectorRefType

} // namespace wsheeet
