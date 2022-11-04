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
#include "Stmt.hpp"
#include "TreeNode.hpp"

#include <DebugLoc.hpp>
#include <Type.hpp>

#include <list>
#include <string>
#include <string_view>

namespace wsheeet::AST {

class IdentifierNode {
  std::string symbol;

public:
  [[nodiscard]] std::string_view name() const { return symbol; }
}; // class IdentifierNode

class IDecl {
public:
  virtual const std::string_view getName() const = 0;
}; // class IDecl

class GlobalScope : public TreeNodeWCh<std::list<IDecl>> {
public:
  GlobalScope() = default;
};

using Scope = ITreeNode;

template <concepts::Type T>
class VarDecl
    : public IDecl,
      public TreeNodeWParentAnd2Ch<Scope, IdentifierNode, ModifyExpr> {
}; // class VarDecl

class FnDecl : public IDecl, public TreeNodeWParentAnd2Ch<Scope, IdentifierNode, CompoundStmt> {
protected:
    FunctionType *FT;

public:
  FnDecl(Scope *S, IdentifierNode *I, CompoundStmt *CStmt)
      : TreeNodeWParentAnd2Ch<Scope, IdentifierNode, CompoundStmt>(S, I, CStmt) {}

    const std::string_view getName() const override {
      return left()->name();
    }
}; // class FnDecl

} // namespace wsheeet::AST
