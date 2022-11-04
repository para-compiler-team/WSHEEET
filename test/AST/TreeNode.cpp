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

#include <AST/Decl.hpp>
#include <AST/Expr.hpp>
#include <AST/Stmt.hpp>
#include <Type.hpp>

#include <gtest/gtest.h>

TEST(ASTTest, TreeNode) {
  // wsheeet::AST::TreeNodeWParent<int> nodeWIntParent;
  wsheeet::AST::ConstSimpleValueExpr<wsheeet::IntType> expr(wsheeet::IntType{},
                                                            0);
  auto gs = wsheeet::AST::GlobalScope{};
  auto in = wsheeet::AST::IdentifierNode{};
  auto ft = wsheeet::AST::CompoundStmt{};
  wsheeet::AST::FnDecl fn_decl{&gs, &in, &ft};
  EXPECT_TRUE(true);
}
