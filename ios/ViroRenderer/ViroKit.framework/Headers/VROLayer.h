//
//  VROLayer.h
//  ViroRenderer
//
//  Created by Raj Advani on 10/13/15.
//  Copyright © 2015 Viro Media. All rights reserved.
//

#ifndef VROLayer_h
#define VROLayer_h

#include <stdio.h>
#include "VRODriver.h"
#include "VRORect.h"
#include <vector>
#include <stack>
#include <memory>
#include "VROByteBuffer.h"
#include "VROGeometry.h"
#include "VROMatrix4f.h"
#include "VRONode.h"
#include "VROMaterial.h"
#include "VROVector3f.h"

class VROLayer : public VRONode {
    
public:
    
    VROLayer();
    virtual ~VROLayer();
    
    void setContents(UIImage *image);
    void setContents(int width, int height, CGContextRef bitmapContext,
                     VRODriver &driver);
    
    void setFrame(VRORect frame);
    void setPosition(VROVector3f point);
    
    VRORect getFrame() const;
    
    std::shared_ptr<VROMaterial> getMaterial();
    
private:
    
    /*
     Position and size of the layer in 3D space or in its parent layer's
     2D space.
     */
    VRORect _frame;
    
    void onFrameUpdate();
    
};

#endif /* VROLayer_h */
