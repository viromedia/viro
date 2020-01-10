//
//  VROPlatformUtil.h
//  ViroRenderer
//
//  Created by Raj Advani on 11/7/16.
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

#ifndef VROPlatformUtil_h
#define VROPlatformUtil_h

#include "VRODefines.h"
#include "VROTexture.h"
#include "VROLog.h"
#include <string>
#include <memory>
#include <functional>
#include <map>

#if VRO_PLATFORM_ANDROID

#include <jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/bitmap.h>

#endif

class VROImage;
enum class VROTextureInternalFormat;

#pragma mark - Platform Identification

enum class VROPlatformType {
    Unknown,
    AndroidGVR,
    AndroidOVR,
    AndroidARCore,
    AndroidSceneView,
    iOSCardboard,
    iOSARKit,
    iOSSceneView,
};

void VROPlatformSetType(VROPlatformType type);
VROPlatformType VROPlatformGetType();

#pragma mark - String Loading
std::string VROPlatformLoadResourceAsString(std::string resource, std::string type);
std::string VROPlatformLoadFileAsString(std::string path);

#pragma mark - Network and File Utilities

/*
 Copy the given bundled resource (e.g., asset on Android, bundled resource on iOS)
 to a location on the filesystem where it can be accessed through a local path.
 isTemp is set to true if the file had to be copied to a temporary file.
 */
std::string VROPlatformCopyResourceToFile(std::string asset, bool *isTemp);

/*
 This function is used exclusively by Android (iOS is no-op) because it makes a few assumptions:
 1) key needs to have its extension removed
 2) key needs be lower case
 3) key needs sanitizing and have 'prohibited' character(s) removed
 */
std::string VROPlatformFindValueInResourceMap(std::string key, std::map<std::string, std::string> resourceMap);

/*
 Load the given URL to a file in a blocking fashion, and return the path to the file.
 If the file is temporary and must be deleted after it's processed, temp will be set to true.
 If the download fails, success will be set to false.
 */
std::string VROPlatformDownloadURLToFile(std::string url, bool *temp, bool *success);

/*
 Load the given URL to a file asynchronously, and invoke the given callbacks on success
 or failure. The success callback returns the path to the file, and, if the file
 is temporary and must be deleted after it's processed, will return true for the temp
 boolean.
 If the download fails, the onFailure() callback will be invoked.
 */
void VROPlatformDownloadURLToFileAsync(std::string url,
                                       std::function<void(std::string, bool)> onSuccess,
                                       std::function<void()> onFailure);
void VROPlatformDeleteFile(std::string filename);

/*
 Load the file into a binary blob and return it. The returned buffer
 *must* be freed by the caller!
 */
void *VROPlatformLoadFile(std::string filename, int *outLength);

/*
 Find the path and index (within the font collection) for the font that most nearly
 matches the given typeface, style, and weight. If the index is -1, then no suitable
 font was found.
 */
std::pair<std::string, int> VROPlatformFindFont(std::string typeface, bool isItalic, int weight);

#pragma mark - Device Information

/*
 Returns the device's model name
 */
std::string VROPlatformGetDeviceModel();

/*
 Returns the device's brand
 */
std::string VROPlatformGetDeviceBrand();

std::string VROPlatformGetCacheDirectory();

#pragma mark - Image Loading

// Returns empty shared_ptr on failure
std::shared_ptr<VROImage> VROPlatformLoadImageFromFile(std::string filename, VROTextureInternalFormat format);
std::shared_ptr<VROImage> VROPlatformLoadImageWithBufferedData(std::vector<unsigned char> rawData,
                                                               VROTextureInternalFormat format);

#if VRO_PLATFORM_ANDROID
std::shared_ptr<VROImage> VROPlatformLoadImageFromAsset(std::string asset, VROTextureInternalFormat format);

// These methods return null on error (e.g. if file or asset is not found).
jobject VROPlatformLoadBitmapFromAsset(std::string resource, VROTextureInternalFormat format);
jobject VROPlatformLoadBitmapFromFile(std::string path, VROTextureInternalFormat format);

// Note the returned buffer *must* be freed by the caller!
void *VROPlatformConvertBitmap(jobject jbitmap, int *bitmapLength, int *width, int *height, bool *hasAlpha);

// Test function to save RGBA8 data as PNG files
void VROPlatformSaveRGBAImage(void *data, int length, int width, int height, std::string path);

// Returns a best guess of the VROTextureFormat based on the given bitmap's info
VROTextureFormat VROPlatformGetBitmapFormat(jobject jbitmap);

#endif

#pragma mark - Threading

/*
 Run the given function on the rendering thread, asynchronously (this function
 returns immediately).
 */
