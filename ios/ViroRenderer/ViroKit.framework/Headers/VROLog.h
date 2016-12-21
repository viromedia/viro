//
//  VROLog.h
//  ViroRenderer
//
//  Created by Raj Advani on 10/21/15.
//  Copyright © 2015 Viro Media. All rights reserved.
//

#include "VRODefines.h"

#pragma once
//// Test debug defines
//#ifdef NDEBUG
//#pragma message "NDEBUG is #defined."
//#else
//#pragma message "NDEBUG is not #defined."
//#endif

#undef PASSERT_INNERLOOPS_ENABLED // Do not define, unless you want passert_innerloop() asserts
// to run, which are passert_msg() calls that are made inside inner loops, which would slow down
// the app massively if on.  Used for debugging those particular loops only, and left off to not slow
// the app down for other developers.

/////////////////////////////////////////////////////////////////////////////////
//
//  Android: Logging
//
/////////////////////////////////////////////////////////////////////////////////
#pragma mark -
#pragma mark Android Logging

#if VRO_PLATFORM_ANDROID

#include <android/log.h>
#include <jni.h>
#include <sys/system_properties.h>

// --------------------------------
// Escape sequences for logging color:
// 16-color = 4-bits = [highlight][blue][green][red]
// These are #defines instead of const char [] so they can be used more easily in perr() and pfatal().
// http://en.wikipedia.org/wiki/ANSI_escape_code

// --------
// special
#define ANSINoColor "\e[0m" // reset / normal
#define ANSIBold "\e[1m" // bold

// NOTE: the following uses 0 (reset) and 1 (bold) for intensities,
// which is used by BOTH foreground and background,
// and 0 will reset BOTH the foreground and background,
// thus this doesn't work: ANSIBackBlue ANSICyan
// since the "0" in Cyan is a reset, which resets the background.

// --------
// foreground
// light colors
#define ANSIBlack "\e[0;30m"
#define ANSIRed "\e[0;31m"
#define ANSIGreen "\e[0;32m"
#define ANSIBrown "\e[0;33m"
#define ANSIDarkYellow "\e[0;33m"
#define ANSIBlue "\e[0;34m"
#define ANSIMagenta "\e[0;35m"
#define ANSICyan "\e[0;36m"
#define ANSILightGray "\e[0;37m"
// bright colors
#define ANSIDarkGray "\e[1;30m"
#define ANSILightRed "\e[1;31m"
#define ANSILightGreen "\e[1;32m"
#define ANSIYellow "\e[1;33m"
#define ANSILightBlue "\e[1;34m"
#define ANSILightMagenta "\e[1;35m"
#define ANSILightCyan "\e[1;36m"
#define ANSIWhite "\e[1;37m"
// default text color
#define ANSIDefault "\e[1;39m"

// --------
// background
// light colors
#define ANSIBackBlack "\e[0;40m"
#define ANSIBackRed "\e[0;41m"
#define ANSIBackGreen "\e[0;42m"
#define ANSIBackBrown "\e[0;43m"
#define ANSIBackBlue "\e[0;44m"
#define ANSIBackMagenta "\e[0;45m"
#define ANSIBackCyan "\e[0;46m"
#define ANSIBackLightGray "\e[0;47m"
// bright colors
#define ANSIBackDarkGray "\e[1;40m"
#define ANSIBackLightRed "\e[1;41m"
#define ANSIBackLightGreen "\e[1;42m"
#define ANSIBackYellow "\e[1;43m"
#define ANSIBackLightBlue "\e[1;44m"
#define ANSIBackLightMagenta "\e[1;45m"
#define ANSIBackLightCyan "\e[1;46m"
#define ANSIBackWhite "\e[1;47m"
// default background color
#define ANSIBackDefault "\e[1;49m"

#elif VRO_PLATFORM_IOS

/*
 ANSI colors don't resolve on the iOS debug console, so we
 deactivate them.
 */
