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
#include <initializer_list>
#include <list>
#include <memory>
#include <stdexcept>

namespace wsheeet::ast {

class TreeNodeBase {
  public:
  virtual ~TreeNodeBase() = default;
}; // class ITreeNode

template <class T>
concept TreeNode = std::derived_from<T, TreeNodeBase>;

template <class T, class C>
concept CanLinkChild = requires(T Node) {
  typename T::ChildTy;
  Node.linkChild(std::declval<typename T::ChildTy>());
}
&&std::same_as<typename T::ChildTy, C>;

template <class T, class C>
concept CanUnlinkChild = requires(T Node) {
  typename T::ChildTy;
  Node.unlinkChild();
}
&&std::same_as<typename T::ChildTy, C>;

template <class T, class C>
concept CanLinkLeftChild = requires(T Node) {
  typename T::LeftChildTy;
  Node.linkLeft(std::declval<typename T::LeftChildTy>());
}
&&std::same_as<typename T::LeftChildTy, C>;

template <class T, class C>
concept CanLinkCenterChild = requires(T Node) {
  typename T::CenterChildTy;
  Node.linkCenter(std::declval<typename T::CenterChildTy>());
}
&&std::same_as<typename T::CenterChildTy, C>;

template <class T, class C>
concept CanLinkRightChild = requires(T Node) {
  typename T::RightChildTy;
  Node.linkRight(std::declval<typename T::RightChildTy>());
}
&&std::same_as<typename T::RightChildTy, C>;

template <class T, class LC, class RC>
concept CanLink2Children = CanLinkLeftChild<T, LC> && CanLinkRightChild<T, RC>;

template <class T, class LC, class CC, class RC>
concept CanLink3Children =
    CanLink2Children<T, LC, RC> && CanLinkCenterChild<T, CC>;

template <class T, class P>
concept CanLinkParent = requires(T Node) {
  typename T::ParentTy;
  Node.linkParent(std::declval<typename T::ParentTy>());
}
&&std::same_as<typename T::ParentTy, P>;

template <class T, class P>
concept CanUnlinkParent = requires(T Node) {
  typename T::ParentTy;
  Node.unlinkParent();
}
&&std::same_as<typename T::ParentTy, P>;

template <class P> class TreeNodeWParent : virtual public TreeNodeBase {
public:
  using ParentTy = P;

protected:
  ParentTy *Parent_ = nullptr;

  TreeNodeWParent() = default;
  TreeNodeWParent(ParentTy *Parent) : Parent_{Parent} {}

public:
  bool isRoot() const { return static_cast<bool>(Parent_); }

  const TreeNodeWParent &linkParent(ParentTy *Parent) {
    Parent_ = Parent;
    return *this;
  }

  const TreeNodeWParent &linkParent(ParentTy &&Parent) {
    if (Parent_ == nullptr)
      throw std::runtime_error("Can't link parent from rvalue");
    *Parent_ = std::move(Parent);
    return *this;
  }

  const TreeNodeWParent &unlinkParent() {
    Parent_ = nullptr;
    return *this;
  }

  virtual ~TreeNodeWParent() = default;
}; // class TreeNodeWParent

static_assert(TreeNode<TreeNodeWParent<int>>,
              "TreeNodeWParent expected to be TreeNode");
static_assert(CanLinkParent<TreeNodeWParent<int>, int>,
              "TreeNodeWParent must can link parent");
static_assert(CanUnlinkParent<TreeNodeWParent<int>, int>,
              "TreeNodeWParent must can unlink parent");

template <class C> class TreeNodeWChild : virtual public TreeNodeBase {
public:
  using ChildTy = C;

protected:
  std::unique_ptr<ChildTy> Child_;

  TreeNodeWChild(ChildTy *Child = nullptr) : Child_{Child} {}

public:
  TreeNodeWChild &linkChild(ChildTy &&Child) {
    *Child_ = std::move(Child);
    if constexpr (CanLinkParent<ChildTy, decltype(*this)>) {
      if (Child_)
        Child_->linkParent(this);
    }
    return *this;
  }

  TreeNodeWChild &unlinkChild() {
    Child_.reset(nullptr);
    return *this;
  }

  virtual ~TreeNodeWChild() = default;
}; // Class TreeNodeWChild

static_assert(TreeNode<TreeNodeWChild<int>>,
              "TreeNodeWChild expected to be TreeNode");
static_assert(CanLinkChild<TreeNodeWChild<int>, int>,
              "TreeNodeWChild must can link child");
static_assert(CanUnlinkChild<TreeNodeWChild<int>, int>,
              "TreeNodeWChild must can unlink child");

template <class P, class C>
class TreeNodeWParentAndChild : virtual public TreeNodeWParent<P>,
                                public TreeNodeWChild<C> {
public:
  using ParentTy = P;
  using ChildTy = C;

protected:
  using TreeNodeWParent<ParentTy>::Parent_;
  using TreeNodeWChild<ChildTy>::Child_;

  TreeNodeWParentAndChild() = default;

  TreeNodeWParentAndChild(ParentTy *Parent)
      : TreeNodeWParent<P>{Parent}, TreeNodeWChild<C>{} {}

  TreeNodeWParentAndChild(ChildTy *Child)
      : TreeNodeWParent<P>{}, TreeNodeWChild<C>{Child} {}

  TreeNodeWParentAndChild(ParentTy *Parent, ChildTy *Child)
      : TreeNodeWParent<P>{Parent}, TreeNodeWChild<C>{Child} {}

public:
  using TreeNodeWParent<ParentTy>::isRoot;
  using TreeNodeWParent<ParentTy>::linkParent;
  using TreeNodeWParent<ParentTy>::unlinkParent;
  using TreeNodeWChild<ChildTy>::linkChild;
  using TreeNodeWChild<ChildTy>::unlinkChild;

  virtual ~TreeNodeWParentAndChild() = default;
}; // class TreeNodeWParentAndChild

static_assert(TreeNode<TreeNodeWParentAndChild<float, int>>,
              "TreeNodeWParent expected to be TreeNode");
static_assert(CanLinkParent<TreeNodeWParentAndChild<float, int>, float>,
              "TreeNodeWParentAndChild must can link parent");
static_assert(CanUnlinkParent<TreeNodeWParentAndChild<float, int>, float>,
              "TreeNodeWParentAndChild must can unlink parent");
static_assert(CanLinkChild<TreeNodeWParentAndChild<float, int>, int>,
              "TreeNodeWParentAndChild must can link child");
static_assert(CanUnlinkChild<TreeNodeWParentAndChild<float, int>, int>,
              "TreeNodeWParentAndChild must can unlink child");

template <typename C1, typename C2>
class TreeNodeW2Children : public virtual TreeNodeBase {
public:
  using LeftChildTy = C1;
  using RightChildTy = C2;

protected:
  std::unique_ptr<LeftChildTy> Left_;
  std::unique_ptr<RightChildTy> Right_;

