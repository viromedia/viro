//
//  VROOpenGL.h
//  ViroRenderer
//
//  Created by Raj Advani on 11/2/16.
//  Copyright © 2016 Viro Media. All rights reserved.
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

#ifndef VROOpenGL_h
#define VROOpenGL_h

#include "VRODefines.h"
#include "VROLog.h"

#if VRO_PLATFORM_ANDROID

#include <cstring>
#include <stdlib.h>
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <GLES2/gl2ext.h>
#include <GLES3/gl3ext.h>
#include <GLES3/gl3platform.h>

// Avoiding glBufferSubData seems to increase stability on Adreno devices
#define VRO_AVOID_BUFFER_SUB_DATA 1

#define pglpush(message,...) ((void)0)
#define pglpop() ((void)0)

#elif VRO_PLATFORM_IOS

#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES1/glext.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES3/glext.h>
#define VRO_AVOID_BUFFER_SUB_DATA 0

#define pglpush(message,...) \
do { \
char str[1024]; \
sprintf(str, #message, ##__VA_ARGS__); \
glPushGroupMarkerEXT(0, str); \
} while (0)

#define pglpop() \
do { \
glPopGroupMarkerEXT(); \
} while (0)

#elif VRO_PLATFORM_MACOS

#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#define GL_INVALID_INDEX 0xFFFFFFFFu
#define GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC 0xdecafbad
#define GL_COMPRESSED_RGBA_ASTC_4x4_KHR 0xdecafbad
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_4x4_KHR 0xdecafbad
#define GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS 0xdecafbad

#define pglpush(message,...) \
do { \
char str[1024]; \
sprintf(str, #message, ##__VA_ARGS__); \
glPushGroupMarkerEXT(0, str); \
} while (0)

#define pglpop() \
do { \
glPopGroupMarkerEXT(); \
} while (0)

#elif VRO_PLATFORM_WASM

#include <GLES3/gl3.h>
#include <GLES2/gl2ext.h>
#include <GLES3/gl3platform.h>
#define VRO_AVOID_BUFFER_SUB_DATA 0

#define pglpush(message,...) \
do { \
char str[1024]; \
sprintf(str, #message, ##__VA_ARGS__); \
\
} while (0)

#define pglpop() \
do { \
\
} while (0)

#endif

#ifdef CHECK_GL_ERRORS

static const char * GlErrorString( GLenum error )
{
	switch ( error )
	{
		case GL_NO_ERROR:						return "GL_NO_ERROR";
		case GL_INVALID_ENUM:					return "GL_INVALID_ENUM";
		case GL_INVALID_VALUE:					return "GL_INVALID_VALUE";
		case GL_INVALID_OPERATION:				return "GL_INVALID_OPERATION";
		case GL_INVALID_FRAMEBUFFER_OPERATION:	return "GL_INVALID_FRAMEBUFFER_OPERATION";
		case GL_OUT_OF_MEMORY:					return "GL_OUT_OF_MEMORY";
		default: return "unknown";
	}
}

static void GLCheckErrors(const char *file, const char *method, int line )
{
	for ( int i = 0; i < 10; i++ )
	{
		const GLenum error = glGetError();
		if ( error == GL_NO_ERROR )
		{
			break;
		}
        pinfo( "GL error [file: %s, method: %s, line: %d]: %s", file, method, line, GlErrorString( error ) );
	}
}

#define GL( func )		func; GLCheckErrors( __FILE__, __func__, __LINE__ );

#else // CHECK_GL_ERRORS

#define GL( func )		func;

#endif // CHECK_GL_ERRORS

#endif /* VROOpenGL_h */