#define ANSINoColor ""
#define ANSIBold ""
#define ANSIBlack ""
#define ANSIRed ""
#define ANSIGreen ""
#define ANSIBrown ""
#define ANSIDarkYellow ""
#define ANSIBlue ""
#define ANSIMagenta ""
#define ANSICyan ""
#define ANSILightGray ""
#define ANSIDarkGray ""
#define ANSILightRed ""
#define ANSILightGreen ""
#define ANSIYellow ""
#define ANSILightBlue ""
#define ANSILightMagenta ""
#define ANSILightCyan ""
#define ANSIWhite ""
#define ANSIDefault ""
#define ANSIBackBlack ""
#define ANSIBackRed ""
#define ANSIBackGreen ""
#define ANSIBackBrown ""
#define ANSIBackBlue ""
#define ANSIBackMagenta ""
#define ANSIBackCyan ""
#define ANSIBackLightGray ""
#define ANSIBackDarkGray ""
#define ANSIBackLightRed ""
#define ANSIBackLightGreen ""
#define ANSIBackYellow ""
#define ANSIBackLightBlue ""
#define ANSIBackLightMagenta ""
#define ANSIBackLightCyan ""
#define ANSIBackWhite ""
#define ANSIBackDefault ""

#endif

/////////////////////////////////////////////////////////////////////////////////
//
//  Android: Logging
//
/////////////////////////////////////////////////////////////////////////////////
#pragma mark -
#pragma mark Android Logging

// define LOG_TAG, if not already defined
#ifndef LOG_TAG
#define LOG_TAG "Viro"
#endif

#if VRO_PLATFORM_ANDROID

// Why do-while(0)?
// --------------------
// It allows this:
// 1. protects against multiple statements when the #define, when it is called like a single-statement.
// 2. the while loop doesn't have a ; at the end, so that it doesn't end an if-statement with the #define called inside.
// 3. it also allows local variables to not interfere with ones on the outside.
// https://www.securecoding.cert.org/confluence/display/seccode/PRE10-C.+Wrap+multistatement+macros+in+a+do-while+loop

#if NDEBUG
    // Release build
    #define pverbose(...) ((void)0)
    #define cpverbose(tag, ...) ((void)0)
    #define pdebug(...) ((void)0)
    #define cpdebug(tag, ...) ((void)0)
#else
    // Debug build

    // verbose:
    #define pverbose(...) \
        do { \
            __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__); \
        } while (0)

    #define cpverbose(tag, ...) \
        do { \
            __android_log_print(ANDROID_LOG_VERBOSE, tag, __VA_ARGS__); \
        } while (0)

    // debug:
    #define pdebug(...) \
        do { \
            __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__); \
        } while (0)

    #define cpdebug(tag, ...) \
        do { \
            __android_log_print(ANDROID_LOG_DEBUG, tag, __VA_ARGS__); \
        } while (0)
#endif

// info:
#define pinfo(...) \
    do { \
        __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__); \
    } while (0)

#define cpinfo(tag, ...) \
    do { \
        __android_log_print(ANDROID_LOG_INFO, tag, __VA_ARGS__); \
    } while (0)

// warn:
#define pwarn(...) \
    do { \
        __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__); \
    } while (0)

#define cpwarn(tag, ...) \
    do { \
        __android_log_print(ANDROID_LOG_WARN, tag, __VA_ARGS__); \
    } while (0)

// error:
#define perr(message, ...) \
    do { \
        __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, \
                ANSILightRed "ERROR: [%s: %s(), line %d]%s\n%sERROR: %s%s" message ANSINoColor, \
                __FILE__, __FUNCTION__, __LINE__, ANSINoColor, ANSILightRed, ANSINoColor, ANSIYellow, \
                ##__VA_ARGS__); \
    } while (0)

