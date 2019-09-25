//
//  VROLog.h
//  React
//
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
//

#import <Foundation/Foundation.h>

/**
 * Used only for logging out information within VRT components.
 * NOTE: This has no relation to VROLog.
 */
@interface VRTLog : NSObject
    /**
     * Static Flag used to enable / disable VRT debug information
     * logged out onto the console in debug builds (data is not
     * logged in release builds).
     */
    + (BOOL)debugEnabled;
    + (void)setdebugEnabled:(BOOL)debugEnabled;

    /**
     * Prints out logs into the console. This is only
     * enabled in debug builds and if debugEnabled is True (Enabled
     * true RCTMenu).
     */
    + (void)debug:(NSString *)log;
@end
