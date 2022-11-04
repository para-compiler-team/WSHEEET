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

template <typename P> class ITreeNodeWParent : public virtual ITreeNode {
public:
  using ParentTy = P;

  virtual bool isRoot() const = 0;

  virtual ParentTy *parent() = 0;
  virtual const ParentTy *parent() const = 0;

  virtual ITreeNodeWParent *linkParent(ParentTy *) = 0;
  virtual ITreeNodeWParent *unlinkParent() = 0;
}; // class ITreeNodeWParent

template <typename T, typename P>
concept CanLinkParent = std::derived_from<T, ITreeNodeWParent<P>>;

template <typename P> class TreeNodeWParent : public ITreeNodeWParent<P> {
public:
  using ParentTy = P;

protected:
  ParentTy *parent_;

  TreeNodeWParent(ParentTy *p = nullptr) : parent_{p} {}

public:
  bool isRoot() const override { return parent_ == nullptr; }

  ParentTy *parent() override { return parent_; }
  const ParentTy *parent() const override { return parent_; }

  // using ITreeNodeWParent<P>::linkParent;
  ITreeNodeWParent<P> *linkParent(ParentTy *parent) override {
    parent_ = parent;
    return this;
  }

  // using ITreeNodeWParent<P>::unlinkParent;
  ITreeNodeWParent<P> *unlinkParent() override {
    parent_ = nullptr;
    return this;
  }

  virtual ~TreeNodeWParent() {}
}; // class ITreeNodeWParent

template <typename C> class ITreeNodeWCh : public virtual ITreeNode {
public:
  using ChildTy = C;

  virtual ITreeNodeWCh *linkChild(ChildTy *child) = 0;
  virtual ITreeNodeWCh *unlinkChild(ChildTy *child) = 0;
#ifdef WITH_VIRTUAL_UNLINK_CHILD
  virtual ITreeNodeWCh *unlinkChild() = 0;
#endif
}; // class ITreeNodeWCh

template <typename C> class TreeNodeWCh : public ITreeNodeWCh<C> {
public:
  using ChildTy = C;

protected:
  ChildTy *child_;

  TreeNodeWCh(ChildTy *child = nullptr) : child_{child} {}

public:
  ITreeNodeWCh<C> *linkChild(ChildTy *child) override {
    child_ = child;
    return this;
  }

  ITreeNodeWCh<C> *unlinkChild(ChildTy *child) override {
    if (child_ == child)
      child_ = nullptr;
    return this;
  }
}; // Class TreeNodeWCh

template <typename P, typename C>
class TreeNodeWParentAndCh : public virtual TreeNodeWParent<P>,
                             public virtual TreeNodeWCh<C> {
public:
  using ParentTy = P;
  using ChildTy = C;

protected:
  using TreeNodeWParent<ParentTy>::parent_;
  using TreeNodeWCh<ChildTy>::child_;

  TreeNodeWParentAndCh(ParentTy *p = nullptr, ChildTy *c = nullptr)
      : TreeNodeWParent<P>{p}, TreeNodeWCh<C>{c} {}
}; // class TreeNodeWParentAndCh

template <typename C1, typename C2>
class ITreeNodeW2Ch : public ITreeNodeWCh<C1>, public ITreeNodeWCh<C2> {
public:
  using LeftChildTy = C1;
  using RightChildTy = C2;

  virtual LeftChildTy *left() = 0;
  virtual const LeftChildTy *left() const = 0;

  virtual RightChildTy *right() = 0;
  virtual const RightChildTy *right() const = 0;

  virtual ITreeNodeW2Ch *linkLeft(LeftChildTy *child) = 0;
  virtual ITreeNodeW2Ch *linkRight(RightChildTy *child) = 0;
}; // class TreeNodeW2Ch

template <typename C> class ITreeNodeW2Ch<C, C> : public ITreeNodeWCh<C> {
public:
  using LeftChildTy = C;
  using RightChildTy = C;

  virtual LeftChildTy *left() = 0;
  virtual const LeftChildTy *left() const = 0;

  virtual RightChildTy *right() = 0;
  virtual const RightChildTy *right() const = 0;

  virtual ITreeNodeW2Ch *linkLeft(LeftChildTy *child) = 0;
  virtual ITreeNodeW2Ch *linkRight(RightChildTy *child) = 0;
}; // class TreeNodeW2Ch

