
//
//  VRTMaterialChangedDelegate.h
//  ViroRenderer
//
//  Copyright © 2018 Viro Media. All rights reserved.
//

#ifndef VRTMaterialChangedDelegate_h
#define VRTMaterialChangedDelegate_h

#import <Foundation/Foundation.h>

@protocol VRTMaterialChangedDelegate <NSObject>

- (void)videoMaterialDidChange:(NSString *)materialName;

@end

#endif /* VRTMaterialChangedDelegate_h */
