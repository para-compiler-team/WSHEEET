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

#include "TreeNode.hpp"

#include <Type.hpp>

#include <string>
#include <string_view>

namespace wsheeet::AST {

class ExprBase {
}; // class ExprBase

template <concepts::SimpleType T>
class ConstSimpleValueExpr : ExprBase {
public:
  using TypeTy = T;
  using ValueTy = typename TypeTy::ValueTy;

protected:
  const TypeTy *type;
  ValueTy value;

public:
  ConstSimpleValueExpr(const TypeTy &t, ValueTy v) : type{&t}, value{v} {}
}; // class ConstValueExpr

class CharConstValueExpr : public ConstSimpleValueExpr<CharType> {
}; // class CharConstValueExpr

class IntConstValueExpr : public ConstSimpleValueExpr<IntType> {
}; // class IntConstValueExpr

class FloatConstValueExpr : public ConstSimpleValueExpr<FloatType> {
}; // class FloatConstValueExpr

class DoubleConstValueExpr : public ConstSimpleValueExpr<DoubleType> {
}; // class DoubleConstValueExpr

class UnOpExpr : public virtual ExprBase, public virtual TreeNodeWParentAndCh <ExprBase, ExprBase> {
}; // class UnOpExpr

class UnPlusExpr : public UnOpExpr {
}; // class UnPlusExpr

class UnMinusExpr : public UnOpExpr {
}; // class UnMinusExpr

class BinOpExpr : public ExprBase, public TreeNodeWParentAnd2Ch<ExprBase, ExprBase, ExprBase> {
}; // class BinOpExpr

class BinPlusExpr : public BinOpExpr {
}; // class BinPlusExpr

class BinMinusExpr : public BinOpExpr {
}; // class BinMinusExpr

class BinMultExpr : public BinOpExpr {
}; // class BinMultExpr

class BinDivExpr : public BinOpExpr {
}; // class BinDivExpr

class ModifyExpr : public BinOpExpr {
}; // class ModifyExpr

} // namespace wsheeet::AST
