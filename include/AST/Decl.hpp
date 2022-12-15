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

#include <string>
#include <string_view>

namespace wsheeet::ast {

class LayerDecl;

class GlobalScope : public TreeNodeWManyChildren<LayerDecl>,
                    public Scope,
                    visitor::AstVisitable<visitor::InterpreitVisitor> {
  using NodeT = TreeNodeWManyChildren<LayerDecl>;

public:
  GlobalScope() : NodeT{} {}
  GlobalScope(LayerDecl &Layer) : NodeT{&Layer} {}

  std::ostream &print(std::ostream &os) {
    os << "GlobalScope 0x" << std::hex << this;
    return os;
  }

  auto accept(visitor::InterpreitVisitor &v)
      -> visitor::InterpreitVisitor::retty override;
};

template <TreeNode ChildTy>
class DeclBase : public TreeNodeWParentAndChild<Scope, ChildTy>,
                 public Scope,
                 public visitor::AstVisitable<visitor::InterpreitVisitor> {
  using NodeT = TreeNodeWParentAndChild<Scope, ChildTy>;

public:
  DeclBase() = default;
  DeclBase(Scope &S) : NodeT(&S) {}
  DeclBase(ChildTy &C) : NodeT(&C) {}
  DeclBase(Scope &S, ChildTy &C) : NodeT(&S, &C) {}
};

class LayerDecl final : public DeclBase<CompoundStmt> {
  unsigned LayerNum_;
  std::string TUName_;

  using NodeT = TreeNodeWChild<CompoundStmt>;

public:
  LayerDecl(CompoundStmt &CStmt, unsigned LayerNumber)
      : DeclBase{CStmt}, LayerNum_{LayerNumber}, TUName_{"unnamed layer"} {}

  LayerDecl(CompoundStmt &CStmt, unsigned LayerNumber, std::string_view Name)
      : DeclBase{CStmt}, LayerNum_{LayerNumber}, TUName_{Name} {}

  LayerDecl(GlobalScope &GScope, CompoundStmt &CStmt, unsigned LayerNumber)
      : DeclBase{GScope, CStmt}, LayerNum_{LayerNumber}, TUName_{
                                                             "unnamed layer"} {}

  LayerDecl(GlobalScope &GScope, CompoundStmt &CStmt, unsigned LayerNumber,
            std::string_view Name)
      : DeclBase{GScope, CStmt}, LayerNum_{LayerNumber}, TUName_{Name} {}

  void linkScope(Scope *S) { linkParent(S); }
  void linkScope(Scope &&S) { linkParent(std::move(S)); }

  StmtBase &child() { return *Child_; }
  const StmtBase &child() const { return *Child_; }

  std::ostream &print(std::ostream &os) {
    os << "LayerDecl 0x" << std::hex << this << std::dec << '\'' << TUName_
       << "' #" << LayerNum_;
    return os;
  }

  auto accept(visitor::InterpreitVisitor &v)
      -> visitor::InterpreitVisitor::retty override;
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