void VROPlatformDispatchAsyncRenderer(std::function<void()> fcn);

/*
 Run the given function on a background thread. The thread can be pooled, 
 or spun up fresh. The caller should make no assumptions.
 */
void VROPlatformDispatchAsyncBackground(std::function<void()> fcn);

/*
 Run the given function on the application UI thread, asynchronously.
 */
void VROPlatformDispatchAsyncApplication(std::function<void()> fcn);

/*
 Flushes the task queues from calling VROPlatformDispatch* before VROPlatformUtil
 was set up.
 */
void VROPlatformFlushTaskQueues();

#if VRO_PLATFORM_IOS

/*
 The EAGL context is required by iOS so that we can ensure it is bound during
 async dispatch calls to the renderer.
 */
@class EAGLContext;
void VROPlatformSetEAGLContext(EAGLContext *context);

#endif

#if VRO_PLATFORM_MACOS

@class NSOpenGLContext;
@class VROViewScene;
void VROPlatformSetOpenGLContext(NSOpenGLContext *context, VROViewScene *scene);

#endif

#if VRO_PLATFORM_IOS || VRO_PLATFORM_MACOS
#import <Foundation/Foundation.h>

NSURLSessionDataTask *VROPlatformDownloadDataWithURL(NSURL *url, void (^completionBlock)(NSData *data, NSError *error));

#endif

#pragma mark - Android Setup

#if VRO_PLATFORM_ANDROID

void VROPlatformSetEnv(JNIEnv *env, jobject appContext, jobject assetManager, jobject platformUtil);
void VROPlatformReleaseEnv();

jobject VROPlatformGetJavaAppContext();
jobject VROPlatformGetJavaAssetManager();
AAssetManager *VROPlatformGetAssetManager();

// Copy the given asset into a file with the same name in the cache dir.
// This enables us to load assets through routines that only take file paths,
// for testing purposes only. Not needed in prod because assets are not used
// in prod.
std::string VROPlatformCopyAssetToFile(std::string asset);

/**
 * This function takes a java map of filename -> resource location and copies the resources
 * to the Android cache w/ the given filename. It then returns a c++ map of filename -> download
 * location.
 */
std::map<std::string, std::string> VROPlatformCopyObjResourcesToFile(jobject resourceMap);

#pragma mark - JNI Utilities

// Get the class loader from the given object
jobject VROPlatformGetClassLoader(JNIEnv *jni, jobject jcontext);

// Find a class using the class loader associated with the given object,
// typically an activity.
// This is safe to call from any thread, even those not spawned in Java
// (i.e. threads created using pthread_create).
jclass VROPlatformFindClass(JNIEnv *jni, jobject javaObject, const char *className);

#pragma mark - Android Image Tracking Debugging

/*
 This function calls setImageOnTrackingImageView on ViroViewARCore in order to debug
 Image Tracking/Detection
 */
void VROPlatformSetTrackingImageView(std::string filepath);

#pragma mark - Android A/V

// Create a video sink on the Java side. Returns the Surface.
jobject VROPlatformCreateVideoSink(int textureId);
jobject VROPlatformCreateVideoSink(int textureId, int width, int height);
void VROPlatformDestroyVideoSink(int textureId);

// Get audio properties for this device.
int VROPlatformGetAudioSampleRate();
int VROPlatformGetAudioBufferSize();

extern "C" {

void Java_com_viro_core_internal_PlatformUtil_runTask(JNIEnv *env, jclass clazz, jint taskId);

}

#endif

#pragma mark - C API (Android + WebAssembly)

#if VRO_PLATFORM_ANDROID || VRO_PLATFORM_WASM

#include VRO_C_INCLUDE

// This function was added because VROPlatformConvertBitmap can be called before the renderer
// is created and as a result, activity and assetManager hasn't been set yet. We should think
// about how to do this better.
void VROPlatformSetEnv(VRO_ENV env);

// Get the host platform environment
VRO_ENV VROPlatformGetJNIEnv();

// Helper functions for setting host object properties through from C++
void VROPlatformSetFloat(VRO_ENV env, VRO_OBJECT obj, const char *fieldName, VRO_FLOAT value);
void VROPlatformSetString(VRO_ENV env, VRO_OBJECT obj, const char *fieldName, std::string value);
void VROPlatformSetInt(VRO_ENV env, VRO_OBJECT obj, const char *fieldName, VRO_INT value);
void VROPlatformSetBool(VRO_ENV env, VRO_OBJECT obj, const char *fieldName, VRO_BOOL value);
void VROPlatformSetEnumValue(VRO_ENV env, VRO_OBJECT obj, const char *fieldName,
                             std::string enumClassPathName, std::string enumValueStr);
