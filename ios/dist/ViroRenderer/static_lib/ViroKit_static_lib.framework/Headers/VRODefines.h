//
//  VRODefines.h
//  ViroRenderer
//
//  Created by Raj Advani on 11/1/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#ifndef VRODefines_h
#define VRODefines_h

#ifdef __OBJC__
#import "TargetConditionals.h" 
#if TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE
#define VRO_PLATFORM_ANDROID 0
#define VRO_PLATFORM_IOS 1
#define VRO_PLATFORM_WASM 0
#define VRO_PLATFORM_MACOS 0
#else
#define VRO_PLATFORM_ANDROID 0
#define VRO_PLATFORM_IOS 0
#define VRO_PLATFORM_WASM 0
#define VRO_PLATFORM_MACOS 1
#endif // __OBJC__
#else
#ifdef WASM_PLATFORM
#define VRO_PLATFORM_ANDROID 0
#define VRO_PLATFORM_IOS 0
#define VRO_PLATFORM_WASM 1
#define VRO_PLATFORM_MACOS 0

#define VRO_C_INCLUDE "VROWasmCAPI.h"

#else  // !WASM_PLATFORM
#define VRO_PLATFORM_ANDROID 1
#define VRO_PLATFORM_IOS 0
#define VRO_PLATFORM_WASM 0
#define VRO_PLATFORM_MACOS 0

#define VRO_C_INCLUDE "VROAndroidCAPI.h"

#endif
#endif // !__OBJC __

#define VRO_METAL 0

// True if building for Posemoji
#define VRO_POSEMOJI 1

#endif /* VRODefines_h */
