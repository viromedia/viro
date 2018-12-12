//
//  VROAtomic.h
//  ViroKit
//
//  Created by Raj Advani on 8/13/18.
//  Copyright © 2018 Viro Media. All rights reserved.
//

#ifndef VROAtomic_h
#define VROAtomic_h

#include "VRODefines.h"

#if VRO_PLATFORM_IOS || VRO_PLATFORM_MACOS || VRO_PLATFORM_ANDROID
#include <atomic>

template <typename T>
using VROAtomic = std::atomic<T>;

#else

// WebAssembly does not support atomic, and is single-threaded
template <typename T>
using VROAtomic = T;

#endif

#endif /* VROAtomic_h */
