//
//  VROViewCardboard.h
//  ViroRenderer
//
//  Created by Raj Advani on 4/28/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <GLKit/GLKit.h>
#import <UIKit/UIKit.h>
#import "VROView.h"
#import "GVROverlayView.h"

@interface VROViewCardboard : GLKView <VROView, GVROverlayViewDelegate>

@property (nonatomic) IBInspectable BOOL testingMode;

@end
