#include <AST/Expr.hpp>
#include <AST/Decl.hpp>
#include <AST/Stmt.hpp>
#include <AST/TypeBuilder.hpp>

#include <visitor/visitor.h>

#include <iostream>

int main() try {
  using namespace wsheeet;
  ast::TypeBuilder TB{};
  auto &Int = TB.get<ast::IntTy>();
  auto &F = TB.get<ast::FloatTy>();
  auto *Const2 = new ast::ConstSimpleValueExpr<ast::IntTy>{Int, 2};
  auto *Const3 = new ast::ConstSimpleValueExpr<ast::FloatTy>{F, 1.f};
  auto *Expr = new ast::BinOpExpr{*Const2, *Const3, ast::BinOpcode::PLUS};
  auto *Stmt = new ast::ExprStmt{*Expr};
  auto *LayerBody = new ast::CompoundStmt{*Stmt};
  auto *LayerDecl = new ast::LayerDecl{*LayerBody, 0};
  auto GS = std::make_unique<ast::GlobalScope>(*LayerDecl);

  // USAGE
  visitor::InterpreitVisitor IV;
  std::cout << LayerBody->accept(IV) << '\n';

  LayerDecl->print(std::cout);
  std::cout << std::endl;
  GS->print(std::cout);
  std::cout << std::endl;
  return 0;
} catch (const std::exception &e) {
  std::cerr << e.what() << std::endl;
}
