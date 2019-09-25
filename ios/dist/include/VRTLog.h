//
//  VROLog.h
//  React
//
//  Copyright © 2016 Viro Media. All rights reserved.
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