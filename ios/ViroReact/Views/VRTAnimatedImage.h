//
//  VRTAnimatedImage.h
//  React
//
//  Copyright © 2018 Viro Media. All rights reserved.
//

#import "VRTImage.h"

@interface VRTAnimatedImage : VRTImage

-(instancetype)initWithBridge:(RCTBridge *)bridge;

@property (nonatomic, assign) BOOL paused;
@property (nonatomic, assign) BOOL loop;
@property (nonatomic, assign) int currentPendingImage;
    
@end
