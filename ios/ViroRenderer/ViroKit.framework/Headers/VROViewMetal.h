//
//  VROViewMetal.h
//  ViroRenderer
//
//  Created by Raj Advani on 4/22/16.
//  Copyright © 2016 Viro Media. All rights reserved.
//

#import <Metal/Metal.h>
#import <MetalKit/MetalKit.h>
#import <memory>
#import "VROView.h"

@interface VROViewMetal : MTKView <MTKViewDelegate, VROView>

@end
