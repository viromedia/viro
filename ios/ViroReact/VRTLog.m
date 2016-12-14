//
//  VROLog.m
//  React
//
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import "VRTLog.h"

@implementation VRTLog

static BOOL sDefaultDebugEnabled = NO;

+(void)setdebugEnabled:(BOOL)debugEnabled {
  sDefaultDebugEnabled = debugEnabled;
}

+(BOOL)debugEnabled{
  return sDefaultDebugEnabled;
}

+(void)debug:(NSString *)log{
  if (sDefaultDebugEnabled){
    NSLog(@"%@", log);
  }
}
@end