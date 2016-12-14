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
#import <UIKit/UIKit.h>
#include <memory>

class VRORenderContext;
class VROTexture;

unsigned char *VROExtractRGBA8888FromImage(UIImage *image, size_t *length);
void *VROImageLoadTextureDataRGBA8888(const char *resource, size_t *bitmapLength, int *width, int *height);

void initBlankTexture(const VRORenderContext &context);
std::shared_ptr<VROTexture> getBlankTexture();

#endif /* VROImageUtil_h */
