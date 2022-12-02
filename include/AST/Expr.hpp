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

#include "Decl.hpp"
#include "Stmt.hpp"
#include "SymbolTable.hpp"
#include "TreeNode.hpp"

#include <Type.hpp>

#include <concepts>
#include <string>
#include <string_view>

namespace wsheeet::AST {

class GlueDecl;

class IExprParent : public ITreeNode {}; // class ExprParent

class IExpr : public IExprParent {
public:
  virtual IType &type() = 0;
  virtual std::ostream &print(std::ostream &os) = 0;
}; // class Expr

namespace concepts {
template <typename T>
concept Expr = std::derived_from<T, IExpr>;
} // namespace concepts

class ExprBase : public IExpr {
protected:
  IType *Type;

  ExprBase(IType &Ty) : Type{&Ty} {}

public:
  IType &type() override { return *Type; }
}; // class ExprBase

template <concepts::SimpleType T>
class ConstSimpleValueExpr : public TreeNodeWParent<IExprParent>,
                             public ExprBase {
public:
  using TypeTy = T;
  using ValueTy = typename TypeTy::ValueTy;

protected:
  ValueTy Value;

public:
  ConstSimpleValueExpr(IExprParent *P, ValueTy Val)
      : TreeNodeWParent{P}, ExprBase{TypeTy::get()}, Value{Val} {}

  ValueTy value() const { return Value; }
}; // class ConstValueExpr

using IntLiteral = ConstSimpleValueExpr<IntType>;

template <std::floating_point F>
using FPLiteral = ConstSimpleValueExpr<FPType<F>>;

class DeclRefExpr : public TreeNodeWParentAndCh<IExprParent, IExpr>, public ExprBase {
protected:
  Identifier Id_;

public:
}; // class DeclRefExpr

class CommaExpr final : public TreeNodeWParentAnd2Ch<IExprParent, IExpr, IExpr>,
                  public ExprBase {}; // class CommaExpr

enum class UnOpcode {
  PLUS,
  MINUS,
  BNOT,
  NOT
};

class UnOpExpr final : public TreeNodeWParentAndCh<IExprParent, IExpr>,
                 public ExprBase {}; // class UnOpExpr

enum class BinOpcode {
  PLUS,
  MINUS,
  MUL,
  DIV,
  REM,
  SHL,
  SHR,
  AND,
  OR,
  XOR,
  LAND,
  LOR,
  EQ,
  NEQ,
  LESS,
  MORE,
  LEQ,
  MEQ,
  COMMA,
  ASSIGN,
  PLUS_ASSIGN,
  MINUS_ASSIGN,
  MUL_ASSIGN,
  DIV_ASSIGN,
  REM_ASSIGN,
  SHL_ASSIGN,
  SHR_ASSIGN,
  AND_ASSIGN,
  OR_ASSIGN,
  XOR_ASSIGN
};

class BinOpExpr final : public TreeNodeWParentAnd2Ch<IExprParent, IExpr, IExpr>,
                        public ExprBase {
  BinOpcode Opcode;
}; // class BinOpExpr

template <typename ArgT>
class ArgList : public TreeNodeWParentAndManyCh<IExprParent, ArgT> {
}; // class ArgList

class CallExpr final : public TreeNodeWParentAnd2Ch<IExprParent, IExpr, ArgList<IExpr>>,
                       public ExprBase {}; // class CallExpr

class ElemAccessExpr : public ExprBase {}; // class ElemAccessExpr

class NamedElemAccessExpr
    : public TreeNodeWParentAnd2Ch<IExprParent, IExpr, DeclRefExpr>,
      public ElemAccessExpr {}; // class NamedElemAccessExpr

class UnnamedElemAccessExpr
    : public TreeNodeWParentAnd2Ch<IExprParent, IExpr, IExpr>,
      public ElemAccessExpr {}; // class UnnamedElemAccessExpr

class InputExpr final : public TreeNodeWParentAndCh<IExprParent, IExpr>,
                  public ExprBase {}; // class InputExpr

class OutputExpr final : public TreeNodeWParentAnd2Ch<IExprParent, IExpr, IExpr>,
                   public ExprBase {}; // class OutputExpr

class RepeatExpr final : public TreeNodeWParentAnd2Ch<IExprParent, IExpr, IExpr>,
                   public ExprBase {}; // class RepeatExpr

class GlueExpr final : public TreeNodeWParentAndCh<IExprParent, ArgList<GlueDecl>>,
                 public ExprBase {}; // class GlueExpr

class BindExpr final : public TreeNodeWParentAnd2Ch<IExprParent, DeclRefExpr,
                                              ArgList<NamedElemAccessExpr>>,
                 public ExprBase {}; // class BindExpr

} // namespace wsheeet::AST
