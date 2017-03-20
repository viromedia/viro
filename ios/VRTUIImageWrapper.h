//
//  VRTUIImageWrapper.h
//  ViroReact
//
//  Created by Vik Advani on 3/20/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <React/RCTConvert.h>

@interface VRTUIImageWrapper : NSObject
  @property (nonatomic, copy) UIImage *image;
@end

@interface RCTConvert (VRTUIImageWrapper)
  + (VRTUIImageWrapper *)VRTUIImageWrapper:(id)json;
@end


