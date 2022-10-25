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

#include <concepts>
#include <memory>
#include <type_traits>

namespace wsheeet::AST {

class ITreeNode {
public:
}; // class ITreeNode

template <typename P> class ITreeNodeWParent : public ITreeNode {
public:
  using ParentTy = P;

  virtual bool isRoot() const = 0;

  virtual ParentTy *parent() = 0;
  virtual const ParentTy *parent() const = 0;

  virtual ITreeNodeWParent *linkParent(ParentTy) = 0;
  virtual ITreeNodeWParent *unlinkParent(ParentTy) = 0;
}; // class ITreeNodeWParent

template <typename P> class TreeNodeWParent : public ITreeNodeWParent<P> {
public:
  using ParentTy = P;

protected:
  ParentTy *parent_;

  TreeNodeWParent(ParentTy *p = nullptr) : parent_{p} {}

public:
  bool isRoot() const override { return parent_ == nullptr; }

  TreeNodeWParent *parent() override { return parent_; }
  const TreeNodeWParent *parent() const override { return parent_; }

  TreeNodeWParent *linkParent(ParentTy *parent) override { parent_ = parent; }

  TreeNodeWParent *unlinkParent() override { parent_ = nullptr; }

  virtual ~TreeNodeWParent() {}
}; // class ITreeNode

template <typename C> class ITreeNodeWCh : public ITreeNode {
public:
  using ChildTy = C;

  virtual ITreeNodeWCh *linkChild(ChildTy *child) = 0;
  virtual ITreeNodeWCh *unlinkChild() = 0;
}; // class ITreeNodeWCh

template <typename C> class ITreeNodeW2Ch : public ITreeNodeWCh<C> {
public:
  using ChildrenTy = C;

  virtual ChildrenTy *left() = 0;
  virtual const ChildrenTy *left() const = 0;

  virtual ChildrenTy *right() = 0;
  virtual const ChildrenTy *right() const = 0;

  virtual ITreeNodeW2Ch *linkLeft(ChildrenTy *child) = 0;
  virtual ITreeNodeW2Ch *linkRight(ChildrenTy *child) = 0;
}; // class TreeNodeW2Ch

template <typename T, typename C>
concept CanLinkChildren = std::derived_from<T, ITreeNodeWCh<C>>;

template <typename C1, typename C2 = C1>
class TreeNodeW2Ch : public ITreeNodeW2Ch<C1>, public ITreeNodeW2Ch<C2> {
public:
  using LeftChildrenTy = C1;
  using RightChildrenTy = C2;

protected:
  LeftChildrenTy *left_;
  RightChildrenTy *right_;

  TreeNodeW2Ch(LeftChildrenTy *l = nullptr, RightChildrenTy *r = nullptr)
      : left_{l}, right_{r} {}

public:
  LeftChildrenTy *left() override { return left_; }
  const LeftChildrenTy *left() const override { return left_; }

  RightChildrenTy *right() override { return right_; }
  const RightChildrenTy *right() const override { return right_; }

  TreeNodeW2Ch *linkLeft(LeftChildrenTy *child) override {
    left_ = child;
    return this;
  }

  TreeNodeW2Ch *linkRight(RightChildrenTy *child) override {
    right_ = child;
    return this;
  }

  TreeNodeW2Ch *linkChild(RightChildrenTy *child) requires(
      std::same_as<std::remove_cvref_t<LeftChildrenTy>,
                   std::remove_cvref_t<RightChildrenTy>>) override {
    return linkRight(child);
  }

  TreeNodeW2Ch *unlinkChild(RightChildrenTy *child) requires(
      std::same_as<std::remove_cvref_t<LeftChildrenTy>,
                   std::remove_cvref_t<RightChildrenTy>>) override {
    if (left_ == child) {
      left_ = nullptr;
      return *this;
    }

    if (right_ == child) {
      right_ = nullptr;
      return *this;
    }

    return this;
  }

  TreeNodeW2Ch *linkChild(LeftChildrenTy *child) requires(
      !std::same_as<std::remove_cvref_t<LeftChildrenTy>,
                    std::remove_cvref_t<RightChildrenTy>>) override {
    return linkLeft(child);
  }

  TreeNodeW2Ch *unlinkChild(LeftChildrenTy *child) requires(
      !std::same_as<std::remove_cvref_t<LeftChildrenTy>,
                    std::remove_cvref_t<RightChildrenTy>>) override {
    if (left_ == child) {
      left_ = nullptr;
      return *this;
    }

    return this;
  }

  TreeNodeW2Ch *linkChild(RightChildrenTy *child) requires(
      !std::same_as<std::remove_cvref_t<LeftChildrenTy>,
                    std::remove_cvref_t<RightChildrenTy>>) override {
    return linkRight(child);
  }

  TreeNodeW2Ch *unlinkChild(RightChildrenTy *child) requires(
      !std::same_as<std::remove_cvref_t<LeftChildrenTy>,
                    std::remove_cvref_t<RightChildrenTy>>) override {
    if (right_ == child) {
      right_ = nullptr;
      return *this;
    }

    return this;
  }
}; // class TreeNodeW2Ch

template <typename P, typename C1, typename C2 = C1>
class TreeNodeWParentAnd2Ch : public TreeNodeWParent<P>,
                              public TreeNodeW2Ch<C> {
public:
  using ParentTy = P;
  using LeftChildrenTy = C1;
  using RightChildrenTy = C2;

protected:
  using TreeNodeWParent<ParentTy>::parent_;
  using TreeNodeW2Ch<LeftChildrenTy>::left_;
  using TreeNodeW2Ch<RightChildrenTy>::right_;

  TreeNodeWParentAnd2Ch(ParentTy *p, LeftChildrenTy *l = nullptr,
                        RightChildrenTy *r = nullptr)
      : TreeNodeWParent<ParentTy>{p},
        TreeNodeW2Ch<LeftChildrenTy, RightChildrenTy>{l, r} {
    if (left_ != nullptr)
      left_->parent_ = this;

    if (right_ != nullptr)
      right_->parent_ = this;
  }

public:
  TreeNodeWParentAnd2Ch *linkLeft(LeftChildrenTy *child) override {
    if (child == nullptr) {
      left_ = nullptr;
      return this;
    }

    left_ = child;
    left_->parent_ = this;
    return this;
  }

  TreeNodeWParentAnd2Ch *linkRight(RightChildrenTy *child) override {
    if (child == nullptr) {
      right_ = nullptr;
      return this;
    }

    right_ = child;
    right_->parent_ = this;
    return this;
  }

  TreeNodeWParentAnd2Ch *linkParent(ParentTy *parent) requires(
      CanLinkChildren<ParentTy, decltype(*this)>) override {
    parent_.unlinkChild(this);
    parent_ = parent;
    parent_.linkChild(this);
  }
}; // class TreeNodeW2Ch

template <typename P, typename C>
class TreeNodeWParentAndManyCh : public TreeNodeWParent<P>, public TreeNodeWManyCh<C> {};
} // namespace wsheeet::AST
