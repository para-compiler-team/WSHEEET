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

#inlcude <DebugLoc.hpp>
#include <Stmt.hpp>
#include <Types/Type.h>

#include <string>

namespace wsheeet::AST {

class Identifier : ITreeNode {};

class Scope : ITreeNode {};

class IDecl {
public:
  virtual const std::string &getName() const = 0;
  virtual DebugLoc &getLoc() const = 0;
}; // class IDecl

class FnDecl : public IDecl, public TreeNodeW2Ch<FunctionType, CompoundStmt> {
protected:

public:
  FnDecl(Scope &s, Identifier i) : TreeNodeWParentAnd2Ch<Scope, Identifier, CompoundStmt>(&s, i) {}
}; // class FnDecl

class FnDef : public FnDecl {}; // class FnDef

class VarDecl : public IDecl, public TreeNode {}; // class VarDecl

class VarDef : public IDecl, public TreeNode {}; // class VarDef

} // namespace wsheeet::AST