  TreeNodeW2Children(LeftChildTy *Left, RightChildTy *Right)
      : Left_{Left}, Right_{Right} {}

public:
  TreeNodeW2Children &linkLeft(LeftChildTy &&Child) {
    *Left_ = std::move(Child);
    if constexpr (CanLinkParent<LeftChildTy, decltype(*this)>) {
      if (Left_)
        Left_->linkParent(this);
    }
    return this;
  }

  TreeNodeW2Children &linkRight(RightChildTy &&Child) {
    *Right_ = std::move(Child);
    if constexpr (CanLinkParent<RightChildTy, decltype(*this)>) {
      if (Right_)
        Right_->linkParent(this);
    }
    return this;
  }

  TreeNodeW2Children &linkChild(LeftChildTy &&Child) {
    return linkLeft(std::move(Child));
  }

  TreeNodeW2Children &linkChild(RightChildTy &&Child) {
    return linkRight(std::move(Child));
  }

  TreeNodeW2Children &unlinkLeft() {
    Left_.reset(nullptr);
    return *this;
  }

  TreeNodeW2Children &unlinkRight() {
    Right_.reset(nullptr);
    return *this;
  }

  virtual ~TreeNodeW2Children() = default;
}; // class TreeNodeW2Children

static_assert(TreeNode<TreeNodeW2Children<float, int>>,
              "TreeNodeWParent expected to be TreeNode");
static_assert(CanLinkLeftChild<TreeNodeW2Children<float, int>, float>,
              "TreeNodeWParentAndChild must can link left child");
static_assert(CanLinkRightChild<TreeNodeW2Children<float, int>, int>,
              "TreeNodeWParentAndChild must can link right child");
static_assert(CanLink2Children<TreeNodeW2Children<float, int>, float, int>,
              "TreeNodeWParentAndChild must can link 2 children");

template <typename C>
class TreeNodeW2Children<C, C> : public virtual TreeNodeBase {
public:
  using LeftChildTy = C;
  using RightChildTy = C;

protected:
  std::unique_ptr<LeftChildTy> Left_;
  std::unique_ptr<RightChildTy> Right_;