void VROPlatformSetObject(VRO_ENV env, VRO_OBJECT obj, const char *fieldName,
                          const char *fieldType, VRO_OBJECT object);

// This function takes a host object Map (Java: java.util.Map) with string keys and values,
// and returns a C++ std::map w/ std::string key and values.
std::map<std::string, std::string> VROPlatformConvertFromJavaMap(VRO_OBJECT javaMap);

#if VRO_PLATFORM_ANDROID

template<typename... Args>
void VROPlatformCallHostFunction(jobject javaObject,
                                 std::string methodName,
                                 std::string methodSig,
                                 Args... args) {
    JNIEnv *env = VROPlatformGetJNIEnv();
    env->ExceptionClear();

    if (javaObject == nullptr) {
        perr("Unable to call (void) method on null object [function %s, method %s]",
             methodName.c_str(), methodSig.c_str());
        return;
    }
    jclass viroClass = env->GetObjectClass(javaObject);
    if (viroClass == nullptr) {
        perr("Unable to find class for making java calls [function %s, method %s]",
             methodName.c_str(), methodSig.c_str());
        return;
    }

    jmethodID method = env->GetMethodID(viroClass, methodName.c_str(), methodSig.c_str());
    if (method == nullptr) {
        perr("Unable to find method %s callback.", methodName.c_str());
        return;
    }

    env->CallVoidMethod(javaObject, method, std::forward<Args>(args)...);
    if (env->ExceptionOccurred()) {
        perr("Exception occurred when calling %s.", methodName.c_str());
        env->ExceptionDescribe();
        std::string errorString = "A java exception has been thrown when calling " + methodName;
        throw std::runtime_error(errorString.c_str());
    }

    env->DeleteLocalRef(viroClass);
}

template<typename... Args>
VRO_BOOL VROPlatformCallHostBoolFunction(jobject javaObject,
                                         std::string methodName,
                                         std::string methodSig,
                                         Args... args) {
    JNIEnv *env = VROPlatformGetJNIEnv();
    env->ExceptionClear();

    if (javaObject == nullptr) {
        perr("Unable to call (bool) method on null object [function %s, method %s]",
             methodName.c_str(), methodSig.c_str());
        return false;
    }
    jclass viroClass = env->GetObjectClass(javaObject);
    if (viroClass == nullptr) {
        perr("Unable to find class for making java calls [function %s, method %s]",
             methodName.c_str(), methodSig.c_str());
        return false;
    }

    jmethodID method = env->GetMethodID(viroClass, methodName.c_str(), methodSig.c_str());
    if (method == nullptr) {
        perr("Unable to find method %s", methodName.c_str());
        return false;
    }

    bool result = env->CallBooleanMethod(javaObject, method, std::forward<Args>(args)...);
    if (env->ExceptionOccurred()) {
        perr("Exception occurred when calling %s", methodName.c_str());
        env->ExceptionDescribe();
        std::string errorString = "Java exception thrown when calling " + methodName;
        throw std::runtime_error(errorString.c_str());
    }

    env->DeleteLocalRef(viroClass);
    return result;
}

template<typename... Args>
VRO_INT VROPlatformCallHostIntFunction(jobject javaObject,
                                       std::string methodName,
                                       std::string methodSig,
                                       Args... args) {
    JNIEnv *env = VROPlatformGetJNIEnv();
    env->ExceptionClear();

    if (javaObject == nullptr) {
        perr("Unable to call (int) method on null object [function %s, method %s]",
             methodName.c_str(), methodSig.c_str());
        return 0;
    }
    jclass viroClass = env->GetObjectClass(javaObject);
    if (viroClass == nullptr) {
        perr("Unable to find class for making java calls [function %s, method %s]",
             methodName.c_str(), methodSig.c_str());
        return 0;
    }

    jmethodID method = env->GetMethodID(viroClass, methodName.c_str(), methodSig.c_str());
    if (method == nullptr) {
        perr("Unable to find method %s", methodName.c_str());
        return 0;
    }

    jint result = env->CallIntMethod(javaObject, method, std::forward<Args>(args)...);
    if (env->ExceptionOccurred()) {
        perr("Exception occurred when calling %s", methodName.c_str());
        env->ExceptionDescribe();
        std::string errorString = "Java exception thrown when calling " + methodName;
        throw std::runtime_error(errorString.c_str());
    }

    env->DeleteLocalRef(viroClass);
    return result;
}

