//
//  VRTBundleURLProvider.h
//  ViroReact
//
//  Created by Vik Advani on 2/1/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface VRTBundleURLProvider : NSObject
- (NSURL *)jsBundleURLForBundleRoot:(NSString *)bundleRoot fallbackResource:(NSString *)resourceName;
@end
