//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <vector>
// vector<bool>

// void reserve(size_type n);

#include <vector>
#include <cassert>
#include <stdexcept>

#include "test_macros.h"
#include "min_allocator.h"
#include "test_allocator.h"

TEST_CONSTEXPR_CXX20 bool tests() {
  {
    std::vector<bool> v;
    v.reserve(10);
    assert(v.capacity() >= 10);
  }
  {
    std::vector<bool> v(100);
    assert(v.capacity() >= 100);
    v.reserve(50);
    assert(v.size() == 100);
    assert(v.capacity() >= 100);
    v.reserve(150);
    assert(v.size() == 100);
    assert(v.capacity() >= 150);
  }
#if TEST_STD_VER >= 11
  {
    std::vector<bool, min_allocator<bool>> v;
    v.reserve(10);
    assert(v.capacity() >= 10);
  }
  {
    std::vector<bool, explicit_allocator<bool>> v;
    v.reserve(10);
    assert(v.capacity() >= 10);
  }
  {
    std::vector<bool, min_allocator<bool>> v(100);
    assert(v.capacity() >= 100);
    v.reserve(50);
    assert(v.size() == 100);
    assert(v.capacity() >= 100);
    v.reserve(150);
    assert(v.size() == 100);
    assert(v.capacity() >= 150);
  }
#endif
#ifndef TEST_HAS_NO_EXCEPTIONS
  if (!TEST_IS_CONSTANT_EVALUATED) {
    std::vector<bool, limited_allocator<bool, 10> > v;
    v.reserve(5);
    try {
      // A typical implementation would allocate chunks of bits.
      // In libc++ the chunk has the same size as the machine word. It is
      // reasonable to assume that in practice no implementation would use
      // 64 kB or larger chunks.
      v.reserve(10 * 65536);
      assert(false);
    } catch (const std::length_error&) {
      // no-op
    }
    assert(v.capacity() >= 5);
  }
#endif

  return true;
}

int main(int, char**) {
  tests();
#if TEST_STD_VER > 17
  static_assert(tests());
#endif
  return 0;
}