  TreeNodeW2Children(LeftChildTy *Left, RightChildTy *Right)
      : Left_{Left}, Right_{Right} {}

public:
  TreeNodeW2Children &linkLeft(LeftChildTy &&Child) {
    *Left_ = std::move(Child);
    if constexpr (CanLinkParent<LeftChildTy, decltype(*this)>) {
      if (Left_)
        Left_->linkParent(this);
    }
    return this;
  }

  TreeNodeW2Children &linkRight(RightChildTy &&Child) {
    *Right_ = std::move(Child);
    if constexpr (CanLinkParent<RightChildTy, decltype(*this)>) {
      if (Right_)
        Right_->linkParent(this);
    }
    return this;
  }

  TreeNodeW2Children &unlinkLeft() {
    Left_.reset(nullptr);
    return *this;
  }

  TreeNodeW2Children &unlinkRight() {
    Right_.reset(nullptr);
    return *this;
  }

  virtual ~TreeNodeW2Children() = default;
}; // class TreeNodeW2Children

static_assert(TreeNode<TreeNodeW2Children<int, int>>,
              "TreeNodeWParent expected to be TreeNode");
static_assert(CanLinkLeftChild<TreeNodeW2Children<int, int>, int>,
              "TreeNodeWParentAndChild must can link left child");
static_assert(CanLinkRightChild<TreeNodeW2Children<int, int>, int>,
              "TreeNodeWParentAndChild must can link right child");
static_assert(CanLink2Children<TreeNodeW2Children<int, int>, int, int>,
              "TreeNodeWParentAndChild must can link 2 children");

template <typename P, typename C1, typename C2>
class TreeNodeWParentAnd2Children : public TreeNodeWParent<P>,
                                    public TreeNodeW2Children<C1, C2> {
public:
  using ParentTy = P;
  using LeftChildTy = C1;
  using RightChildTy = C2;

protected:
  using TreeNodeWParent<ParentTy>::Parent_;
  using TreeNodeW2Children<LeftChildTy, RightChildTy>::Left_;
  using TreeNodeW2Children<LeftChildTy, RightChildTy>::Right_;

  TreeNodeWParentAnd2Children(LeftChildTy *Left, RightChildTy *Right)
      : TreeNodeWParent<ParentTy>{},
        TreeNodeW2Children<LeftChildTy, RightChildTy>{Left, Right} {
    if constexpr (CanLinkParent<LeftChildTy, decltype(*this)>) {
      if (Left_)
        Left_->linkParent(this);
    }

    if constexpr (CanLinkParent<RightChildTy, decltype(*this)>) {
      if (Right_)
        Right_->linkParent(this);
    }
  }

  TreeNodeWParentAnd2Children(ParentTy *Parent, LeftChildTy *Left,
                              RightChildTy *Right)
      : TreeNodeWParent<ParentTy>{Parent},
        TreeNodeW2Children<LeftChildTy, RightChildTy>{Left, Right} {
    if constexpr (CanLinkParent<LeftChildTy, decltype(*this)>) {
      if (Left_)
        Left_->linkParent(this);
    }

    if constexpr (CanLinkParent<RightChildTy, decltype(*this)>) {
      if (Right_)
        Right_->linkParent(this);
    }
  }

public:
  using TreeNodeWParent<ParentTy>::isRoot;
  using TreeNodeWParent<ParentTy>::linkParent;
  using TreeNodeWParent<ParentTy>::unlinkParent;

  using TreeNodeW2Children<LeftChildTy, RightChildTy>::linkLeft;
  using TreeNodeW2Children<LeftChildTy, RightChildTy>::unlinkLeft;
  using TreeNodeW2Children<LeftChildTy, RightChildTy>::linkRight;
  using TreeNodeW2Children<LeftChildTy, RightChildTy>::unlinkRight;

  virtual ~TreeNodeWParentAnd2Children() = default;
}; // class TreeNodeWParentAnd2Children

static_assert(TreeNode<TreeNodeWParentAnd2Children<int, float, int>>,
              "TreeNodeWParent expected to be TreeNode");
static_assert(
    CanLinkLeftChild<TreeNodeWParentAnd2Children<int, float, int>, float>,
    "TreeNodeWParentAnd2Children must can link left child");
static_assert(
    CanLinkRightChild<TreeNodeWParentAnd2Children<int, float, int>, int>,
    "TreeNodeWParentAnd2Children must can link right child");
static_assert(
    CanLink2Children<TreeNodeWParentAnd2Children<int, float, int>, float, int>,
    "TreeNodeWParentAnd2Children must can link ParentAnd2 children");

template <typename C1, typename C2, typename C3>
class TreeNodeW3Children : public TreeNodeW2Children<C1, C3> {
public:
  using LeftChildTy = C1;
  using CenterChildTy = C2;
  using RightChildTy = C3;

protected:
  using TreeNodeW2Children<LeftChildTy, RightChildTy>::Left_;
  std::unique_ptr<CenterChildTy> Center_;
  using TreeNodeW2Children<LeftChildTy, RightChildTy>::Right_;

