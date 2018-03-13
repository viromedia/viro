//
//  VROImageUtil.h
//  ViroRenderer
//
//  Created by Raj Advani on 10/21/15.
//  Copyright © 2015 Viro Media. All rights reserved.
//

#ifndef VROImageUtil_h
#define VROImageUtil_h

#include <stdio.h>
#include <memory>
#include "VRODefines.h"

#if VRO_PLATFORM_IOS
#import <UIKit/UIKit.h>
#endif

class VRORenderContext;
class VROTexture;
enum class VROTextureType;

void initBlankTexture(const VRORenderContext &context);
std::shared_ptr<VROTexture> getBlankTexture(VROTextureType type);

void initPointCloudTexture();
std::shared_ptr<VROTexture> getPointCloudTexture();

#endif /* VROImageUtil_h */
