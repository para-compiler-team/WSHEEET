#pragma once

namespace ast {

using Datum = int;

struct INode {
  virtual Datum eval() = 0;
  virtual ~INode() {}
};

struct ValueNode final : INode {
  Datum Value{};

  ValueNode(Datum const &V) : Value{V} {}

  Datum eval() override { return Value; }
  ~ValueNode() {}
};

struct BinaryNode : INode {

  INode *lft{};
  INode *rgh{};

  BinaryNode(INode *lft, INode *rgh) : lft{lft}, rgh{rgh} {}

  virtual Datum eval() override = 0;
  virtual ~BinaryNode() {}
};

struct Plus final : BinaryNode {
  Plus(INode *lft, INode *rgh) : BinaryNode{lft, rgh} {}
  Datum eval() override {
    Datum lft_r = lft->eval();
    Datum rft_r = rgh->eval();
    return lft_r + rft_r;
  }
};

} // namespace ast
