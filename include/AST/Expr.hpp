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

using Scope = ITreeNode;

class ExprParent : public ITreeNode {}; // class ExprParent

class Expr : public ExprParent {}; // class Expr

template <concepts::SimpleType T> class ConstSimpleValueExpr : public Expr {
public:
  using TypeTy = T;
  using ValueTy = typename TypeTy::ValueTy;

protected:
  const TypeTy *Type;
  ValueTy Value;

public:
  ConstSimpleValueExpr(ValueTy V) : Type{TypeTy::get()}, Value{V} {}

  ValueTy getValue() const { return Value; }
}; // class ConstValueExpr

using IntLiteral = ConstSimpleValueExpr<IntType>;

template <std::floating_point F>
using FPLiteral = ConstSimpleValueExpr<FPType<F>>;

class DeclRefExpr : public Expr,
                    public TreeNodeWParent<ExprParent> {}; // class DeclRefExpr

class CommaExpr : public Expr,
                  public TreeNodeWParentAnd2Ch<ExprParent, Expr, Expr> {
}; // class CommaExpr

class UnOpExpr : public Expr,
                 public TreeNodeWParentAndCh<ExprParent, Expr> {
}; // class UnOpExpr

class BinOpExpr : public Expr,
                  public TreeNodeWParentAnd2Ch<ExprParent, Expr, Expr> {
}; // class BinOpExpr

template <typename ArgT>
class ArgList : public TreeNodeWParentAndManyCh<ExprParent, ArgT> {
}; // class ArgList

class CallExpr : public Expr,
                 public TreeNodeWParentAndCh<ExprParent, ArgList<Expr>> {
}; // class CallExpr

class ElemAccessExpr : public Expr {}; // class ElemAccessExpr

class NamedElemAccessExpr
    : public ElemAccessExpr,
      public TreeNodeWParentAnd2Ch<ExprParent, Expr, DeclRefExpr> {
}; // class NamedElemAccessExpr

class UnnamedElemAccessExpr
    : public ElemAccessExpr,
      public TreeNodeWParentAnd2Ch<ExprParent, Expr, Expr> {
}; // class UnnamedElemAccessExpr

class InputExpr : public Expr,
                  public TreeNodeWParentAndCh<ExprParent, Expr> {
}; // class InputExpr

class OutputExpr : public Expr,
                   public TreeNodeWParentAnd2Ch<ExprParent, Expr, Expr> {
}; // class OutputExpr

class RepeatExpr : public Expr,
                   public TreeNodeWParentAnd2Ch<ExprParent, Expr, Expr> {
}; // class RepeatExpr

class GlueDecl;
class GlueExpr : public Expr,
                 public TreeNodeWParentAndCh<ExprParent, ArgList<GlueDecl>> {
}; // class GlueExpr

class BindExpr : public Expr,
                 public TreeNodeWParentAnd2Ch<ExprParent, DeclRefExpr,
                                              ArgList<NamedElemAccessExpr>> {
}; // class BindExpr

} // namespace wsheeet::AST
