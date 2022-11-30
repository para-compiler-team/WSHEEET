#pragma once

#include "datum.h"

namespace visitor {
class IVisitor;
} // namespace visitor

namespace ast {

using Datum = int;

struct INode {
  virtual Datum eval() = 0;
  virtual detail::Datum accept(visitor::IVisitor &) = 0;
  virtual ~INode() {}
};

struct ValueNode final : INode {
  detail::Datum Value{};

  ValueNode(detail::Datum const &V) : Value{V} {}

  // This has to be included.
  detail::Datum accept(visitor::IVisitor &) override;

  Datum eval() override { return boost::get<int>(Value); }
  ~ValueNode() {}
};

struct BinaryNode : INode {

  INode *lft{};
  INode *rgh{};

  BinaryNode(INode *lft, INode *rgh) : lft{lft}, rgh{rgh} {}

  virtual Datum eval() override = 0;
  virtual detail::Datum accept(visitor::IVisitor &) override = 0;
  virtual ~BinaryNode() {}
};

struct PlusNode final : BinaryNode {
  PlusNode(INode *lft, INode *rgh) : BinaryNode{lft, rgh} {}
  detail::Datum accept(visitor::IVisitor &) override;

  Datum eval() override {
    Datum lft_r = lft->eval();
    Datum rft_r = rgh->eval();
    return lft_r + rft_r;
  }
};

} // namespace ast
