//
//  VROOpenGL.h
//  ViroRenderer
//
//  Created by Raj Advani on 11/2/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#ifndef VROOpenGL_h
#define VROOpenGL_h

#include "VRODefines.h"

#if VRO_PLATFORM_ANDROID

#include <EGL/egl.h>
#include <GLES2/gl2ext.h>
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include <GLES3/gl3platform.h>

#elif VRO_PLATFORM_IOS

#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES1/glext.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES3/glext.h>

#endif

#endif /* VROOpenGL_h */
