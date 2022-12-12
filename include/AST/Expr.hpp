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
#include "Type.hpp"
#include "TypeBuilder.hpp"

#include <concepts>
#include <string>
#include <string_view>

namespace wsheeet::ast {

class ExprParent {};

class ExprBase : public ExprParent {
protected:
  TypeBase *Type;
  ExprBase(TypeBase &Ty) : Type{&Ty} {}

public:
  TypeBase &type() { return *Type; }
}; // class ExprBase

template <typename T>
concept Expr = std::derived_from<T, ExprBase>;

template <SimpleType T>
class ConstSimpleValueExpr final : protected TreeNodeWParent<ExprParent>,
                                   public ExprBase {
public:
  using TypeTy = T;
  using ValueTy = typename TypeTy::ValueTy;

protected:
  ValueTy Value;

public:
  ConstSimpleValueExpr(wsheeet::ast::ExprParent &P, TypeTy &Ty, ValueTy Val)
      : TreeNodeWParent{&P}, ExprBase{Ty}, Value{Val} {}

  ValueTy value() const { return Value; }
}; // class ConstValueExpr

using IntLiteral = ConstSimpleValueExpr<IntTy>;

template <std::floating_point F>
using FPLiteral = ConstSimpleValueExpr<FPTy<F>>;

#ifdef WITH_DECL_REF_EXPR
class DeclRefExpr : public TreeNodeWParentAndCh<ExprParent, ExprBase>,
                    public ExprBase {
protected:
  Identifier Id_;

public:
}; // class DeclRefExpr
#endif

#ifdef WITH_COMMA_EXPR
class CommaExpr final
    : public TreeNodeWParentAnd2Children<ExprParent, ExprBase, ExprBase>,
      public ExprBase {}; // class CommaExpr
#endif

enum class UnOpcode { PLUS, MINUS, BNOT, NOT };

class UnOpExpr final : protected TreeNodeWParentAndChild<ExprParent, ExprBase>,
                       public ExprBase {
  UnOpcode Opcode_;

  using NodeT = TreeNodeWParentAndChild<ExprParent, ExprBase>;

public:
  UnOpExpr(ExprParent &Parent, ExprBase &Child, UnOpcode Opcode)
      : NodeT{&Parent, &Child}, ExprBase{Child.type()}, Opcode_(Opcode) {}

  auto opcode() const noexcept { return Opcode_; }
  ExprBase &child() { return *Child_; }
  const ExprBase &child() const { return *Child_; }
}; // class UnOpExpr

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

class BinOpExpr final
    : protected TreeNodeWParentAnd2Children<ExprParent, ExprBase, ExprBase>,
      public ExprBase {
  BinOpcode Opcode_;

  using NodeT = TreeNodeWParentAnd2Children<ExprParent, ExprBase, ExprBase>;

public:
  BinOpExpr(ExprParent &Parent, ExprBase &Left, ExprBase &Right, BinOpcode Opcode)
      : NodeT{&Parent, &Left, &Right}, ExprBase{Left.type().builder().getGeneric()}, Opcode_(Opcode) {}

  auto opcode() const noexcept { return Opcode_; }
  ExprBase &LHS() { return *Left_; }
  ExprBase &RHS() { return *Right_; }
  const ExprBase &LHS() const { return *Left_; }
  const ExprBase &RHS() const { return *Right_; }
}; // class BinOpExpr

#ifdef WITH_COMPLEX_EXPRS
template <typename ArgT>
class ArgList : public TreeNodeWParentAndManyCh<ExprParent, ArgT> {
}; // class ArgList

class CallExpr final
    : public TreeNodeWParentAnd2Ch<ExprParent, ExprBase, ArgList<ExprBase>>,
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

class OutputExpr final
    : public TreeNodeWParentAnd2Ch<IExprParent, IExpr, IExpr>,
      public ExprBase {}; // class OutputExpr

class RepeatExpr final
    : public TreeNodeWParentAnd2Ch<IExprParent, IExpr, IExpr>,
      public ExprBase {}; // class RepeatExpr

class GlueExpr final
    : public TreeNodeWParentAndCh<IExprParent, ArgList<GlueDecl>>,
      public ExprBase {}; // class GlueExpr

class BindExpr final
    : public TreeNodeWParentAnd2Ch<IExprParent, DeclRefExpr,
                                   ArgList<NamedElemAccessExpr>>,
      public ExprBase {}; // class BindExpr
#endif

} // namespace wsheeet::ast