  TreeNodeW3Children(LeftChildTy *Left, CenterChildTy *Center,
                     RightChildTy *Right)
      : TreeNodeW2Children<LeftChildTy, RightChildTy>{Left, Right},
        Center_{Center} {}

public:
  using TreeNodeW2Children<LeftChildTy, RightChildTy>::linkLeft;
  using TreeNodeW2Children<LeftChildTy, RightChildTy>::unlinkLeft;
  using TreeNodeW2Children<LeftChildTy, RightChildTy>::linkRight;
  using TreeNodeW2Children<LeftChildTy, RightChildTy>::unlinkRight;

  TreeNodeW3Children &linkCenter(CenterChildTy &&child) {
    *Center_ = std::move(child);
    if constexpr (CanLinkParent<CenterChildTy, decltype(*this)>) {
      if (Center_)
        Center_->linkParent(this);
    }
    return this;
  }

  TreeNodeW3Children &unlinkCenter() {
    *Center_.reset(nullptr);
    return this;
  }

  virtual ~TreeNodeW3Children() = default;
}; // class ITreeNodeW3Ch

template <typename P, typename C1, typename C2, typename C3>
class TreeNodeWParentAnd3Children : public TreeNodeWParent<P>,
                                    public TreeNodeW3Children<C1, C2, C3> {
public:
  using ParentTy = P;
  using LeftChildTy = C1;
  using CenterChildTy = C2;
  using RightChildTy = C3;

protected:
  using TreeNodeWParent<ParentTy>::Parent_;
  using TreeNodeW3Children<LeftChildTy, CenterChildTy, RightChildTy>::Left_;
  using TreeNodeW3Children<LeftChildTy, CenterChildTy, RightChildTy>::Center_;
  using TreeNodeW3Children<LeftChildTy, CenterChildTy, RightChildTy>::Right_;

  TreeNodeWParentAnd3Children(ParentTy *p, LeftChildTy *l = nullptr,
                              CenterChildTy *c = nullptr,
                              RightChildTy *r = nullptr)
      : TreeNodeWParent<ParentTy>{p},
        TreeNodeW3Children<LeftChildTy, CenterChildTy, RightChildTy>{l, c, r} {
    if constexpr (CanLinkParent<LeftChildTy, decltype(*this)>) {
      if (Left_)
        Left_->linkParent(this);
    }

    if constexpr (CanLinkParent<CenterChildTy, decltype(*this)>) {
      if (Center_)
        Center_->linkParent(this);
    }

    if constexpr (CanLinkParent<RightChildTy, decltype(*this)>) {
      if (Right_)
        Right_->linkParent(this);
    }
  }

public:
  using TreeNodeWParent<ParentTy>::isRoot;
  using TreeNodeWParent<ParentTy>::linkParent;
  using TreeNodeWParent<ParentTy>::unlinkParent;

  using TreeNodeW3Children<LeftChildTy, CenterChildTy, RightChildTy>::linkLeft;
  using TreeNodeW3Children<LeftChildTy, CenterChildTy,
                           RightChildTy>::unlinkLeft;
  using TreeNodeW3Children<LeftChildTy, CenterChildTy,
                           RightChildTy>::linkCenter;
  using TreeNodeW3Children<LeftChildTy, CenterChildTy,
                           RightChildTy>::unlinkCenter;
  using TreeNodeW3Children<LeftChildTy, CenterChildTy, RightChildTy>::linkRight;
  using TreeNodeW3Children<LeftChildTy, CenterChildTy,
                           RightChildTy>::unlinkRight;

  virtual ~TreeNodeWParentAnd3Children() = default;
}; // class TreeNodeWParentAnd3Children

template <typename C>
class TreeNodeWManyChildren : virtual public TreeNodeBase {
public:
  using ChildTy = C;

protected:
  using ListTy = std::list<ChildTy *>;
  ListTy Children_;

  TreeNodeWManyChildren() = default;

