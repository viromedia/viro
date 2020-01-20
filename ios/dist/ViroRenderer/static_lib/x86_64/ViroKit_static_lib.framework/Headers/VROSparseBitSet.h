//
//  VROSparseBitSet.h
//  ViroKit
//
//  Created by Raj Advani on 3/21/18.
//  Copyright © 2018 Viro Media. All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining
//  a copy of this software and associated documentation files (the
//  "Software"), to deal in the Software without restriction, including
//  without limitation the rights to use, copy, modify, merge, publish,
//  distribute, sublicense, and/or sell copies of the Software, and to
//  permit persons to whom the Software is furnished to do so, subject to
//  the following conditions:
//
//  The above copyright notice and this permission notice shall be included
//  in all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

// Modified from SparseBitSet.h in Android Miniken project

/*
 * Copyright (C) 2012 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef VROSparseBitSet_h
#define VROSparseBitSet_h

#include <stdint.h>
#include <sys/types.h>
#include <memory>

// ---------------------------------------------------------------------------

// This is an implementation of a set of integers. It is optimized for
// values that are somewhat sparse, in the ballpark of a maximum value
// of thousands to millions. It is particularly efficient when there are
// large gaps. The motivating example is Unicode coverage of a font, but
// the abstraction itself is fully general.
class VROSparseBitSet {
public:
    // Create an empty bit set.
    VROSparseBitSet() : mMaxVal(0) {}

    // Initialize the set to a new value, represented by ranges. For
    // simplicity, these ranges are arranged as pairs of values,
    // inclusive of start, exclusive of end, laid out in a uint32 array.
    VROSparseBitSet(const uint32_t* ranges, size_t nRanges) : VROSparseBitSet() {
        initFromRanges(ranges, nRanges);
    }

    VROSparseBitSet(VROSparseBitSet&&) = default;
    VROSparseBitSet& operator=(VROSparseBitSet&&) = default;

    // Determine whether the value is included in the set
    bool get(uint32_t ch) const {
        if (ch >= mMaxVal) return false;
        const uint32_t *bitmap = &mBitmaps[mIndices[ch >> kLogValuesPerPage]];
        uint32_t index = ch & kPageMask;
        return (bitmap[index >> kLogBitsPerEl] & (kElFirst >> (index & kElMask))) != 0;
    }

    // One more than the maximum value in the set, or zero if empty
    uint32_t length() const {
        return mMaxVal;
    }

    // The next set bit starting at fromIndex, inclusive, or kNotFound
    // if none exists.
    uint32_t nextSetBit(uint32_t fromIndex) const;

    static const uint32_t kNotFound = ~0u;

private:
    void initFromRanges(const uint32_t* ranges, size_t nRanges);

    static const uint32_t kMaximumCapacity = 0xFFFFFF;
    static const int kLogValuesPerPage = 8;
    static const int kPageMask = (1 << kLogValuesPerPage) - 1;
    static const int kLogBytesPerEl = 2;
    static const int kLogBitsPerEl = kLogBytesPerEl + 3;
    static const int kElMask = (1 << kLogBitsPerEl) - 1;
    // invariant: sizeof(element) == (1 << kLogBytesPerEl)
    typedef uint32_t element;
    static const element kElAllOnes = ~((element)0);
    static const element kElFirst = ((element)1) << kElMask;
    static const uint16_t noZeroPage = 0xFFFF;

    static uint32_t calcNumPages(const uint32_t* ranges, size_t nRanges);
    static int CountLeadingZeros(element x);

    uint32_t mMaxVal;

    std::unique_ptr<uint16_t[]> mIndices;
    std::unique_ptr<element[]> mBitmaps;
    uint16_t mZeroPageIndex;

    // Forbid copy and assign.
    VROSparseBitSet(const VROSparseBitSet&) = delete;
    void operator=(const VROSparseBitSet&) = delete;
};

#endif // VROSparseBitSet