template <typename T, typename C>
concept CanLinkChildren = std::derived_from<T, ITreeNodeWCh<C>>;

template <typename C1, typename C2>
class TreeNodeW2Ch : public ITreeNodeW2Ch<C1, C2> {
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

  TreeNodeW2Ch *linkChild(LeftChildTy *child) override {
    return linkLeft(child);
  }

  TreeNodeW2Ch *unlinkChild(LeftChildTy *child) override {
    if (left_ == child) {
      left_ = nullptr;
      return this;
    }

    return this;
  }

  TreeNodeW2Ch *linkChild(RightChildTy *child) override {
    return linkRight(child);
  }

  TreeNodeW2Ch *unlinkChild(RightChildTy *child) override {
    if (right_ == child) {
      right_ = nullptr;
      return this;
    }

    return this;
  }
}; // class TreeNodeW2Ch

template <typename C> class TreeNodeW2Ch<C, C> : public ITreeNodeW2Ch<C, C> {
public:
  using LeftChildTy = C;
  using RightChildTy = C;

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

  TreeNodeW2Ch *linkChild(RightChildTy *child) override {
    return linkRight(child);
  }

  TreeNodeW2Ch *unlinkChild(RightChildTy *child) override {
    if (left_ == child) {
      left_ = nullptr;
      return this;
    }

    if (right_ == child) {
      right_ = nullptr;
      return this;
    }

    return this;
  }
}; // class TreeNodeW2Ch

template <typename P, typename C1, typename C2>
class TreeNodeWParentAnd2Ch : public TreeNodeWParent<P>,
                              public TreeNodeW2Ch<C1, C2> {
public:
  using ParentTy = P;
  using LeftChildTy = C1;
  using RightChildTy = C2;

protected:
  using TreeNodeWParent<ParentTy>::parent_;
  using TreeNodeW2Ch<LeftChildTy, RightChildTy>::left_;
  using TreeNodeW2Ch<LeftChildTy, RightChildTy>::right_;

  TreeNodeWParentAnd2Ch(ParentTy *p, LeftChildTy *l = nullptr,
                        RightChildTy *r = nullptr)
      : TreeNodeWParent<ParentTy>{p}, TreeNodeW2Ch<LeftChildTy, RightChildTy>{
                                          l, r} {
    if constexpr (CanLinkParent<LeftChildTy, decltype(*this)>) {
      if (left_ != nullptr)
        left_->linkParent(this);
    }

    if constexpr (CanLinkParent<LeftChildTy, decltype(*this)>) {
      if (right_ != nullptr)
        right_->linkParent(this);
    }
  }

public:
  TreeNodeWParentAnd2Ch *linkLeft(LeftChildTy *child) override {
    if (child == nullptr) {
      left_ = nullptr;
      return this;
    }

    left_ = child;
    if constexpr (CanLinkParent<LeftChildTy, decltype(*this)>) {
      left_->linkParent(this);
    }
    return this;
  }

  TreeNodeWParentAnd2Ch *linkRight(RightChildTy *child) override {
    if (child == nullptr) {
      right_ = nullptr;
      return this;
    }

    right_ = child;
    if constexpr (CanLinkParent<RightChildTy, decltype(*this)>) {
      right_->linkParent(this);
    }
    return this;
  }

  TreeNodeWParentAnd2Ch *linkParent(ParentTy *parent) override {
    if constexpr (CanLinkChildren<ParentTy, decltype(*this)>)
     parent_.unlinkChild(this);

    parent_ = parent;
    if constexpr (CanLinkChildren<ParentTy, decltype(*this)>)
      parent_.linkChild(this);

    return this;
  }
}; // class TreeNodeW2Ch

#if ENABLE_NODE_WITH_MANY_CH
template <typename P, typename C>
class TreeNodeWParentAndManyCh : public TreeNodeWParent<P>,
                                 public TreeNodeWManyCh<C> {};
#endif

} // namespace wsheeet::AST
