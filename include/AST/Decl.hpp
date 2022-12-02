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
#include "SymbolTable.hpp"
#include "TreeNode.hpp"

#include <DebugLoc.hpp>
#include <Type.hpp>

#include <list>
#include <string>
#include <string_view>

namespace wsheeet::AST {

class IStmt;
class LayerDecl;

class Scope : public ITreeNode {}; // class Scope

class IDeclStmt : public IStmt {
public:
  [[nodiscard]] virtual std::string_view name() const = 0;
}; // class IDecl

class GlobalScope : public TreeNodeWManyCh<LayerDecl> {
public:
  GlobalScope() = default;
};

class LayerDecl final : public TreeNodeWParentAndCh<GlobalScope, CompoundStmt>,
                        public IDeclStmt {
  unsigned LayerNum_;
  std::string TUName_;

public:
  LayerDecl(GlobalScope &GScope, CompoundStmt &CStmt, unsigned LayerNumber)
      : TreeNodeWParentAndCh{&GScope, &CStmt}, LayerNum_{LayerNumber},
        TUName_{"unnamed layer"} {}
  LayerDecl(GlobalScope &GScope, CompoundStmt &CStmt, unsigned LayerNumber,
            std::string_view Name)
      : TreeNodeWParentAndCh{&GScope, &CStmt}, LayerNum_{LayerNumber},
        TUName_{Name} {}
}; // class LayerDecl

class GlueTypeDecl final : public TreeNodeWManyCh<IType>, public IDeclStmt {
  std::vector<Identifier> Members;
};

template <concepts::Expr InitExpr>
class ValueDecl : public TreeNodeWParentAnd2Ch<Scope, DeclRefExpr, InitExpr>,
                  public IDeclStmt {}; // class ValueDecl

template <concepts::Type T>
class VarDecl : public TreeNodeWParentAnd2Ch<Scope, DeclRefExpr, IExpr>,
                public IDeclStmt {}; // class VarDecl

class FunctionTypeDecl final : public TreeNodeW2Ch<IType, ArgList<IType>>,
                               public IDeclStmt {}; // class FunctionTypeDecl

class FunctionDecl
    : public TreeNodeWParentAnd2Ch<Scope, DeclRefExpr, CompoundStmt>,
      public IDeclStmt {
protected:
public:
  FunctionDecl(Scope &S, DeclRefExpr &I, CompoundStmt &CStmt)
      : TreeNodeWParentAnd2Ch<Scope, DeclRefExpr, CompoundStmt>(&S, &I,
                                                                &CStmt) {}
}; // class FnDecl

} // namespace wsheeet::AST