  TreeNodeWManyChildren(ChildTy *Children) : Children_(1, Children) {
#ifdef NOW_RAW_LOOPS
    for (auto *Child : Children_) {
      if (Child) {
        if constexpr (CanLinkParent<ChildTy, decltype(*this)>)
          Child->linkParent(this);
      } else {
        Children_.erase(Child);
      }
    }
#else
    for (auto It = Children_.begin(), End = Children_.end(); It != End; ++It) {
      if (*It) {
        if constexpr (CanLinkParent<ChildTy, decltype(*this)>)
          It->linkParent(this);
      } else {
        Children_.erase(It);
      }
    }
#endif
  }

public:
  auto begin() { return Children_.begin(); }
  auto end() { return Children_.end(); }
  auto begin() const { return Children_.begin(); }
  auto end() const { return Children_.end(); }

  TreeNodeWManyChildren &linkChild(ChildTy &&Child) {
    Children_.emplace_back(std::make_unique<ChildTy>(std::move(Child)));
    return *this;
  }

  TreeNodeWManyChildren &unlinkChild(const ChildTy &Child) {
    Children_.remove(Child);
    return *this;
  }

  TreeNodeWManyChildren &unlinkChild(typename ListTy::iterator Pos) {
    Children_.erase(Pos);
    return *this;
  }

  virtual ~TreeNodeWManyChildren() = default;
}; // class TreeNodeWManyChildren

template <typename P, typename C>
class TreeNodeWParentAndManyChildren : public TreeNodeWParent<P>,
                                       public TreeNodeWManyChildren<C> {
public:
  using ParentTy = P;
  using ChildTy = C;

protected:
  using TreeNodeWParent<ParentTy>::Parent_;
  using TreeNodeWManyChildren<ChildTy>::Children_;

  TreeNodeWParentAndManyChildren() = default;

  TreeNodeWParentAndManyChildren(ParentTy *Parent)
      : TreeNodeWParent<ParentTy>{Parent}, TreeNodeWManyChildren<ChildTy>{} {}

  TreeNodeWParentAndManyChildren(ChildTy *Child)
      : TreeNodeWParent<ParentTy>{}, TreeNodeWManyChildren<ChildTy>{Child} {}

  TreeNodeWParentAndManyChildren(ParentTy *Parent, ChildTy *Child)
      : TreeNodeWParent<ParentTy>{Parent}, TreeNodeWManyChildren<ChildTy>{
                                               Child} {}

public:
  using TreeNodeWParent<ParentTy>::isRoot;
  using TreeNodeWParent<ParentTy>::linkParent;
  using TreeNodeWParent<ParentTy>::unlinkParent;

  using TreeNodeWManyChildren<ChildTy>::begin; // Does this use both const and
                                               // non-const?
  using TreeNodeWManyChildren<ChildTy>::end;   //

  using TreeNodeWManyChildren<ChildTy>::linkChild;
  using TreeNodeWManyChildren<ChildTy>::unlinkChild; // Does this use all
                                                     // overloads?

  virtual ~TreeNodeWParentAndManyChildren() = default;
}; // class TreeNodeWParentAndManyChildren

#if 0
template <typename T> class ListNode final : public INode {
  std::unique_ptr<ListNode> Prev_;
  std::unique_ptr<ListNode> Next_;
  T Data_;

  ListNode(ListNode *Prev, ListNode *Next)
      : Prev_{Prev}, Next_{Next}, Data_{} {}

  template <typename... ArgTypes>
  ListNode(ListNode *Prev, ListNode *Next, ArgTypes &&...Args)
      : Prev_{Prev}, Next_{Next}, Data_{std::forward(Args)...} {}

public:
  ~ListNode() noexcept {
    if (Prev_ != nullptr)
      Prev_->Next_ = nullptr;
  }

  static auto getNewList() {
    return std::make_unique(ListNode(nullptr, nullptr));
  }

  template <typename... ArgTypes> static auto getNewList(ArgTypes &&...Args) {
    return std::make_unique<ListNode>(nullptr, nullptr, std::forward(Args)...);
  }

  [[nodiscard]] ListNode *prev() const noexcept { return Prev_; }
  [[nodiscard]] ListNode *next() const noexcept { return Next_; }

  ListNode &linkPrev(ListNode *ToLink) noexcept {
    Prev_.reset(ToLink);
    return *this;
  }

  ListNode &linkNext(ListNode *ToLink) noexcept {
    Next_.reset(ToLink);
    return *this;
  }

  [[nodiscard]] T &operator*() const noexcept { return Data_; }
  [[nodiscard]] T *operator->() const noexcept { return &Data_; }
}; // class List
#endif

} // namespace wsheeet::ast