template<typename... Args>
jlong VROPlatformCallHostLongFunction(jobject javaObject,
                                      std::string methodName,
                                      std::string methodSig,
                                      Args... args) {
    JNIEnv *env = VROPlatformGetJNIEnv();
    env->ExceptionClear();

    if (javaObject == nullptr) {
        perr("Unable to call (long) method on null object [function %s, method %s]",
             methodName.c_str(), methodSig.c_str());
        return 0;
    }
    jclass viroClass = env->GetObjectClass(javaObject);
    if (viroClass == nullptr) {
        perr("Unable to find class for making java calls [function %s, method %s]",
             methodName.c_str(), methodSig.c_str());
        return 0;
    }

    jmethodID method = env->GetMethodID(viroClass, methodName.c_str(), methodSig.c_str());
    if (method == nullptr) {
        perr("Unable to find method %s", methodName.c_str());
        return 0;
    }

    jlong result = env->CallLongMethod(javaObject, method, std::forward<Args>(args)...);
    if (env->ExceptionOccurred()) {
        perr("Exception occurred when calling %s", methodName.c_str());
        env->ExceptionDescribe();
        std::string errorString = "Java exception thrown when calling " + methodName;
        throw std::runtime_error(errorString.c_str());
    }

    env->DeleteLocalRef(viroClass);
    return result;
}

template<typename... Args>
jobject VROPlatformCallHostObjectFunction(jobject javaObject,
                                          std::string methodName,
                                          std::string methodSig,
                                          Args... args) {
    JNIEnv *env = VROPlatformGetJNIEnv();
    env->ExceptionClear();

    if (javaObject == nullptr) {
        perr("Unable to call (object) method  on null object [function %s, method %s]",
             methodName.c_str(), methodSig.c_str());
        return 0;
    }
    jclass viroClass = env->GetObjectClass(javaObject);
    if (viroClass == nullptr) {
        perr("Unable to find class for making java calls [function %s, method %s]",
             methodName.c_str(), methodSig.c_str());
        return 0;
    }

    jmethodID method = env->GetMethodID(viroClass, methodName.c_str(), methodSig.c_str());
    if (method == nullptr) {
        perr("Unable to find method %s with signature [%s]", methodName.c_str(), methodSig.c_str());
        return 0;
    }

    jobject result = env->CallObjectMethod(javaObject, method, std::forward<Args>(args)...);
    if (env->ExceptionOccurred()) {
        perr("Exception occurred when calling %s", methodName.c_str());
        env->ExceptionDescribe();
        std::string errorString = "Java exception thrown when calling " + methodName;
        throw std::runtime_error(errorString.c_str());
    }

    env->DeleteLocalRef(viroClass);
    return result;
}

template<typename... Args>
VRO_OBJECT VROPlatformConstructHostObject(std::string className,
                                          std::string constructorSig,
                                          Args... args) {
    JNIEnv *env = VROPlatformGetJNIEnv();
    env->ExceptionClear();

    jclass cls = env->FindClass(className.c_str());
    jmethodID constructor = env->GetMethodID(cls, "<init>", constructorSig.c_str());

    jobject object = env->NewObject(cls, constructor, std::forward<Args>(args)...);
    if (env->ExceptionOccurred()) {
        perr("Exception occurred when calling constructor %s", constructorSig.c_str());
        env->ExceptionDescribe();

        std::string errorString = "A java exception has been thrown when calling constructor " + constructorSig;
        throw std::runtime_error(errorString.c_str());
    }

    env->DeleteLocalRef(cls);
    return object;
}

#else

template<typename... Args>
void VROPlatformCallHostFunction(VRO_OBJECT object,
                                 std::string methodName,
                                 std::string methodSig,
                                 Args... args) {
    object.call<void>(methodName.c_str(), std::forward<Args>(args)...);
}

template<typename... Args>
VRO_LONG VROPlatformCallHostLongFunction(VRO_OBJECT object,
                                         std::string methodName,
                                         std::string methodSig,
                                         Args... args) {
    return object.call<VRO_LONG>(methodName.c_str(), std::forward<Args>(args)...);
}

template<typename... Args>
VRO_BOOL VROPlatformCallHostBoolFunction(VRO_OBJECT object,
                                         std::string methodName,
                                         std::string methodSig,
                                         Args... args) {
    return object.call<VRO_BOOL>(methodName.c_str(), std::forward<Args>(args)...);
}

template<typename... Args>
VRO_OBJECT VROPlatformConstructHostObject(std::string className,
                                          std::string constructorSig,
                                          Args... args) {
    // TODO wasm
    return VRO_OBJECT_NULL;
}

#endif // WASM
#endif // ANDROID || WASM

#if VRO_PLATFORM_IOS || VRO_PLATFORM_ANDROID

// Returns enum of type gvr_audio_material_type
int VROPlatformParseGVRAudioMaterial(std::string property);

#endif

#endif /* VROPlatformUtil_h */
