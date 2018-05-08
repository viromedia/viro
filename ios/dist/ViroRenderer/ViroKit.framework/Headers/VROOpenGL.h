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

#elif VRO_PLATFORM_IOS

#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES1/glext.h>
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES3/glext.h>
#define VRO_AVOID_BUFFER_SUB_DATA 0

#elif VRO_PLATFORM_MACOS

#include <OpenGL/gl3.h>
#include <OpenGL/gl3ext.h>
#define GL_INVALID_INDEX 0xFFFFFFFFu
#define GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC 0xdecafbad
#define GL_COMPRESSED_RGBA_ASTC_4x4_KHR 0xdecafbad
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_4x4_KHR 0xdecafbad
#define GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS 0xdecafbad

#elif VRO_PLATFORM_WASM

#include <GLES3/gl3.h>
#include <GLES2/gl2ext.h>
#include <GLES3/gl3platform.h>
#define VRO_AVOID_BUFFER_SUB_DATA 0

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

static void GLCheckErrors( int line )
{
	for ( int i = 0; i < 10; i++ )
	{
		const GLenum error = glGetError();
		if ( error == GL_NO_ERROR )
		{
			break;
		}
		pinfo( "GL error on line %d: %s", line, GlErrorString( error ) );
	}
}

#define GL( func )		func; GLCheckErrors( __LINE__ );

#else // CHECK_GL_ERRORS

#define GL( func )		func;

#endif // CHECK_GL_ERRORS

#endif /* VROOpenGL_h */
