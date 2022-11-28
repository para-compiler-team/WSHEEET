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

#include <list>

namespace wsheeet::AST {

class IStmt : public IExprParent {}; // class StmtBase

class ExprStmt : public IStmt,
                 public TreeNodeWParentAndCh<Scope, IExpr> {}; // class ExprStmt

class CompoundStmt : public IStmt,
                     public TreeNodeWParentAndCh<Scope, std::list<IStmt>> {
}; // class CompoundStmt

class IfStmt : public IStmt,
               public TreeNodeWParentAnd2Ch<Scope, IStmt, IStmt> {
}; // class IfStmt

class ForStmt : public IStmt,
                public TreeNodeWParentAnd3Ch<Scope, DeclRefExpr, IExpr, IStmt> {
}; // class ForStmt

class WhileStmt : public IStmt,
                  public TreeNodeWParentAnd2Ch<Scope, IExpr, IStmt> {
}; // class WhileStmt

class ReturnStmt : public IStmt,
                   public TreeNodeWParentAndCh<Scope, IExpr> {
}; // class ReturnStmt

} // namespace wsheeet::AST
