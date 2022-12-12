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
#include "Stmt.hpp"

#include <string>
#include <string_view>

namespace wsheeet::ast {

class LayerDecl;
class Scope : public ExprParent {}; // class Scope

class GlobalScope : public TreeNodeWManyChildren<LayerDecl>, public Scope {
  using NodeT = TreeNodeWManyChildren<LayerDecl>;
public:
    GlobalScope() : NodeT{} {}
};

class DeclBase : public TreeNodeWParent<Scope> {
  using NodeT = TreeNodeWParent<Scope>;
public:
   DeclBase(Scope &S) : NodeT(&S) {}
};

class LayerDecl final : public DeclBase, public TreeNodeWChild<CompoundStmt>{
  unsigned LayerNum_;
  std::string TUName_;

  using NodeT = TreeNodeWChild<CompoundStmt>;

public:
  LayerDecl(GlobalScope &GScope, CompoundStmt &CStmt, unsigned LayerNumber)
    : DeclBase{GScope}, NodeT{&CStmt}, LayerNum_{LayerNumber},
        TUName_{"unnamed layer"} {}

  LayerDecl(GlobalScope &GScope, CompoundStmt &CStmt, unsigned LayerNumber,
            std::string_view Name)
      : DeclBase{GScope}, NodeT{&CStmt}, LayerNum_{LayerNumber},
        TUName_{Name} {}
}; // class LayerDecl

#ifdef WITH_GLUE
class GlueTypeDecl final : public TreeNodeWManyChildren<TypeBase> {
  std::vector<Identifier> Members;
};
#endif

#ifdef WITH_DECL_REF_EXPR
template <Type T>
class VarDecl : public TreeNodeWParentAnd2Ch<Scope, DeclRefExpr, ExprBase>;
#endif

#ifdef WITH_FUNCTION
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
#endif

} // namespace wsheeet::ast
