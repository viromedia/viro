//
//  VROPlatformUtil.h
//  ViroRenderer
//
//  Created by Raj Advani on 11/7/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#ifndef VROPlatformUtil_h
#define VROPlatformUtil_h

#include "VRODefines.h"
#include "VROTexture.h"
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

std::string VROPlatformGetCacheDirectory();

#pragma mark - Image Loading

// Returns empty shared_ptr on failure
std::shared_ptr<VROImage> VROPlatformLoadImageFromFile(std::string filename, VROTextureInternalFormat format);

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
#import <UIKit/UIKit.h>

NSURLSessionDataTask *VROPlatformDownloadDataWithURL(NSURL *url, void (^completionBlock)(NSData *data, NSError *error));

#endif

#pragma mark - Android Setup

#if VRO_PLATFORM_ANDROID

void VROPlatformSetEnv(JNIEnv *env, jobject appContext, jobject assetManager, jobject platformUtil);

// This function was added because VROPlatformConvertBitmap can be called before the renderer
// is created and as a result, activity and assetManager hasn't been set yet. We should think
// about how to do this better.
void VROPlatformSetEnv(JNIEnv *env);
void VROPlatformReleaseEnv();

JNIEnv *VROPlatformGetJNIEnv();
jobject VROPlatformGetJavaAppContext();
jobject VROPlatformGetJavaAssetManager();
AAssetManager *VROPlatformGetAssetManager();

// Copy the given asset into a file with the same name in the cache dir.
// This enables us to load assets through routines that only take file paths,
// for testing purposes only. Not needed in prod because assets are not used
// in prod.
std::string VROPlatformCopyAssetToFile(std::string asset);

/**
 * This function takes a Java Map (java.util.Map) w/ String key and values and returns
 * a C++ std::map w/ std::string key and values.
 */
std::map<std::string, std::string> VROPlatformConvertFromJavaMap(jobject javaMap);

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

// Calls a java function from native through JNI on the given jObject with the given
// functionName, methodID and desired java function parameters.
//
// Example: VROPlatformCallJavaFunction(jObj,
//                                      "onHover",
//                                      "(Z)V",
//                                      isGazing);
void VROPlatformCallJavaFunction(jobject javaObject,
                                 std::string functionName,
                                 std::string methodID, ...);
jlong VROPlatformCallJavaLongFunction(jobject javaObject,
                                     std::string functionName,
                                     std::string methodID, ...);

// Helper functions for setting jObject properties through JNI from C++
void VROPlatformSetFloat(JNIEnv *env, jclass cls, jobject jObj, const char *fieldName, jfloat value);
void VROPlatformSetString(JNIEnv *env, jclass cls, jobject jObj, const char *fieldName, std::string value);
void VROPlatformSetInt(JNIEnv *env, jclass cls, jobject jObj, const char *fieldName, jint value);
void VROPlatformSetBool(JNIEnv *env, jclass cls, jobject jObj, const char *fieldName, jboolean value);
void VROPlatformSetEnumValue(JNIEnv *env, jclass cls, jobject jObj, const char *fieldName,
                             std::string enumClassPathName, std::string enumValueStr);

// Safely converts the given string with the provided jni environment.
std::string VROPlatformGetString(jstring string, JNIEnv *env);

#pragma mark - Android Image Tracking Debugging
/*
 This function calls setImageOnTrackingImageView on ViroViewARCore in order to debug
 Image Tracking/Detection
 */
void VROPlatformSetTrackingImageView(std::string filepath);

#pragma mark - Android A/V

// Create a video sink on the Java side. Returns the Surface.
jobject VROPlatformCreateVideoSink(int textureId);
void VROPlatformDestroyVideoSink(int textureId);

// Get audio properties for this device.
int VROPlatformGetAudioSampleRate();
int VROPlatformGetAudioBufferSize();

extern "C" {

void Java_com_viro_core_internal_PlatformUtil_runTask(JNIEnv *env, jclass clazz, jint taskId);

}

#endif

#if VRO_PLATFORM_IOS || VRO_PLATFORM_ANDROID

// Returns enum of type gvr_audio_material_type
int VROPlatformParseGVRAudioMaterial(std::string property);

#endif

#endif /* VROPlatformUtil_h */
