/**
 * @file linked_list.hpp
 * @author CrackLewis (ghxx040406@163.com)
 * @brief
 * @version 0.1
 * @date 2023-03-18
 *
 * @copyright Copyright (c) 2023
 *
 * This file defines the linked list data structure.
 *
 * Be clear that this structure might not be suitable for special cases.
 */

#ifndef LINKED_LIST_HPP_
#define LINKED_LIST_HPP_

#include <cassert>
#include <cstddef>
#include <deque>

template <class Ty>
class LinkedList {
  // definitions
 public:
  struct ListNode {
    ListNode *prior_, *next_;
    Ty value_;
  };

  // constructors & destructor
 public:
  LinkedList() {
    head_ = tail_ = nullptr;
    size_ = 0;
  }

  LinkedList(const LinkedList& oth) {
    copy_period(head_, tail_, oth.head_, oth.tail_);
    size_ = oth.size_;
  }

  LinkedList(LinkedList&& oth) {
    head_ = oth.head_, oth.head_ = nullptr;
    tail_ = oth.tail_, oth.tail_ = nullptr;
    size_ = oth.size_, oth.size_ = 0;
  }

  ~LinkedList() {
    release_period(head_, tail_);
    head_ = tail_ = nullptr;
    size_ = 0;
  }

  // public method
 public:
  int size() const { return size_; }

  bool empty() const { return (size_ == 0u); }

  ListNode* first() const { return first_; }

  ListNode* last() const { return last_; }

  ListNode* at(size_t idx) const {
    ListNode* ret = first_;
    while (idx--) ret = ret->next_;
    return ret;
  }

  void push_back(const Ty& val) {
    ListNode* node = alloc(val);
    if (head_ == nullptr) {
      head_ = tail_ = node;
      size_ = 1;
    } else {
      tail_->next_ = node;
      node->prior_ = tail_;
      tail_ = node;
      size_++;
    }
  }

  void pop_back() {
    assert((tail_ != nullptr) && "pop_back on an empty list");

    if (head_ == tail_) {
      dealloc(head_);
      head_ = tail_ = nullptr;
      size_ = 0;
    } else {
      tail_ = tail_->prior_;
      dealloc(tail_->next_);
      tail_->next_ = nullptr;
      size_--;
    }
  }

  void push_front(const Ty& val) {
    ListNode* node = alloc(val);
    if (head_ == nullptr) {
      head_ = tail_ = node;
      size_ = 1;
    } else {
      head_->prior_ = node;
      node->next_ = head_;
      head_ = node;
      size_++;
    }
  }

  void pop_front() {
    assert((head_ != nullptr) && "pop_back on an empty list");

    if (head_ == tail_) {
      dealloc(tail_);
      head_ = tail_ = nullptr;
      size_ = 0;
    } else {
      head_ = head_->next_;
      dealloc(head_->prior_);
      head_->prior_ = nullptr;
      size_--;
    }
  }

  Ty front() const { return head_->value_; }
  Ty back() const { return tail_->value_; }

  void foo() {
    std::deque<int> Q;
    
  }
  // private method
 private:
  ListNode* alloc(const Ty& value) {
    ListNode* ret = new ListNode;
    ret->value_ = value;
    ret->prior_ = ret->next_ = nullptr;
    return ret;
  }

  void dealloc(ListNode* node) {
    if (node) delete node;
  }

  /**
   * @brief
   *
   * form a copy of the specified portion of linked list.
   *
   * @param rhead the head node of copy
   * @param rtail the tail node of copy
   * @param chead the head node of copied portion
   * @param ctail the tail node of copied portion
   */
  void copy_period(ListNode*& rhead, ListNode*& rtail, ListNode* chead,
                   ListNode* ctail) {
    if (chead == nullptr || ctail == nullptr)
      assert(0 && "chead or ctail is nullptr");

    ListNode *ctemp = chead, *rtemp = alloc(chead->value_);
    while (1) {
      if (ctemp == ctail) break;

      if (ctemp->next_ != nullptr) {
        rtemp->next_ = alloc(ctemp->next_->value_);
        rtemp->next_->prior_ = rtemp;
        rtemp = rtemp->next_;
        ctemp = ctemp->next_;
      } else {
        assert(0 && "chead and ctail are not in the same linklist");
      }
    }
  }

  /**
   * @brief
   *
   * release a specified section of linked list and splice two broken ends
   *
   * @param head the head node of list section
   * @param tail the tail node of list section
   */
  void release_period(ListNode* head, ListNode* tail) {
    if (head == nullptr || tail == nullptr)
      assert(0 && "head or tail is nullptr");

    ListNode *cur = head, *tmp;
    while (1) {
      if (cur == nullptr) {
        assert(0 && "head and tail are not in the same linklist");
      }
      if (cur->next_) cur->next_->prior_ = cur->prior_;
      if (cur->prior_) cur->prior_->next_ = cur->next_;

      tmp = cur->next_;
      dealloc(cur);
      if (cur == tail) break;
      cur = tmp;
    }
  }

  // members
 private:
  ListNode *head_, *tail_;
  size_t size_;
};

#endif