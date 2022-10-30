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

template <typename P>
class ITreeNodeWParent : public virtual ITreeNode {
public:
  using ParentTy = P;

  virtual bool isRoot() const = 0;

  virtual ParentTy *parent() = 0;
  virtual const ParentTy *parent() const = 0;

  virtual ITreeNodeWParent *linkParent(ParentTy) = 0;
  virtual ITreeNodeWParent *unlinkParent(ParentTy) = 0;
}; // class ITreeNodeWParent

template <typename P>
class TreeNodeWParent : public ITreeNodeWParent<P> {
public:
  using ParentTy = P;

protected:
  ParentTy *parent_;

  TreeNodeWParent(ParentTy *p = nullptr) : parent_{p} {}

public:
  bool isRoot() const override { return parent_ == nullptr; }

  ParentTy *parent() override { return parent_; }
  const ParentTy *parent() const override { return parent_; }

  using ITreeNodeWParent<P>::linkParent;
  ITreeNodeWParent<P> *linkParent(ParentTy *parent) override { parent_ = parent; }
  using ITreeNodeWParent<P>::unlinkParent;
  ITreeNodeWParent<P> *unlinkParent() override { parent_ = nullptr; }

  virtual ~TreeNodeWParent() {}
}; // class ITreeNode

template <typename C>
class ITreeNodeWCh : public virtual ITreeNode {
public:
  using ChildTy = C;

  virtual ITreeNodeWCh *linkChild(ChildTy *child) = 0;
  virtual ITreeNodeWCh *unlinkChild() = 0;
}; // class ITreeNodeWCh

template <typename C>
class TreeNodeWCh : public ITreeNodeWCh<C> {
public:
  using ChildTy = C;

protected:
  ChildTy *child_;

  TreeNodeWCh(ChildTy *child) : child_{child} {}

public:
  ITreeNodeWCh<C> *linkChild(ChildTy *child) override { child_ = child; }
  ITreeNodeWCh<C> *unlinkChild() override { child_ = nullptr; }
}; // class TreeNodeWCh

template <typename P, typename C>
class TreeNodeWParentAndCh : public virtual TreeNodeWParent<P>, public virtual TreeNodeWCh<C> {
public:
  using ParentTy = P;
  using ChildTy = C;

protected:
  using TreeNodeWParent<ParentTy>::parent_;
  using TreeNodeWCh<ChildTy>::child_;


  TreeNodeWParentAndCh(ParentTy *p = nullptr, ChildTy *c = nullptr)
      : TreeNodeWParent<P>{p}, TreeNodeWCh<C>{c} {}
}; // class TreeNodeWParentAndCh

template <typename C1, typename C2 = C1>
class ITreeNodeW2Ch : public ITreeNodeWCh<C1> {
public:
  using LeftChildTy = C1;
  using RightChildTy = C2;

  virtual LeftChildTy *left() = 0;
  virtual const RightChildTy *left() const = 0;

  virtual LeftChildTy *right() = 0;
  virtual const RightChildTy *right() const = 0;

  virtual ITreeNodeW2Ch *linkLeft(LeftChildTy *child) = 0;
  virtual ITreeNodeW2Ch *linkRight(RightChildTy *child) = 0;
}; // class TreeNodeW2Ch

template <typename T, typename C>
concept CanLinkChildren = std::derived_from<T, ITreeNodeWCh<C>>;

template <typename C1, typename C2>
class TreeNodeW2Ch : public virtual ITreeNodeW2Ch<C1>, public virtual ITreeNodeW2Ch<C2> {
public:
  using LeftChildTy = C1;
  using RightChildTy = C2;

protected:
  LeftChildTy *left_;
  RightChildTy *right_;

  TreeNodeW2Ch(LeftChildTy *l = nullptr, RightChildTy *r = nullptr)
      : left_{l}, right_{r} {}

public:
  LeftChildTy *left() override { return left_; }
  const LeftChildTy *left() const override { return left_; }

  RightChildTy *right() override { return right_; }
  const RightChildTy *right() const override { return right_; }

  TreeNodeW2Ch *linkLeft(LeftChildTy *child) override {
    left_ = child;
    return this;
  }

  TreeNodeW2Ch *linkRight(RightChildTy *child) override {
    right_ = child;
    return this;
  }

  TreeNodeW2Ch *linkChild(RightChildTy *child) requires(
      std::same_as<std::remove_cvref_t<LeftChildTy>,
                   std::remove_cvref_t<RightChildTy>>) override {
    return linkRight(child);
  }

  TreeNodeW2Ch *unlinkChild(RightChildTy *child) requires(
      std::same_as<std::remove_cvref_t<LeftChildTy>,
                   std::remove_cvref_t<RightChildTy>>) override {
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

  TreeNodeW2Ch *linkChild(LeftChildTy *child) requires(
      !std::same_as<std::remove_cvref_t<LeftChildTy>,
                    std::remove_cvref_t<RightChildTy>>) override {
    return linkLeft(child);
  }

  TreeNodeW2Ch *unlinkChild(LeftChildTy *child) requires(
      !std::same_as<std::remove_cvref_t<LeftChildTy>,
                    std::remove_cvref_t<RightChildTy>>) override {
    if (left_ == child) {
      left_ = nullptr;
      return *this;
    }

    return this;
  }

  TreeNodeW2Ch *linkChild(RightChildTy *child) requires(
      !std::same_as<std::remove_cvref_t<LeftChildTy>,
                    std::remove_cvref_t<RightChildTy>>) override {
    return linkRight(child);
  }

  TreeNodeW2Ch *unlinkChild(RightChildTy *child) requires(
      !std::same_as<std::remove_cvref_t<LeftChildTy>,
                    std::remove_cvref_t<RightChildTy>>) override {
    if (right_ == child) {
      right_ = nullptr;
      return *this;
    }

    return this;
  }
}; // class TreeNodeW2Ch

template <typename P, typename C1, typename C2 = C1>
class TreeNodeWParentAnd2Ch : public TreeNodeWParent<P>,
                              public TreeNodeW2Ch<C1, C2> {
public:
  using ParentTy = P;
  using LeftChildTy = C1;
  using RightChildTy = C2;

protected:
  using TreeNodeWParent<ParentTy>::parent_;
  using TreeNodeW2Ch<LeftChildTy>::left_;
  using TreeNodeW2Ch<RightChildTy>::right_;

  TreeNodeWParentAnd2Ch(ParentTy *p, LeftChildTy *l = nullptr,
                        RightChildTy *r = nullptr)
      : TreeNodeWParent<ParentTy>{p},
        TreeNodeW2Ch<LeftChildTy, RightChildTy>{l, r} {
    if (left_ != nullptr)
      left_->parent_ = this;

    if (right_ != nullptr)
      right_->parent_ = this;
  }

public:
  TreeNodeWParentAnd2Ch *linkLeft(LeftChildTy *child) override {
    if (child == nullptr) {
      left_ = nullptr;
      return this;
    }

    left_ = child;
    left_->parent_ = this;
    return this;
  }

  TreeNodeWParentAnd2Ch *linkRight(RightChildTy *child) override {
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

#if ENABLE_NODE_WITH_MANY_CH
template <typename P, typename C>
class TreeNodeWParentAndManyCh : public TreeNodeWParent<P>, public TreeNodeWManyCh<C> {};
#endif

} // namespace wsheeet::AST
