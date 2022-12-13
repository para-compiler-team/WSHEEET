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

#include "Expr.hpp"
#include "TreeNode.hpp"

namespace wsheeet::ast {

class Scope : public ExprParent {};

class StmtBase : public Scope {};

class ExprStmt : public TreeNodeWParentAndChild<Scope, ExprBase>,
                 public StmtBase {
  using NodeT = TreeNodeWParentAndChild<Scope, ExprBase>;

public:
  ExprStmt(ExprBase &Expr) : NodeT{nullptr, &Expr} {}
  ExprStmt(Scope &S, ExprBase &Expr) : NodeT{&S, &Expr} {}

  void linkScope(Scope *S) { linkParent(S); }
  void linkScope(Scope &&S) { linkParent(std::move(S)); }
}; // class ExprStmt

class CompoundStmt : public TreeNodeWParentAndManyChildren<Scope, StmtBase>,
                     public StmtBase {
  using NodeT = TreeNodeWParentAndManyChildren<Scope, StmtBase>;

public:
  CompoundStmt() = default;
  CompoundStmt(Scope &S) : NodeT(&S), StmtBase{} {}
  CompoundStmt(StmtBase &Stmt) : NodeT(nullptr, &Stmt), StmtBase{} {}
  CompoundStmt(Scope &S, StmtBase &Stmt) : NodeT(&S, &Stmt), StmtBase{} {}

  void linkScope(Scope *S) { linkParent(S); }
  void linkScope(Scope &&S) { linkParent(std::move(S)); }

  std::ostream &print(std::ostream &os) {
    os << "CompoundStmt 0x" << this;
    return os;
  }
}; // class CompoundStmt

#ifdef WITH_COMPOUND_CF
class IfStmt
    : public TreeNodeWParentAnd2Ch<Scope, IStmt, IStmt, public StmtBase> {
}; // class IfStmt

class ForStmt : public TreeNodeWParentAnd3Ch<Scope, DeclRefExpr, IExpr, IStmt,
                                             public StmtBase> {
}; // class ForStmt

class WhileStmt
    : public TreeNodeWParentAnd2Ch<Scope, IExpr, IStmt, public StmtBase> {
}; // class WhileStmt
#endif

#ifdef WITH_FUNC
class ReturnStmt : public TreeNodeWParentAndCh<Scope, IExpr, public StmtBase> {
}; // class ReturnStmt
#endif

} // namespace wsheeet::ast
