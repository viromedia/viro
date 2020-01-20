//
//  VROLog.h
//  ViroRenderer
//
//  Created by Raj Advani on 10/21/15.
//  Copyright © 2015 Viro Media. All rights reserved.
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

#include "VRODefines.h"
#pragma once

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

#define ANSINoColor "\e[0m" // reset / normal
#define ANSIBold "\e[1m" // bold
#define ANSIBlack "\e[0;30m"
#define ANSIRed "\e[0;31m"
#define ANSIGreen "\e[0;32m"
#define ANSIBrown "\e[0;33m"
#define ANSIDarkYellow "\e[0;33m"
#define ANSIBlue "\e[0;34m"
#define ANSIMagenta "\e[0;35m"
#define ANSICyan "\e[0;36m"
#define ANSILightGray "\e[0;37m"
#define ANSIDarkGray "\e[1;30m"
#define ANSILightRed "\e[1;31m"
#define ANSILightGreen "\e[1;32m"
#define ANSIYellow "\e[1;33m"
#define ANSILightBlue "\e[1;34m"
#define ANSILightMagenta "\e[1;35m"
#define ANSILightCyan "\e[1;36m"
#define ANSIWhite "\e[1;37m"
#define ANSIDefault "\e[1;39m"
#define ANSIBackBlack "\e[0;40m"
#define ANSIBackRed "\e[0;41m"
#define ANSIBackGreen "\e[0;42m"
#define ANSIBackBrown "\e[0;43m"
#define ANSIBackBlue "\e[0;44m"
#define ANSIBackMagenta "\e[0;45m"
#define ANSIBackCyan "\e[0;46m"
#define ANSIBackLightGray "\e[0;47m"
#define ANSIBackDarkGray "\e[1;40m"
#define ANSIBackLightRed "\e[1;41m"
#define ANSIBackLightGreen "\e[1;42m"
#define ANSIBackYellow "\e[1;43m"
#define ANSIBackLightBlue "\e[1;44m"
#define ANSIBackLightMagenta "\e[1;45m"
#define ANSIBackLightCyan "\e[1;46m"
#define ANSIBackWhite "\e[1;47m"
#define ANSIBackDefault "\e[1;49m"

#elif VRO_PLATFORM_IOS || VRO_PLATFORM_MACOS

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

// info:
#define pinfo(...) \
    do { \
        __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__); \
    } while (0)

// warn:
#define pwarn(...) \
    do { \
        __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__); \
    } while (0)

// error:
#define perr(message, ...) \
    do { \
        __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, \
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

#define pgllabel(message,...) ((void)0)

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

#elif VRO_PLATFORM_IOS || VRO_PLATFORM_MACOS

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

#endif

/////////////////////////////////////////////////////////////////////////////////
//
//  WASM: Logging
//
/////////////////////////////////////////////////////////////////////////////////
#pragma mark -
#pragma mark WASM Logging

#if VRO_PLATFORM_WASM

#include <cstdarg>
#include <cstdio>
#include "assert.h"

#define ANSINoColor "\e[0m" // reset / normal
#define ANSIBlack "\e[0;30m"
#define ANSIRed "\e[0;31m"
#define ANSIGreen "\e[0;32m"
#define ANSIBrown "\e[0;33m"
#define ANSIDarkYellow "\e[0;33m"
#define ANSIBlue "\e[0;34m"
#define ANSIMagenta "\e[0;35m"
#define ANSICyan "\e[0;36m"
#define ANSILightGray "\e[0;37m"
#define ANSIDarkGray "\e[1;30m"
#define ANSILightRed "\e[1;31m"
#define ANSILightGreen "\e[1;32m"
#define ANSIYellow "\e[1;33m"
#define ANSILightBlue "\e[1;34m"
#define ANSILightMagenta "\e[1;35m"
#define ANSILightCyan "\e[1;36m"
#define ANSIWhite "\e[1;37m"
#define ANSIDefault "\e[1;39m"
#define ANSIBackBlack "\e[0;40m"
#define ANSIBackRed "\e[0;41m"
#define ANSIBackGreen "\e[0;42m"
#define ANSIBackBrown "\e[0;43m"
#define ANSIBackBlue "\e[0;44m"
#define ANSIBackMagenta "\e[0;45m"
#define ANSIBackCyan "\e[0;46m"
#define ANSIBackLightGray "\e[0;47m"
#define ANSIBackDarkGray "\e[1;40m"
#define ANSIBackLightRed "\e[1;41m"
#define ANSIBackLightGreen "\e[1;42m"
#define ANSIBackYellow "\e[1;43m"
#define ANSIBackLightBlue "\e[1;44m"
#define ANSIBackLightMagenta "\e[1;45m"
#define ANSIBackLightCyan "\e[1;46m"
#define ANSIBackWhite "\e[1;47m"
#define ANSIBackDefault "\e[1;49m"

#define passert(condition) (assert(condition))
#define pverbose(message,...) ((void)0)

#define pdebug(message,...) \
do { \
\
} while (0)

#define pinfo(message,...) \
do { \
printf(#message"\n", ##__VA_ARGS__); \
} while (0)

#define pwarn(message,...) \
do { \
printf(#message"\n", ##__VA_ARGS__); \
} while (0)

#define perr(message,...) \
do { \
printf("Error: "#message"\n", ##__VA_ARGS__); \
} while (0)

#define pfatal(message,...) \
do { \
printf("Fatal Error: "#message"\n", ##__VA_ARGS__); \
} while (0)

#define pgllabel(message,...) \
do { \
char str[1024]; \
sprintf(str, #message, ##__VA_ARGS__); \
 \
} while (0)

#endif

/////////////////////////////////////////////////////////////////////////////////
//
//  Common: Logging
//
/////////////////////////////////////////////////////////////////////////////////
#pragma mark -
#pragma mark Common Logging


// --------------------------------
// markers
#define plocation() pinfo("[%s%s:%s %s%s()%s, %sline %d%s]", \
        ANSILightCyan, __FILE__, ANSINoColor, \
        ANSILightMagenta, __FUNCTION__, ANSINoColor, \
        ANSILightBlue, __LINE__, ANSINoColor)
#define pcr() pinfo(" ")
#define pline() pline1()
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
 Abort the program, printing out a stacktrace then creating a SEG fault to halt.
 The SEG fault will be 0xdecafbad.
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

# define passert_msg(condition, ...)                                    \
    do {                                                                \
        if (! (condition))                                    \
            _pabort(__FILE__, __LINE__, __func__, ## __VA_ARGS__);      \
    } while (false)
