/**
 * @file linear_list.hpp
 * @author CrackLewis (ghxx040406@163.com)
 * @brief
 * @version 0.1
 * @date 2023-03-27
 *
 * @copyright Copyright (c) 2023
 *
 * This file defines the linear list structure.
 *
 */

#ifndef LINEAR_LIST_HPP_
#define LINEAR_LIST_HPP_

#include <cassert>
#include <cstddef>
#include <cstring>
#include <new>

namespace LinearListPolicy {
const static size_t LIST_INIT_SIZE = 10;
const static size_t LIST_INCREMENT_INTERCEPT = 0;
const static double LIST_INCREMENT_COEFFICIENT = 1.5;

static size_t calc_new_size(size_t oldsize) {
  if (oldsize == 0) return LIST_INIT_SIZE;
  return std::max(oldsize,
                  (size_t)(oldsize * LIST_INCREMENT_COEFFICIENT + 0.5) +
                      LIST_INCREMENT_INTERCEPT);
}
};  // namespace LinearListPolicy

template <typename Ty>
class LinearList {
 public:
  LinearList() : content_(nullptr), len_(0), size_(0) {}

  LinearList(const Ty* oth, size_t len) {
    if (!len)
      content_ = nullptr, len_ = size_ = 0;
    else {
      content_ = alloc(len);
      assert((content_ != nullptr) && "bad linear-list copy construction");
      memcpy(content_, oth, sizeof(Ty) * (size_ = len_ = len));
    }
  }

  LinearList(const LinearList& oth) : LinearList(oth.content_, oth.len_) {}

  LinearList(LinearList&& old) {
    content_ = old.content_, old.content_ = nullptr;
    len_ = old.len_, old.len_ = 0;
    size_ = old.size_, old.size_ = 0;
  }

  ~LinearList() {
    dealloc(content_);
    content_ = nullptr;
    size_ = len_ = 0;
  }

 public:
  bool empty() const { return (len_ == 0); }

  size_t size() const { return len_; }

  void push_back(const Ty& val) {
    *get_next() = val;
    len_++;
  }

  void pop_back() {
    assert((len_ > 0) && "pop back on an empty linear-list");
    len_--;
  }

  Ty front() const { return content_[0]; }

  Ty back() const { return content_[len_ - 1]; }

  Ty* first() const { return content_; }

  Ty* last() const { return content_ + (len_ - 1); }

  const Ty& at(size_t idx) const { return content_[idx]; }

  Ty& operator[](size_t idx) { return content_[idx]; }

  void resize(size_t idx) { __resize(idx); }

 private:
  Ty* alloc(size_t size) { return ::operator new(sizeof(Ty) * size); }

  void dealloc(Ty* content) {
    if (content) ::operator delete(content);
  }

  Ty* get_next() {
    if (len_ >= size_) expand();
    return (content_ + len_);
  }

  void expand() { __resize(LinearListPolicy::calc_new_size(size_)); }

  void __resize(size_t newsize) {
    if (!newsize) {
      dealloc(content_);
      content_ = nullptr, size_ = 0u, len_ = 0u;
      return;
    }
    Ty* new_content = alloc(size_ = newsize);
    assert((new_content != nullptr) && "linear-list expansion failed");
    if (content_)
      memcpy(new_content, content_,
             sizeof(Ty) * (len_ = std::min(len_, size_)));
    dealloc(content_);
    content_ = new_content;
  }

 private:
  Ty* content_;
  size_t len_, size_;
};

#endif