// -------------------------------------------------------------------------- //
// Copyright 2022 Ilya Andreev, Yuly Tarasov
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
#include <AST/TypeBuilder.hpp>

#include <iostream>

int main() {
  using namespace wsheeet;
  std::cout << "Call from ast\n";

  ast::TypeBuilder TB{};

  auto &Int = TB.get<ast::IntTy>();
  std::cout << Int << '\n';
  auto &Int16 = TB.get<ast::IntTy>(16u);
  assert(&Int != &Int16);
  std::cout << Int16 << '\n';

  auto &Int16_2 = TB.get<ast::IntTy>(16u);
  std::cout << Int16_2 << '\n';
  assert(&Int16_2 == &Int16);

  ast::GlobalScope GS{};
  auto CI = ast::ConstSimpleValueExpr<ast::IntTy>{GS, Int, 5};
  auto &vector = TB.get<ast::VectorTy>(Int, CI);
  std::cout << vector << '\n';

  auto &VA = TB.get<ast::VectorTy>(Int16, CI);
  assert(&vector != &VA);
  std::cout << VA << '\n';

  // Is it range, or init list?
  auto &S = TB.get<ast::StructTy>(VA, vector, Int16, Int);
  auto &S_2 = TB.get<ast::StructTy>(VA, vector, Int16, Int);
  auto &S2 = TB.get<ast::StructTy>(VA, vector, Int16);
  assert(&S == &S_2);
  assert(&S != &S2);

  std::cout << S << '\n';
}