#define cperr(tag, message, ...) \
    do { \
        __android_log_print(ANDROID_LOG_ERROR, tag, \
                ANSILightRed "ERROR: [%s: %s(), line %d]%s\n%sERROR: %s%s" message ANSINoColor, \
                __FILE__, __FUNCTION__, __LINE__, ANSINoColor, ANSILightRed, ANSINoColor, ANSIYellow, \
                ##__VA_ARGS__); \
    } while (0)

// fatal:
#define pfatal(message, ...) \
    do { \
        __android_log_print(ANDROID_LOG_FATAL, LOG_TAG, \
                ANSILightRed "FATAL ERROR: [%s:%d]%s\n%sFATAL ERROR: %s%s" message ANSINoColor, \
                __FILE__, __LINE__, ANSINoColor, ANSILightRed, ANSINoColor, ANSIYellow, \
                ##__VA_ARGS__); \
        abort(); \
    } while (0)

#define cpfatal(tag, message, ...) \
    do { \
        __android_log_print(ANDROID_LOG_FATAL, tag, \
                ANSILightRed "FATAL ERROR: [%s:%d]%s\n%sFATAL ERROR: %s%s" message ANSINoColor, \
                __FILE__, __LINE__, ANSINoColor, ANSILightRed, ANSINoColor, ANSIYellow, \
                ##__VA_ARGS__); \
        abort(); \
    } while (0)

#define pgllabel(message,...) ((void)0)
#define pglpush(message,...) ((void)0)
#define pglpop() ((void)0)

// __android_log_assert():
// Log an assertion failure and SIGTRAP the process to have a chance
// to inspect it. This uses the FATAL priority.
#define passert(condition) \
    do \
    { \
        if (!(condition)) {                               \
            _pabort(__FILE__, __LINE__, __func__,                   \
                    "ASSERTION FAILED\n"                            \
                    "  Expression: %s",                             \
                    #condition);                                    \
        }                                                           \
    } while (0)

#if VRO_THREADPOOL_LOG
    #define pthreadpool(message,...) \
    do { \
    __android_log_print(ANDROID_LOG_VERBOSE, threadPoolName.c_str(), "%s%s(): "#message" %s", ANSILightMagenta, __FUNCTION__, ##__VA_ARGS__, ANSINoColor); \
    } while (0)

    #define pthreadtask(message,...) \
    do { \
    __android_log_print(ANDROID_LOG_VERBOSE, threadIdentifier.c_str(), "    %s%s(): "#message" %s", ANSILightCyan, __FUNCTION__, ##__VA_ARGS__, ANSINoColor); \
    } while (0)

    #define pthreadtaskexe(message,...) \
    do { \
    __android_log_print(ANDROID_LOG_VERBOSE, threadIdentifier.c_str(), "      %s%s(): "#message" %s", ANSILightBlue, __FUNCTION__, ##__VA_ARGS__, ANSINoColor); \
    } while(0)

    #define pthreadtaskerror(message,...) \
    do { \
    __android_log_print(ANDROID_LOG_VERBOSE, threadIdentifier.c_str(), "      %s%s(): "#message" %s", ANSIRed, __FUNCTION__, ##__VA_ARGS__, ANSINoColor); \
    } while(0)
#else
    #define pthreadpool(message,...) ((void)0)
    #define pthreadtask(message,...) ((void)0)
    #define pthreadtaskexe(message,...) ((void)0)
    #define pthreadtaskerror(message,...) ((void)0)
#endif

#define VERBOSE_LOGGING 0 // turn this on during development as needed

#if NDEBUG
    // Release build
    #define DEBUG_LOGGING 0
#else
    // Debug build
    #define DEBUG_LOGGING 1
#endif

static inline int
toLevel(const char* value) {
    switch (value[0]) {
        case 'V': return ANDROID_LOG_VERBOSE;
        case 'D': return ANDROID_LOG_DEBUG;
        case 'I': return ANDROID_LOG_INFO;
        case 'W': return ANDROID_LOG_WARN;
        case 'E': return ANDROID_LOG_ERROR;
        case 'A': return ANDROID_LOG_FATAL;
        case 'S': return ANDROID_LOG_SILENT;
    }
    return ANDROID_LOG_INFO;
}

#define isLoggable(tag, level)                                  \
    ({                                                          \
        char value[PROP_VALUE_MAX];                             \
        __system_property_get("log.tag." tag, value);           \
        int logLevel = toLevel(value);                          \
        logLevel != ANDROID_LOG_SILENT && level >= logLevel;    \
    })

#elif VRO_PLATFORM_IOS

/////////////////////////////////////////////////////////////////////////////////
//
//  iOS: Logging
//
/////////////////////////////////////////////////////////////////////////////////
#pragma mark -
#pragma mark iOS Logging

#include <assert.h>
#include <Foundation/Foundation.h>

#define passert(condition) (assert(condition))

#define cpverbose(tag, ...) ((void)0)
#define cpdebug(tag, ...) ((void)0)
#define pverbose(message,...) ((void)0)

#define pdebug(message,...) \
do { \
\
} while (0)

#define pinfo(message,...) \
do { \
NSLog(@#message, ##__VA_ARGS__); \
} while (0)

#define pwarn(message,...) \
do { \
NSLog(@#message, ##__VA_ARGS__); \
} while (0)

#define perr(message,...) \
do { \
NSLog(@"Error: "#message, ##__VA_ARGS__); \
} while (0)

#define pfatal(message,...) \
do { \
NSLog(@"Fatal Error: "#message, ##__VA_ARGS__); \
} while (0)

#if VRO_THREADPOOL_LOG
#define pthreadpool(message,...) \
do { \
NSLog(@"[%s]%s(): "#message, threadPoolName.c_str(), __FUNCTION__, ##__VA_ARGS__); \
} while (0)

#define pthreadtask(message,...) \
do { \
NSLog(@"[%s]%s(): "#message, threadIdentifier.c_str(), __FUNCTION__, ##__VA_ARGS__); \
} while (0)

#define pthreadtaskexe(message,...) \
do { \
NSLog(@"[%s]%s(): "#message, threadIdentifier.c_str(), __FUNCTION__, ##__VA_ARGS__); \
} while (0)

#define pthreadtaskerror(message,...) \
do { \
NSLog(@"[%s]%s(): "#message, threadIdentifier.c_str(), __FUNCTION__, ##__VA_ARGS__); \
} while (0)
#else
#define pthreadpool(message,...) ((void)0)
#define pthreadtask(message,...) ((void)0)
#define pthreadtaskexe(message,...) ((void)0)
#define pthreadtaskerror(message,...) ((void)0)
#endif

#define pgllabel(message,...) \
do { \
char str[1024]; \
sprintf(str, #message, ##__VA_ARGS__); \
glInsertEventMarkerEXT(0, str); \
} while (0)

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

#define cpinfo(tag, ...) \
do { \
\
} while (0)

#endif

/////////////////////////////////////////////////////////////////////////////////
//
//  Common: Logging
//
/////////////////////////////////////////////////////////////////////////////////
#pragma mark -
#pragma mark Common (Android & iOS) Logging

// --------------------------------
// variables:
#define pStringifyBool(boolExpression) ((boolExpression) ? "true" : "false")
#define pStringifyNull(stringExpression) ((stringExpression) ? stringExpression : "null")
#define pbool(b) pinfo("%s%s()%s: "#b" = %s%s%s.", ANSILightMagenta, __FUNCTION__, ANSINoColor, ANSILightBlue, pStringifyBool(b), ANSINoColor);
#define pboolean(b) pbool(b);
#define pbyte(b) pinfo("%s%s()%s: "#b" = %s%d = 0x%02X = %d%d%d%d%d%d%d%d%s.", \
        ANSILightMagenta, __FUNCTION__, ANSINoColor, ANSILightBlue, b, b, \
        static_cast<int>((b&(1<<7))!=0), \
        static_cast<int>((b&(1<<6))!=0), \
        static_cast<int>((b&(1<<5))!=0), \
        static_cast<int>((b&(1<<4))!=0), \
        static_cast<int>((b&(1<<3))!=0), \
        static_cast<int>((b&(1<<2))!=0), \
        static_cast<int>((b&(1<<1))!=0), \
        static_cast<int>((b&(1<<0))!=0), ANSINoColor)
#define pint(i) pinfo("%s%s()%s: "#i" = %s%d%s.", ANSILightMagenta, __FUNCTION__, ANSINoColor, ANSILightBlue, i, ANSINoColor)
#define puint(i) pinfo("%s%s()%s: "#i" = %s%u%s.", ANSILightMagenta, __FUNCTION__, ANSINoColor, ANSILightBlue, i, ANSINoColor)
#define pbits(bitfield) pinfo("%s%s()%s: "#bitfield \
        " = %s%d = 0x%08X = %d%d%d%d%d%d%d%d-%d%d%d%d%d%d%d%d-%d%d%d%d%d%d%d%d-%d%d%d%d%d%d%d%d%s.", \
        ANSILightMagenta, __FUNCTION__, ANSINoColor, ANSILightBlue, bitfield, bitfield, \
        static_cast<int>((bitfield&(1<<31))!=0), \
        static_cast<int>((bitfield&(1<<30))!=0), \
        static_cast<int>((bitfield&(1<<29))!=0), \
        static_cast<int>((bitfield&(1<<28))!=0), \
        static_cast<int>((bitfield&(1<<27))!=0), \
        static_cast<int>((bitfield&(1<<26))!=0), \
        static_cast<int>((bitfield&(1<<25))!=0), \
        static_cast<int>((bitfield&(1<<24))!=0), \
        static_cast<int>((bitfield&(1<<23))!=0), \
        static_cast<int>((bitfield&(1<<22))!=0), \
        static_cast<int>((bitfield&(1<<21))!=0), \
        static_cast<int>((bitfield&(1<<20))!=0), \
        static_cast<int>((bitfield&(1<<19))!=0), \
        static_cast<int>((bitfield&(1<<18))!=0), \
        static_cast<int>((bitfield&(1<<17))!=0), \
        static_cast<int>((bitfield&(1<<16))!=0), \
        static_cast<int>((bitfield&(1<<15))!=0), \
        static_cast<int>((bitfield&(1<<14))!=0), \
        static_cast<int>((bitfield&(1<<13))!=0), \
        static_cast<int>((bitfield&(1<<12))!=0), \
        static_cast<int>((bitfield&(1<<11))!=0), \
        static_cast<int>((bitfield&(1<<10))!=0), \
        static_cast<int>((bitfield&(1<<9))!=0), \
        static_cast<int>((bitfield&(1<<8))!=0), \
        static_cast<int>((bitfield&(1<<7))!=0), \
        static_cast<int>((bitfield&(1<<6))!=0), \
        static_cast<int>((bitfield&(1<<5))!=0), \
        static_cast<int>((bitfield&(1<<4))!=0), \
        static_cast<int>((bitfield&(1<<3))!=0), \
        static_cast<int>((bitfield&(1<<2))!=0), \
        static_cast<int>((bitfield&(1<<1))!=0), \
        static_cast<int>((bitfield&(1<<0))!=0), ANSINoColor)
#define pfloat(f) pinfo("%s%s()%s: "#f" = %s%f%s.", ANSILightMagenta, __FUNCTION__, ANSINoColor, ANSILightBlue, f, ANSINoColor)
#define pdouble(d) pinfo("%s%s()%s: "#d" = %s%f%s.", ANSILightMagenta, __FUNCTION__, ANSINoColor, ANSILightBlue, d, ANSINoColor)
#define ppointer(p) pinfo("%s%s()%s: "#p" = %s%p%s", ANSILightMagenta, __FUNCTION__, ANSINoColor, ANSILightBlue, (void*)p, ANSINoColor)
// Note: cannot use pptr(), as it is used in boost.

// --------------------------------
// complex types or combos:
#define pxy(x,y) pinfo("%s%s()%s: ("#x", "#y") = (%s%f, %f%s).", ANSILightMagenta, __FUNCTION__, ANSINoColor, ANSILightBlue, x, y, ANSINoColor)
#define pintxy(x,y) pinfo("%s%s()%s: ("#x", "#y") = (%s%d, %d%s).", ANSILightMagenta, __FUNCTION__, ANSINoColor, ANSILightBlue, x, y, ANSINoColor)

/*
 Logs an init array.
 13 characters per float = 11 for "+1000111222", 2 for ", ".
 Note the length of the arary is +1, so that snprintf() has
 room to write the NULL terminater, though we adjust it
 for the last index anyway.
 */
#define pintarray(pIntArray, size) \
    do \
    { \
        int const numCharsPerInt = 13; \
        char str[size * numCharsPerInt + 1]; \
        for (int i=0; i<size; i++) snprintf(&str[i * numCharsPerInt], numCharsPerInt + 1, "%11d, ", pIntArray[i]); \
        str[size * numCharsPerInt - 2] = '\0'; \
        pinfo("%s%s()%s: "#pIntArray"[%d] = %s{%s}%s", \
                ANSILightMagenta, __FUNCTION__, ANSINoColor, size, \
                ANSILightBlue, str, ANSINoColor); \
    } while (0)

/*
 Logs a float array.
 8 characters per float = 6 for "+9.999", 2 for ", ".
 Note the length of the arary is +1, so that snprintf() has
 room to write the NULL terminater, though we adjust it
 for the last index anyway.
 */
#define pfloatarray(pFloatArray, size) \
    do \
    { \
        int const numCharsPerFloat = 8; \
        char str[size * numCharsPerFloat + 1]; \
        for (int i=0; i<size; i++) snprintf(&str[i * numCharsPerFloat], numCharsPerFloat + 1, "%6.3f, ", pFloatArray[i]); \
        str[size * numCharsPerFloat - 2] = '\0'; \
        pinfo("%s%s()%s: "#pFloatArray"[%d] = %s{%s}%s", \
                ANSILightMagenta, __FUNCTION__, ANSINoColor, size, \
                ANSILightBlue, str, ANSINoColor); \
    } while (0)

/*
 Logs a VROVector3f.
 */
#define pvec3(vec) \
    do \
    { \
        pinfo("%s%s()%s: "#vec" = %s%s%s", \
                ANSILightMagenta, __FUNCTION__, ANSINoColor, \
                ANSILightBlue, (vec).toString().c_str(), ANSINoColor); \
    } while (0)

/*
 Logs a pointer to VROVector3f.
 */
#define ppvec3(pVec) \
    do \
    { \
        pinfo("%s%s()%s: *"#pVec" = %s%s%s", \
                ANSILightMagenta, __FUNCTION__, ANSINoColor, \
                ANSILightBlue, pVec->toString().c_str(), ANSINoColor); \
    } while (0)

/*
 Logs a 4X4 matrix.
 */
#define pmatrix(m) \
    do \
    { \
        pinfo("%s%s()%s: "#m"[] =", ANSILightMagenta, __FUNCTION__, ANSINoColor); \
        pinfo("%s[%6.3f, %6.3f, %6.3f, %6.3f,%s", ANSILightBlue, m[0], m[4], m[8], m[12], ANSINoColor); \
        pinfo("%s %6.3f, %6.3f, %6.3f, %6.3f,%s", ANSILightBlue, m[1], m[5], m[9], m[13], ANSINoColor); \
        pinfo("%s %6.3f, %6.3f, %6.3f, %6.3f,%s", ANSILightBlue, m[2], m[6], m[10], m[14], ANSINoColor); \
        pinfo("%s %6.3f, %6.3f, %6.3f, %6.3f]%s", ANSILightBlue, m[3], m[7], m[11], m[15], ANSINoColor); \
    } while (0)

/*
 Logs a VROBoundingBox.
 */
#define pbb(bb) \
    do \
    { \
        pinfo("%s%s()%s: "#bb" = %s%s%s", \
                ANSILightMagenta, __FUNCTION__, ANSINoColor, \
                ANSILightBlue, (bb).toString().c_str(), ANSINoColor); \
    } while (0)

// --------------------------------
// markers
#define plocation() pinfo("[%s%s:%s %s%s()%s, %sline %d%s]", \
        ANSILightCyan, __FILE__, ANSINoColor, \
        ANSILightMagenta, __FUNCTION__, ANSINoColor, \
        ANSILightBlue, __LINE__, ANSINoColor)
#define pcr() pinfo(" ")
#define pline() pline1()
#define cpline(tag) cpinfo(tag, "----------------------------------------------------------------")
#define pline1() pinfo("----------------------------------------------------------------")
#define pline2() pinfo("================================================================")
#define predline() pinfo("%s################################################################%s", ANSILightRed, ANSINoColor);
#define pyellowline() pinfo("%s################################################################%s", ANSIYellow, ANSINoColor);
#define phighlight(color) pinfo("%s****************************************************************%s", color, ANSINoColor)
#define pfunc() \
    do \
    { \
        pinfo("%s%s()----------------------------------------------------------------%s", ANSILightMagenta, __FUNCTION__, ANSINoColor); \
    } while (0)
#define pfuncfull() \
    do \
    { \
        pinfo("%s%s ----------------------------------------------------------------%s", ANSILightMagenta, __func__, ANSINoColor); \
    } while (0)
#define pfunction() \
    do \
    { \
        pinfo("%s%s:%s():%s%d%s", ANSILightMagenta, __FILE__, __FUNCTION__, ANSIMagenta, __LINE__, ANSINoColor); \
    } while (0)
#define pfuncstart() \
    do \
    { \
        pinfo("%s%s() starting...%s", ANSILightMagenta, __FUNCTION__, ANSILightGreen, ANSINoColor); \
    } while (0)
#define pfuncend() \
    do \
    { \
        pinfo("%s%s() %sending.%s", ANSILightMagenta, __FUNCTION__, ANSILightGreen, ANSINoColor); \
    } while (0)
#define pmarker() \
    do \
    { \
        pinfo("%s%s(): %sLine %d.%s", ANSILightMagenta, __FUNCTION__, ANSILightGreen, __LINE__, ANSINoColor); \
    } while (0)

#if VRO_SECURITY_LOG
#define psecurityinfo(message,...) pinfo("Viro-Security:: "#message, ##__VA_ARGS__)
#define psecurityerr(message,...) perr("Viro-Security:: "#message, ##__VA_ARGS__)
#else
#define psecurityinfo(message,...) ((void)0)
#define psecurityerr(message,...) ((void)0)
#endif

/////////////////////////////////////////////////////////////////////////////////
//
//  Common: Stack Trace
//
/////////////////////////////////////////////////////////////////////////////////
#pragma mark -
#pragma mark (Common) Stack Trace

/*
 Print out a message and a stacktrace, including the current stackframe
 */
void pstack(const char *fmt, ...)
    __attribute__ ((format(printf, 1, 2)));

/*
 Print out a stacktrace, including the current stackframe
 */
void pstack();

/////////////////////////////////////////////////////////////////////////////////
//
//  Common: Abort
//
/////////////////////////////////////////////////////////////////////////////////
#pragma mark -
#pragma mark Common: Abort

/*
 Abort the program.
 Print out a stacktrace; then cause a SEG fault to halt execution.
 SEG fault will be at 0xdecafbad.
 */
#define pabort(...) _pabort(__FILE__, __LINE__, __func__, ## __VA_ARGS__)

void _pabort(const char *file, int line, const char *func) __attribute__((noreturn));
void _pabort(const char* file, int line, const char *func, const char *fmt, ...)
    __attribute__ ((format(printf, 4, 5), noreturn));

/////////////////////////////////////////////////////////////////////////////////
//
//  Common: Assert with Message
//
/////////////////////////////////////////////////////////////////////////////////
#pragma mark -
#pragma mark Common: Assert with Message

/*
 Abort the program if 'condition' is false.
 Print out a stacktrace and formatted message; then cause a SEG fault to halt execution.
 SEG fault will be at 0xdecafbad.
 */
# define passert_msg(condition, ...)                                    \
    do {                                                                \
        if (! (condition))                                    \
            _pabort(__FILE__, __LINE__, __func__, ## __VA_ARGS__);      \
    } while (false)

/*
 Only defined, *even in DEBUG builds*, if the follow is #defined: PASSERT_INNERLOOPS
 */
#if (PASSERT_INNERLOOPS_ENABLED && !NDEBUG)
    // Debug build with inner loop assert enabled.
    /*
     Defined for Debug mode, and if we want inner loop assertions.
     */
# define passert_innerloop(condition, ...)                              \
    do {                                                                \
        if (! (condition))                                    \
            _pabort(__FILE__, __LINE__, __func__, ## __VA_ARGS__); \
    } while (false)

#else
    /*
     Prevent any code generation.
     */
# define passert_innerloop(...) ((void)(0))
#endif

/*
 Dev only passert_msg.  Asserts using this will not run in release builds.
 Meant for cases where in development we want to abort, but in production we
 want to continue (and there is code to handle that case).
 */
#if !NDEBUG
# define passert_msg_dev(condition, ...)                                \
    do {                                                                \
        if (! (condition))                                    \
            _pabort(__FILE__, __LINE__, __func__, ## __VA_ARGS__);      \
    } while (false)


#else
    /*
     Prevent any code generation.
     */
# define passert_msg_dev(...) ((void)(0))
#endif
