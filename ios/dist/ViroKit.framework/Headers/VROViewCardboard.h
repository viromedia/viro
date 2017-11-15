//
//  VROViewCardboard.h
//  ViroRenderer
//
//  Created by Raj Advani on 4/28/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "VROView.h"
#import "GVRCardboardView.h"

@interface VROViewCardboard : GVRCardboardView <GVRCardboardViewDelegate, VROView>

@property (nonatomic) IBInspectable BOOL testingMode;

@end
