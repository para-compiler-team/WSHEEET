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

#include <string_view>
#include <vector>

namespace wsheeet {

class IType {
public:
  virtual std::string_view name() const = 0;
  virtual bool isGeneric() const = 0;
  virtual bool isSimple() const = 0;
  virtual bool isInt() const = 0;
  virtual bool isFloat() const = 0;
  virtual bool isDouble() const = 0;
  virtual bool isCompound() const = 0;
  virtual bool isArray() const = 0;
  virtual bool isStructure() const = 0;
  virtual bool isVector() const = 0;
  virtual bool isFunction() const = 0;
}; // class IType

class GenericType : public IType {
public:
  bool isGeneric() const override { return true; }

  bool isSimple() const override { return false; }
  bool isInt() const override { return false; }
  bool isFloat() const override { return false; }
  bool isDouble() const override { return false; }
  bool isCompound() const override { return false; }
  bool isArray() const override { return false; }
  bool isStructure() const override { return false; }
  bool isVector() const override { return false; }
  bool isFunction() const override { return false; }
}; // class GenericType

class SimpleType : public IType {
public:
  bool isSimple() const override { return true; }

  bool isGeneric() const override { return false; }
  bool isInt() const override { return false; }
  bool isFloat() const override { return false; }
  bool isDouble() const override { return false; }
  bool isCompound() const override { return false; }
  bool isArray() const override { return false; }
  bool isStructure() const override { return false; }
  bool isVector() const override { return false; }
  bool isFunction() const override { return false; }
}; // class SimpleType

class IntType : public SimpleType {
public:
  bool isInt() const override { return true; }

protected:
  size_t bitwidth;
}; // class IntType

class FloatType : public SimpleType {
public:
  bool isFloat() const override { return true; }

}; // class FloatType

class DoubleType : public SimpleType {
public:
  bool isDouble() const override { return true; }

}; // class DoubleType

class CompoundType : public IType {
public:
  bool isCompound() const override { return true; }

  bool isGeneric() const override { return false; }
  bool isSimple() const override { return false; }
  bool isInt() const override { return false; }
  bool isFloat() const override { return false; }
  bool isDouble() const override { return false; }
  bool isArray() const override { return false; }
  bool isStructure() const override { return false; }
  bool isVector() const override { return false; }
  bool isFunction() const override { return false; }
}; // class CompoundType

class ArrayType : public CompoundType {
public:
  bool isArray() const override { return true; }

protected:
  IType *elemType;
  size_t size;
}; // class ArrayType

class StructureType : public CompoundType {
public:
  bool isStructure() const override { return true; }

protected:
  std::vector<IType *> nestedTypes;
}; // class StructureType

class VectorType : public CompoundType {
public:
  bool isVector() const override { return true; }

protected:
  IType *elemType;
  size_t size;
}; // class VectorType

class FunctionType : public CompoundType {
public:
  bool isFunction() const override { return true; }

protected:
  IType *returnType;
  std::vector<IType *> args;
}; // class FunctionType

} // namespace wsheeet
